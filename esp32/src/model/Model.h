#ifndef  __MODEL__
#define __MODEL__

#include "../communication/MqttManager.h"
#include "../Configuration.h"
#include <ArduinoJson.h>

class Model
{
    private:
        MqttManager* mqttManager;
        static int inputPin[NUM_INPUT_PIN];
        static int isAnalog[NUM_INPUT_PIN];
        static int indexInputPin;

    public:
        Model();
        ~Model();
        static void changeState(char* topic, byte* payload, unsigned int length);
        void tick();
};

#endif