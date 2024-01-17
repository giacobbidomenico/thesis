#include <WiFi.h>
#include <PubSubClient.h>
#include "Configuration.h"
#include "communication/MqttManager.h"

MqttManager* mqttManager;


void setup() {
  Serial.begin(BOUND_RATE);
  mqttManager = new MqttManager(SSID, PASSWORD, TOPIC, MQTT_SERVER, PORT);
  mqttManager->establishMqttConnection();
}

void loop() {
  mqttManager->tick();
}