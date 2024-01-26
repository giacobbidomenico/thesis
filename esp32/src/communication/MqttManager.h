#ifndef _MQTT_CLIENT_MANAGER__
#define _MQTT_CLIENT_MANAGER__

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <IPAddress.h>
#include "../Configuration.h"

//Class that manages communications through the MQTT protocol
class MqttManager {
  private:
    static WiFiClient espClient;
    static PubSubClient client;
  public:
    static void establishWifiConnection();
    //Configure the MQTT connection
    static void establishMqttConnection();
    //Reconnect to the broker
    static void reconnect();
    //Send a message with the Mqtt protocol
    static void sendJsonMessage(String jsonMessage);
    static void tick();
    static void receiveJson(char* topic, byte* payload, unsigned int length);
};

#endif