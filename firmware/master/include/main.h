#ifndef MAIN_H__
#define MAIN_H__

#include <Arduino.h>
#include <FreeRTOSConfig.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>

const char* ssid = "Dao Van Quan";
const char* password = "88888888";

typedef enum {
  FALSE,
  TRUE
} recv_node_t;

String urlThingSpeak_nodeID1 = "https://api.thingspeak.com/update?api_key=EZKH261OZ4RXTTT7";
String urlThingSpeak_nodeID2 = "https://api.thingspeak.com/update?api_key=W0URL6A5PTC0OYZG";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

#endif