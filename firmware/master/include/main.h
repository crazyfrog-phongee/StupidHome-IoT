#ifndef MAIN_H__
#define MAIN_H__

#include <Arduino.h>
#include <FreeRTOSConfig.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>

const char* ssid = "Dao Van Quan";
const char* password = "88888888";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

#endif