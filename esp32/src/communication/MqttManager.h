#ifndef _MQTT_CLIENT_MANAGER__
#define _MQTT_CLIENT_MANAGER__

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "../Configuration.h"

//Class that manages communications through the MQTT protocol
class MqttManager {
  private:
    String ssid;
    String password;
    WiFiClient espClient;
    PubSubClient client;
    int port;
    String topic;
    String mqttServer;
    char msg[MSG_BUFFER_SIZE];
  public:
    MqttManager(String ssid, String password, String topic, String mqttServer, int port);
    ~MqttManager();
    void establishWifiConnection();
    //Configure the MQTT connection
    void establishMqttConnection();
    //Reconnect to the broker
    void reconnect();
    //Send a message with the Mqtt protocol
    void sendJsonMessage(String jsonMessage);
    void tick();
    static void callback(char* topic, byte* payload, unsigned int length);
};

#endif