#ifndef _MQTT_CLIENT_MANAGER__
#define _MQTT_CLIENT_MANAGER__

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <IPAddress.h>
#include "../Configuration.h"

//Class that manages communications through the MQTT protocol
class MqttManager {
  private:
    WiFiClient espClient;
    PubSubClient client;
  public:
    MqttManager();
    void establishWifiConnection();
    //Configure the MQTT connection
    void establishMqttConnection(void (*func)(char*, byte*, unsigned int));
    //Reconnect to the broker
    void reconnect();
    //Send a message with the Mqtt protocol
    void sendJsonMessage(String topic, String jsonMessage);
    void tick();
};

#endif