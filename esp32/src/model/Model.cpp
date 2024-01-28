#include "Model.h"

Pin Model::inputPin[NUM_INPUT_PIN];
int Model::indexInputPin = 0;
int Model::changed = 0;

Model::Model() {
    this->mqttManager = new MqttManager();
    this->mqttManager->establishMqttConnection(&changeState);
}


Model::~Model() {
    free(this->mqttManager);
}

int Model::isInputPin(int pin) {
  for(int i = 0; i < Model::indexInputPin; i++) {
    if(Model::inputPin[i].pin == pin) {
      return TRUE;
    }
  }
  return FALSE;
}

void Model::changeState(char* topic, byte* payload, unsigned int length) {
    JsonDocument doc;
    deserializeJson(doc, payload);
    Serial.println(String("Message arrived on [") + topic + "] len: " + length + " value");

    int deviceId = doc["deviceId"];
    int type = doc["type"];
    int pin = doc["pin"];
    int value = doc["value"];

    Serial.println(type);
    Serial.println(pin);
    Serial.println(value);

    if(deviceId == DEVICE_ID) {
      switch (type)
      {
        case PINMODE_TYPE:
          Serial.println("pin mode");
          if(value == 0) {
            pinMode(pin, OUTPUT);
          } else {
            if(!Model::isInputPin(pin)) {
              pinMode(pin, INPUT);
              Model::changed = 1;
              Model::inputPin[Model::indexInputPin].pin = pin;
              Model::inputPin[Model::indexInputPin].digitalValue = 0;
              Model::inputPin[Model::indexInputPin].analogValue = 0;
              Model::indexInputPin++;
            }
          }
          break;
        case OUTPUT_TYPE:
          Serial.println("Output");
          digitalWrite(pin, value);
          break;
        default:
          break;
      }
    }
}

void Model::tick() {
    JsonDocument doc;
    char msg[MSG_BUFFER_SIZE];
    for(int i=0; i < Model::indexInputPin; i++) {
      int digitalValue = digitalRead(Model::inputPin[i].pin);
      
      if(Model::inputPin[i].digitalValue != digitalValue) {
        Model::inputPin[i].digitalValue = digitalValue;
        Model::changed = 1;
        Serial.println("changed digital");
      }

      doc["digitalValue"] = Model::inputPin[i].digitalValue;
      
      if(Model::changed != 0) {
        serializeJson(doc, msg);
        this->mqttManager->sendJsonMessage(msg);
        Model::changed = 0;
      }
    }

    this->mqttManager->tick();
}