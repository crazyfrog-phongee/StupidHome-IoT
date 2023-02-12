
#include "main.h"
#include "mqtt.h"
#include "lcd.h"

#include <DHT.h>
#include <Adafruit_Sensor.h>


DHT dht(DHTPIN, DHTTYPE);

void temp_task(void *arg);
void gas_task(void *arg);
void light_task(void *arg);

void setup()
{
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  // pinMode(GASPIN, INPUT);
  pinMode(LIGHTPIN, INPUT);
  pinMode(BUZZERPIN, OUTPUT);

  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  connect_to_broker();

  initLCD();

  xTaskCreate(temp_task, "TempTask", 1024 * 5, NULL, 1, NULL);
  xTaskCreate(gas_task, "GasTask", 1024 * 5, NULL, 2, NULL);
  // xTaskCreate(light_task, "LightTask", 1024 * 5, NULL, 2, NULL);
}

void loop()
{
  client.loop();
}

void temp_task(void *arg)
{
  Serial.println("Hello from Measure Task");

  for (;;)
  {
    delay(2000);
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
    if (isnan(h) || isnan(t) || isnan(f))
    {
      Serial.println(F("Failed to read from DHT sensor!"));
    }
    else
    {
      client.publish(MQTT_TEMPERATURE_TOPIC, String(t).c_str());
      client.publish(MQTT_HUMIDITY_TOPIC, String(h).c_str());
    }
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.println(F("°C "));

    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

void gas_task(void *arg)
{
  Serial.println("Hello from Gas Measurement Task");
  char* status_msg = "";
  for (;;)
  {
    int sensor_Aout = analogRead(GASPIN); /* Analog value read function */
    Serial.print("Gas Sensor: ");
    Serial.println(sensor_Aout); /* Read value printed */
    client.publish(MQTT_GAS_TOPIC, String(sensor_Aout).c_str());

    if (sensor_Aout > 1800)
    {
      Serial.println("Gas");
      status_msg = "Nguy Hiem";
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(BUZZERPIN, HIGH);
    }
    else
    {
      Serial.println("No Gas");
      status_msg = "An Toan";
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(BUZZERPIN, LOW);
    }

    displayLCD(sensor_Aout, status_msg);

    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

void light_task(void *arg)
{
  Serial.println("Hello from Light Task");

  for (;;)
  {
    int value = digitalRead(LIGHTPIN);
    digitalWrite(LED_BUILTIN, value);
    Serial.println(value);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}