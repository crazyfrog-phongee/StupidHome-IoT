#ifndef MAIN_H__
#define MAIN_H__

#include <Arduino.h>
#include <FreeRTOSConfig.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <MQUnifiedsensor.h>

#define DHTPIN                    25
#define DHTTYPE                   DHT11

#define BOARD                     ("ESP-32")
#define GASPIN                    (35) 
#define TYPE                      ("MQ-2") //MQ2 or other MQ Sensor, if change this verify your a and b values.
#define VOLTAGE_RESOLUTION        (3.3) 
#define ADC_BIT_RESOLUTION        (12)
#define RatioMQ2CleanAir          (9.83) //RS / R0 = 9.83 ppm

#define LIGHTPIN                  27
#define RELAYPIN                  32
#define BUTTONPIN                 26

const char* ssid = "Dao Van Qua";
const char* password = "88888888";

typedef enum
{
  DISABLE,
  ENABLE,
  LOCAL
} status_control_t;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

DHT dht(DHTPIN, DHTTYPE);
MQUnifiedsensor MQ2(BOARD, VOLTAGE_RESOLUTION, ADC_BIT_RESOLUTION, GASPIN, TYPE);

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

status_control_t status_control;
uint8_t lamp_state = 0;
String state = "0";


#endif