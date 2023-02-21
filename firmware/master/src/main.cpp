
#include "main.h"
#include "mqtt.h"
#include "ota.h"

String url = "";
String para_nodeid1 = "";
String para_nodeid2 = "";

recv_node_t recv_temp_nodeid[2][3];

String httpGETRequest(const char* url);
void ota_task(void *arg);

void setup() {
  Serial.begin(9600);

  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  connect_to_broker();

  OTADRIVE.setInfo(key, version);
  xTaskCreate(ota_task, "OTA Task", 1024 * 5, NULL, 1, NULL);
}

void loop() {
  if (!client.connected())
  {
    connect_to_broker();
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String data = "";

  for (int i = 0; i < length; i ++) 
  { 
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
  Serial.println();
  if (strcmp(topic, MQTT_TEMPERATURE_TOPIC_NODEID1) == 0 && (recv_temp_nodeid[0][0] == FALSE))
  {
    para_nodeid1 = "&field1=" + data;
    recv_temp_nodeid[0][0] = TRUE;
  } else if (strcmp(topic, MQTT_HUMIDITY_TOPIC_NODEID1) == 0 && (recv_temp_nodeid[0][0] == TRUE) && (recv_temp_nodeid[0][1] == FALSE))
  {
    para_nodeid1 += "&field2=" + data;
    recv_temp_nodeid[0][1] = TRUE;
  } else if (strcmp(topic, MQTT_GAS_TOPIC_NODEID1) == 0 && (recv_temp_nodeid[0][1] == TRUE) && (recv_temp_nodeid[0][0] == TRUE))
  {
    para_nodeid1 += "&field3=" + data;
    url = urlThingSpeak_nodeID1 + para_nodeid1;
    Serial.println(url);
    Serial.println();
    httpGETRequest(url.c_str());
    recv_temp_nodeid[0][0] = FALSE;
    recv_temp_nodeid[0][1] = FALSE;
    para_nodeid1 = "";
  }
  
  if (strcmp(topic, MQTT_TEMPERATURE_TOPIC_NODEID2) == 0 && (recv_temp_nodeid[1][0] == FALSE))
  {
    para_nodeid2 = "&field1=" + data;
    recv_temp_nodeid[1][0] = TRUE;
  } else if (strcmp(topic, MQTT_HUMIDITY_TOPIC_NODEID2) == 0 && (recv_temp_nodeid[1][0] == TRUE) && (recv_temp_nodeid[1][1] == FALSE))
  {
    para_nodeid2 += "&field2=" + data;
    recv_temp_nodeid[1][1] = TRUE;
  } else if (strcmp(topic, MQTT_LED_TOPIC) == 0 && (recv_temp_nodeid[1][0] == TRUE) && (recv_temp_nodeid[1][1] == TRUE))
  {
    para_nodeid2 += "&field3=" + data;
    url = urlThingSpeak_nodeID2 + para_nodeid2;
    Serial.println(url);
    Serial.println();
    httpGETRequest(url.c_str());
    recv_temp_nodeid[1][0] = FALSE;
    recv_temp_nodeid[1][1] = FALSE;
    para_nodeid2 = "";
  }
}

String httpGETRequest(const char* url)
{
  HTTPClient http;

  http.begin(url);
  int responseCode = http.GET();
  String responseBody = "{}";
  if(responseCode > 0)
  {
    Serial.print("responseCode:");
    Serial.println(responseCode);
    responseBody = http.getString();
  }
  else
  {
    Serial.print("Error Code: ");
    Serial.println(responseCode);
  }
  http.end();
  return responseBody;
}

void ota_task(void *arg)
{
  Serial.println("Hello from OTA Task");

  for (;;)
  {
    Serial.print("Ver: ");
    Serial.println(version);
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("Check update version");
      OTADRIVE.updateFirmware();
    }

    client.publish(MQTT_OTA_TOPIC, version.c_str());

    vTaskDelay(180000 / portTICK_PERIOD_MS);
  }
}
