#include "Model.h"

Pin Model::inputPin[NUM_INPUT_PIN];
int Model::indexInputPin = 0;

Model::Model() {
    this->mqttManager = new MqttManager();
    this->mqttManager->establishMqttConnection(&changeState);
}


Model::~Model() {
    free(this->mqttManager);
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
            pinMode(pin, INPUT);
            Model::inputPin[Model::indexInputPin].pin = pin;
            Model::inputPin[Model::indexInputPin].digitalValue = 0;
            Model::inputPin[Model::indexInputPin].analogValue = 0;
            Model::indexInputPin++;
          }
          break;
        case OUTPUT_TYPE:
          Serial.println("Output");
          digitalWrite(pin, value);
          break;
        case INPUT_TYPE:
          break;
        default:
          break;
      }
    }

    /*
    int deviceId = doc["deviceId"];
    int input = doc["input"];
    int pin = doc["pin"];
    int value = doc["value"];

    if(deviceId == DEVICE_ID) {
      Serial.println(input);
      switch (input)
      {
        case INPUT_TYPE:
          Serial.println("Output");
          Serial.println(value ? "HIGH" : "LOW");
          pinMode(pin, OUTPUT);
          digitalWrite(pin, value);
          break;
        case OUTPUT_TYPE:
          Serial.println("Input");
          pinMode(pin, INPUT);
          int isAnalog = doc["isAnalog"];
          double value;
          if(isAnalog) {
            value = analogRead(pin);
          } else {
            value = digitalRead(pin);
          }
          MqttManager::sendJsonMessage(String("{\"value\":" + String(value) + "}"));
          break;
      }
      */
}

void Model::tick() {
    JsonDocument doc;
    char msg[MSG_BUFFER_SIZE];
    int changed = 0;
    for(int i=0; i < Model::indexInputPin; i++) {
      int digitalValue = digitalRead(Model::inputPin[i].pin);
      
      if(Model::inputPin[i].digitalValue != digitalValue) {
        Model::inputPin[i].digitalValue = digitalValue;
        changed = 1;
        Serial.println("changed digital");
      }
      /*if(Model::inputPin[i].analogValue != analogValue) {
        Model::inputPin[i].analogValue = analogValue;
        changed = 1;
      }*/
      doc[i]["pin"] = Model::inputPin[i].pin;
      doc[i]["digitalValue"] = Model::inputPin[i].digitalValue;
      //doc[i]["analogValue"] = Model::inputPin[i].analogValue;
    }
    if(changed != 0) {
      serializeJson(doc, msg);
      this->mqttManager->sendJsonMessage(msg);
      changed = 0;
    }
    this->mqttManager->tick();
}