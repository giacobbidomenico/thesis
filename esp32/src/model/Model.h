#ifndef  __MODEL__
#define __MODEL__

#include "../communication/MqttManager.h"
#include "../Configuration.h"
#include <ArduinoJson.h>

class Model
{
    private:
        MqttManager* mqttManager;
    public:
        Model();
        ~Model();
        static void changeState(char* topic, byte* payload, unsigned int length);
        void tick();
};

#endif