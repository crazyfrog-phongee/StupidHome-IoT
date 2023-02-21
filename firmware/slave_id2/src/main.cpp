
#include "main.h"
#include "mqtt.h"
#include "lcd.h"

void temp_task(void *arg);
void light_task(void *arg);
void local_control(void *arg);
void wifi_detect(void *arg);

int flag = 0;
static int created = 0;
int local_flag = 0;
int value;

void setup()
{
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LIGHTPIN, INPUT);
  pinMode(RELAYPIN, OUTPUT);
  pinMode(BUTTONPIN, INPUT_PULLUP);

  initLCD();

  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  connect_to_broker();

  xTaskCreate(temp_task, "TempTask", 1024 * 5, NULL, 2, NULL);
  xTaskCreate(light_task, "LightTask", 1024 * 5, NULL, 2, NULL);
  xTaskCreate(wifi_detect, "WiFiDetectTask", 1024 * 5, NULL, 1, NULL);
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

    displayLCD(h, t);

    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

void light_task(void *arg)
{
  Serial.println("Hello from Light Task");
  int enable_task = 0;
  for (;;)
  {
    if (status_control == DISABLE)
    {
      int value = digitalRead(LIGHTPIN);
      digitalWrite(LED_BUILTIN, value);
      state = (value == 1) ? "1" : "0";
      client.publish(MQTT_LED_TOPIC, state.c_str());
    }
    else if (status_control == ENABLE)
    {
      Serial.println("Enable Lamp Control Mode");
      state = (lamp_state == 1) ? "1" : "0";
      client.publish(MQTT_LED_TOPIC, state.c_str());
      digitalWrite(LED_BUILTIN, lamp_state);
    }
    
    if (status_control != LOCAL)
    {
      if (strcmp(state.c_str(), "1") == 0)
      {
        digitalWrite(RELAYPIN, HIGH);
        Serial.println("Turn on the lamp by light task button");
      } else if (strcmp(state.c_str(), "0") == 0)
      {
        digitalWrite(RELAYPIN, LOW);
        Serial.println("Turn off the lamp by light task button");
      }
    }

    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

void local_control(void *arg)
{
  Serial.println("Hello from Local Control Task");

  for (;;)
  {
    if (digitalRead(BUTTONPIN) == LOW && local_flag == 1)
    {
      delay(200);
      value = !value;
      digitalWrite(RELAYPIN, value);
      digitalWrite(LED_BUILTIN, value);

      if (value == 1)
      {
        Serial.println("Turn on the lamp by local button");
      }
      else
      {
        Serial.println("Turn off the lamp by local button");
      }
    }

    if (local_flag == 0)
    {
      vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
    else 
    {
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }
}

void wifi_detect(void *arg)
{
  Serial.println("Hello from WiFi Detect Task");

  for (;;)
  {
    if (WiFi.status() != WL_CONNECTED && flag == 0)
    {
      if (created == 0)
      {
        xTaskCreate(local_control, "LocalControlTask", 1024 * 5, NULL, 2, NULL);
        created = 1;
      }
      else
      {
        Serial.println("Resume Local Control Task");
      }
      local_flag = 1;
      flag = 1;
      status_control = LOCAL;
      value = (strcmp(state.c_str(), "1") == 0)? 1: 0;
    } 
    else if (WiFi.status() == WL_CONNECTED && flag == 1)
    {
      Serial.println("Suspend Local Control Task");
      flag = 0;
      status_control = DISABLE;
      local_flag = 0;
      if (!client.connected())
      {
        connect_to_broker();
      }
    }
    else if (WiFi.status() != WL_CONNECTED && flag == 1)
    {
      setup_wifi();
    }

    switch (WiFi.status())
    {
      case WL_CONNECTED:
        Serial.println("Status connection between Device and WiFi: Connected");
        break;
      case WL_DISCONNECTED:
        Serial.println("Status connection between Device and WiFi: Disconnected");
        break;
      case WL_NO_SSID_AVAIL:
        Serial.println("Status connection between Device and WiFi: No SSID Available");
        break;
      default:
        break;
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
      }
      else
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