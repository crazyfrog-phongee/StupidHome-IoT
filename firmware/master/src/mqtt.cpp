#include "mqtt.h"

/* Setup device connect to WiFi Network */
void setup_wifi(void)
{
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

/* Setup devive connect to Broker MQTT Server */
void connect_to_broker() 
{
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = "NodeIOT";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            client.subscribe(MQTT_TEMPERATURE_TOPIC_NODEID1);
            client.subscribe(MQTT_HUMIDITY_TOPIC_NODEID1);
            client.subscribe(MQTT_TEMPERATURE_TOPIC_NODEID2);
            client.subscribe(MQTT_HUMIDITY_TOPIC_NODEID2);
            client.subscribe(MQTT_GAS_TOPIC_NODEID1);
            client.subscribe(MQTT_LED_TOPIC);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 2 seconds");
            delay(2000);
        }
    }
}
