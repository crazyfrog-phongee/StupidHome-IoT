
#include "main.h"
#include "mqtt.h"

#include <DHT.h>
#include <Adafruit_Sensor.h>
#include "LiquidCrystal_I2C.h"

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

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

  lcd.backlight();// turn on lcd backlight
  lcd.init();// initialize lcd

  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  connect_to_broker();

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

    lcd.clear();
    lcd.setCursor(0,0);// set the cursor on the first row and column
    lcd.print("Humidity: ");
    lcd.print(h);//print the humidity
    lcd.print("%");
    lcd.setCursor(0,1);//set the cursor on the second row and first column
    lcd.print("Temp:     ");
    lcd.print(t);//print the temperature
    lcd.print(char(223));
    lcd.print("C");

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
    Serial.println(sensor_Aout); /* Read value printed */
    client.publish(MQTT_GAS_TOPIC, String(sensor_Aout).c_str());

    if (sensor_Aout > 1800)
    {
      Serial.println("Gas");
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(BUZZERPIN, HIGH);
    }
    else
    {
      Serial.println("No Gas");
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(BUZZERPIN, LOW);
    }

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