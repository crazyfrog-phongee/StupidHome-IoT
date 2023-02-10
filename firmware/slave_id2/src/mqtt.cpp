#include "mqtt.h"

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

void connect_to_broker() 
{
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = "NodeIOT";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str())) {
        Serial.println("connected");
        client.subscribe(MQTT_CONTROL_TOPIC);
        } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 2 seconds");
        delay(2000);
        }
    }
}
