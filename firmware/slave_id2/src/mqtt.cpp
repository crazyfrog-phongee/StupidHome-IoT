#include "mqtt.h"

void setup_wifi(void)
{
    Serial.print("Connecting to ");
    Serial.print(ssid);
    WiFi.begin(ssid, password);

    int count = 0;
    while (WiFi.status() != WL_CONNECTED) {
        count++;
        delay(500);
        Serial.print(".");
        if (count == 5)
        {
            Serial.println();
            count = 0;
            break;
        }
    }
    
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }
}

void connect_to_broker() 
{
    int count = 0;
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = "NodeIOT";
        clientId += String(random(0xffff), HEX);

        count++;

        if (client.connect(clientId.c_str())) {
        Serial.println("connected");
        client.subscribe(MQTT_CONTROL_TOPIC);
        } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 2 seconds");
        delay(2000);
        }

        if (count == 5)
        {
            break;
        }
    }
}
