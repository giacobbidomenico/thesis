#include <WiFi.h>
#include <PubSubClient.h>
#include "Configuration.h"
#include "communication/MqttManager.h"
#include "model/Model.h"

Model* model;

void setup() {
  Serial.begin(BOUND_RATE);
  pinMode(1, INPUT);
  model = new Model();
}

void loop() {
  model->tick();
}