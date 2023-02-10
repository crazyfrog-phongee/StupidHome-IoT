#include <Arduino.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//----------------- Khai bao cac bien hang --------------------------

const char *ssid = "DaoVanQuan";                 //Su dung Wifi
const char *password = "123456789";
const char *mqtt_server = "172.20.10.2";   // Sever la cai node Red chay tren may tinh cua minh luon ket noi may khac thi dia chi IP cua may khac
const int mqtt_port = 1883;                // ???????????????????????????????
const char *mqtt_id = "esp8266";            
const char *topic_subcribe = "to-esp8266";
const char *topic_publish = "from-esp8266";

WiFiClient client;                          // Tạo 1 cái client để nó truyền vào
PubSubClient mqtt_client(client);           // Quan lớp này mới đọc dữ liệu này

// Callback
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Recived data from: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (size_t i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("------------------------------------------");
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Connecting to Wifi... ");
  WiFi.begin(ssid, password);
  WiFi.reconnect();      // Ham nay tu dong reconect
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);         // Cho no ket noi
    Serial.print(".");
  }
  // Sau khi et noi thanh cong thi hien thi ra nhung cai sau
  Serial.print("Connected to Wifi ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.print(WiFi.localIP());
  Serial.println();


  delay(1000);

  // Ket noi Mqtt sever
  mqtt_client.setServer(mqtt_server, mqtt_port);
  mqtt_client.setCallback(callback);
  Serial.println("Connecting to mqtt... ");
  while (!mqtt_client.connect(mqtt_id))
  {
    delay(500);
  }
  Serial.println("Connected to mqtt ");
  mqtt_client.subscribe(topic_subcribe);
  mqtt_client.publish(topic_publish, "Hello Server");
}

void loop() {
  // put your main code here, to run repeatedly:
  mqtt_client.loop();
}