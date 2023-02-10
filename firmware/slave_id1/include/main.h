#ifndef MAIN_H__
#define MAIN_H__

#include <Arduino.h>
#include <FreeRTOSConfig.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define DHTPIN                    25
#define DHTTYPE                   DHT11

#define BOARD                     ("ESP-32")
#define GASPIN                    (35) 
#define TYPE                      ("MQ-2") //MQ2 or other MQ Sensor, if change this verify your a and b values.
#define VOLTAGE_RESOLUTION        (3.3) 
#define ADC_BIT_RESOLUTION        (12)
#define RatioMQ2CleanAir          (9.83) //RS / R0 = 9.83 ppm

#define LIGHTPIN                  27
#define BUZZERPIN                 33

const char* ssid = "Dao Van Quan";
const char* password = "88888888";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

#endif