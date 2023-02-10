//----------------- Khai Bao -----------------------------
#include "DHT.h"
#include <WiFi.h>
extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/timers.h"
}
#include <AsyncMqttClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>

//Thiết lập wifi
#define WIFI_SSID "DaoVanQuan"
#define WIFI_PASSWORD "123456789"

#define MQTT_HOST IPAddress(172, 20, 10, 2)  //specify your Raspberry Pi IP Address- Mình dùng máy tính nên nó sẽ là địa chỉ IP của máy tính
#define MQTT_PORT 1883

//MQTT Topics
#define MQTT_PUB_TEMP_DHT "esp32/dht/temperature"
#define MQTT_PUB_HUM_DHT  "esp32/dht/humidity"
#define MQTT_PUB_GAS "esp32/dht/gas"
unsigned long previousMillis = 0;   
const long interval = 10000;  

float temperature_DHT, humidity_DHT;
int MQ2; 

AsyncMqttClient mqttClient;

TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;


// Ham ket noi với wifi và MQTT
void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %dn", event);
  switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      connectToMqtt();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      xTimerStop(mqttReconnectTimer, 0); 
      xTimerStart(wifiReconnectTimer, 0);
      break;
  }
}

//-----------------------------------------------
void onMqttConnect(bool sessionPresent) {     // Hàm kết nối thành công với MQTT
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {   // HÀm trả về chưa kết nối thành công với MQTT
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}


//--------------------- Hàm gửi gói chủ đề -------------------
void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}




#define DHTPIN  25    // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// Cam bien khi Gas
#define mq2 34
 int khiga( )
 {
  int trangthai;
  int sensorA_out = analogRead(mq2);
  if (sensorA_out > 1800)
  {
    trangthai = 1;
  }
  else{
    trangthai = 0;
  }
  return trangthai;

 }

void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(1000);


  //------------- Tạo bộ hẹn giờ --------------------
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  connectToWifi();

}

void loop() {
  //-------------------- doc nhiet do ----------------------
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    //Read from DHT
    humidity_DHT = dht.readHumidity();
    temperature_DHT = dht.readTemperature();
    MQ2 = khiga();
    if (isnan(temperature_DHT) || isnan(humidity_DHT)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    // Publish an MQTT message on topic esp32/dht/temperature
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_TEMP_DHT, 1, true, String(temperature_DHT).c_str());                            
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i", MQTT_PUB_TEMP_DHT, packetIdPub1);
    Serial.printf("Message: %.2f \n", temperature_DHT);

    // Publish an MQTT message on topic esp32/dht/humidity
    uint16_t packetIdPub2 = mqttClient.publish(MQTT_PUB_HUM_DHT, 1, true, String(humidity_DHT).c_str());                            
    Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ", MQTT_PUB_HUM_DHT, packetIdPub2);
    Serial.printf("Message: %.2f \n", humidity_DHT);

    // Publish an MQTT message on topic esp32/dht/temperature
    uint16_t packetIdPub3 = mqttClient.publish(MQTT_PUB_GAS, 1, true, String(MQ2).c_str());                            
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i", MQTT_PUB_GAS, packetIdPub3);
    Serial.printf("Message: %d \n", MQ2);
  }
}
