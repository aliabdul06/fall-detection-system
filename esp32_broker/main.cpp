#include <WiFi.h>
#include "sMQTTBroker.h"

// ===== NETWORK CONFIGURATION =====
const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASSWORD";
// ==========================

sMQTTBroker broker;

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32 MQTT Broker starting...");

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Broker IP: ");
    Serial.println(WiFi.localIP());

    broker.init(1883);
    Serial.println("MQTT Broker started on port 1883");
}

void loop() {
    broker.update();
}