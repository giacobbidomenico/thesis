#include "Arduino.h"
#include "Configuration.h"
#include "model/Model.h"

Model* model;

void setup() {
  Serial.begin(BOUND_RATE);
  model = new Model();
}

void loop() {
  delay(10);
  model->tick();
}