
#include "main.h"
#include "mqtt.h"
#include "ota.h"

String url = "";
String para_nodeid1 = "";
String para_nodeid2 = "";

recv_node_t recv_temp_nodeid[2][3];

/* 
  Hàm này sử dụng API httpGET để upload dữ liệu lên cloud. 
  Para: URL Cloud Server
  Return: response code từ server trả về
 */
String httpGETRequest(const char* url);

/* Task xử lý OTA */
void ota_task(void *arg);

void setup() {
  Serial.begin(9600); /* Setup tần số cổng Serial Debug */

  setup_wifi(); /* Function call setup_wifi() được define ở mqtt.cpp */
  client.setServer(MQTT_SERVER, MQTT_PORT); /* Setup Server MQTT Broker và PORT */
  client.setCallback(callback); /* Setup callback() fucntion khi gateway nhận được topic từ Broker */
  connect_to_broker();  /* Connect tới Broker */

  OTADRIVE.setInfo(key, version); /* Setup API Key và version để OTA */
  xTaskCreate(ota_task, "OTA Task", 1024 * 5, NULL, 1, NULL); /* Tạo task xử lý OTA */
}

void loop() {
  if (!client.connected())  /* Kiếm tra connection between Gateway and Broker*/
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

  /* Print the payload message */
  for (int i = 0; i < length; i ++) 
  { 
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
  Serial.println();

  /* Check message topic */
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
    httpGETRequest(url.c_str());    /* Upload data to Cloud Server */
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
    httpGETRequest(url.c_str());  /* Upload data to Cloud Server */
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
    if (WiFi.status() == WL_CONNECTED)  /* Kiểm tra connection between WiFI and Gateway*/
    {
      Serial.println("Check update version");
      OTADRIVE.updateFirmware();  /* Check and update OTA Version */
    }

    client.publish(MQTT_OTA_TOPIC, version.c_str());  /* Publish current version to MQTT_OTA_TOPIC */

    vTaskDelay(180000 / portTICK_PERIOD_MS);  /* Setup period 3 minutes for check updating version */
  }
}
