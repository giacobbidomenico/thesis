#include <WiFi.h>
#include <PubSubClient.h>
#include "Configuration.h"
#include "communication/MqttClientManager.h"

MqttClientManager* mqttManager;


void setup() {
  Serial.begin(BOUND_RATE);
  mqttManager = new MqttClientManager(SSID, PASSWORD, TOPIC, MQTT_SERVER, PORT);
  mqttManager->establishMqttConnection();
}

void loop() {
  mqttManager->tick();
}