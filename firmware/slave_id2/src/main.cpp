
#include "main.h"
#include "mqtt.h"

#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <MQUnifiedsensor.h>

typedef enum {
  DISABLE,
  ENABLE
} status_control_t;

DHT dht(DHTPIN, DHTTYPE);
MQUnifiedsensor MQ2(BOARD, VOLTAGE_RESOLUTION, ADC_BIT_RESOLUTION, GASPIN, TYPE);
status_control_t status_control;
uint8_t lamp_state = 0;

void temp_task(void *arg);
void gas_task(void *arg);
void light_task(void *arg);

void setup()
{
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  // pinMode(GASPIN, INPUT);
  pinMode(LIGHTPIN, INPUT);

  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  connect_to_broker();

  xTaskCreate(temp_task, "TempTask", 1024 * 5, NULL, 1, NULL);
  // xTaskCreate(gas_task, "GasTask", 1024 * 5, NULL, 2, NULL);
  xTaskCreate(light_task, "LightTask", 1024 * 5, NULL, 2, NULL);

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
    delay(5000);
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

  for (;;)
  {
    int sensor_Aout = analogRead(GASPIN); /* Analog value read function */
    Serial.print("Gas Sensor: ");
    Serial.print(sensor_Aout); /* Read value printed */

    if (sensor_Aout > 1800)
    {
      Serial.println("Gas");
      digitalWrite(LED_BUILTIN, LOW);
    }
    else
    {
      Serial.println("No Gas");
      digitalWrite(LED_BUILTIN, HIGH);
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void light_task(void *arg)
{
  Serial.println("Hello from Light Task");
  String state = "";
  for (;;)
  {
    if (status_control == DISABLE)
    {
      int value = digitalRead(LIGHTPIN);
      digitalWrite(LED_BUILTIN, value);
      state = (value == 1)? "1" : "0";
      client.publish(MQTT_LED_TOPIC, state.c_str());
    } else if (status_control == ENABLE)
    {
      Serial.println("Hello ENABLED");
      state = (lamp_state == 1)? "1" : "0";
      client.publish(MQTT_LED_TOPIC, state.c_str());
      digitalWrite(LED_BUILTIN, lamp_state);
    }
    
    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String data = "";
  
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
  Serial.println();

  if (strcmp(topic, MQTT_CONTROL_TOPIC) == 0)
  {
    if (strcmp(data.c_str(), "ON") == 0 || strcmp(data.c_str(), "OFF") == 0)
    {
      status_control = ENABLE;
      if (strcmp(data.c_str(), "ON") == 0)
      {
        lamp_state = 1;
      } else 
      {
        lamp_state = 0;
      }
    } 
    else if (strcmp(data.c_str(), "disable") == 0)
    {
      status_control = DISABLE;
    }
  }
}