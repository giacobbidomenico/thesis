#include "Model.h"

Pin Model::inputPin[NUM_INPUT_PIN];
int Model::indexInputPin = 0;
int Model::changed = FALSE;

Model::Model() {
    this->mqttManager = new MqttManager();
    this->mqttManager->establishMqttConnection(&changeState);
    String commandTopic = String(COMMAND_TOPIC);
    commandTopic.concat(DEVICE_ID);
    this->mqttManager->subscribeTopic(commandTopic);
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

    int type = doc["type"];
    int pin = doc["pin"];
    int value = doc["value"];
    boolean isAnalog = doc["isAnalog"];

    switch (type)
    {
      case PINMODE_TYPE:
        if(value == 0) {
          pinMode(pin, OUTPUT);
        } else {
          if(!Model::isInputPin(pin)) {
            pinMode(pin, INPUT);
            Model::changed = TRUE;
            Model::inputPin[Model::indexInputPin].pin = pin;
            Model::inputPin[Model::indexInputPin].value = 0;
            Model::inputPin[Model::indexInputPin].isAnalog = isAnalog;
            Model::indexInputPin++;
          }
        }
        break;
      case OUTPUT_TYPE:
        digitalWrite(pin, value);
        break;
      default:
        break;
    }
}

void Model::tick() {


    JsonDocument doc;
    char msg[MSG_BUFFER_SIZE];

    for(int i=0; i < Model::indexInputPin; i++) {
      int value;
      if(Model::inputPin[i].isAnalog) {
        value = analogRead(Model::inputPin[i].pin);
      } else {
        value = digitalRead(Model::inputPin[i].pin);
      }

      if(Model::inputPin[i].value != value ) {
        Model::inputPin[i].value = value;
        Model::changed = TRUE;
      }

      if(Model::changed != FALSE) {
        doc[i]["pin"] = Model::inputPin[i].pin;
        doc[i]["readValue"] = Model::inputPin[i].value;
      }
    }

    if(Model::changed) {
      serializeJson(doc, msg);
      String topic = String(MESSAGE_TOPIC);
      topic.concat(DEVICE_ID);
      this->mqttManager->sendJsonMessage(topic, msg);
    }

    Model::changed = FALSE;
    
    this->mqttManager->tick();
}