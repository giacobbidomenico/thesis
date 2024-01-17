#include <WiFi.h>
#include <PubSubClient.h>
#include "Configuration.h"
#include "communication/MqttManager.h"


void setup() {
  Serial.begin(BOUND_RATE);
  MqttManager::establishMqttConnection();
}

void loop() {
  MqttManager::tick();
}