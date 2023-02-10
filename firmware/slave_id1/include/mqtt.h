#ifndef MQTT_H__
#define MQTT_H__

#include <WiFi.h>
#include <PubSubClient.h>

#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_USER "trongphong1708"
#define MQTT_PASSWORD "Crazyfrog"

#define MQTT_TEMPERATURE_TOPIC  "NodeID1/Temp"
#define MQTT_HUMIDITY_TOPIC     "NodeID1/Humid"
#define MQTT_GAS_TOPIC          "NodeID1/Gas"

extern const char* ssid;
extern const char* password;

extern WiFiClient wifiClient;
extern PubSubClient client;


void setup_wifi(void);
void connect_to_broker(void); 
static void callback(char* topic, byte *payload, unsigned int length);

#endif