#include "MqttManager.h"

MqttManager::MqttManager(String ssid, String password, String topic, String mqttServer, int port) : client(espClient) {
  this->ssid = ssid;
  this->password  = password;
  this->topic = topic;
  this->mqttServer = mqttServer;
  this->port = port;
  randomSeed(micros());
}

MqttManager::~MqttManager() {

}

void MqttManager::establishWifiConnection() {
  delay(10);

  Serial.println(String("Connecting to ") + ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//Configure the MQTT connection
void MqttManager::establishMqttConnection() {
  this->establishWifiConnection();
  client.setServer(mqttServer.c_str(), port);
  client.setCallback(MqttManager::callback);
  client.setKeepAlive(2*60*60);
  reconnect();
}

//Reconnect to the broker
void MqttManager::reconnect() {

  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Create a random client ID
    String clientId = String("espDevice-")+String(random(0xffff), HEX);

    Serial.println(clientId);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(topic.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

//Send a message with the Mqtt protocol
void MqttManager::sendJsonMessage(String jsonMessage) {
  if(!client.connected()) {
    reconnect();
  }

  jsonMessage.toCharArray(msg, MSG_BUFFER_SIZE);
  Serial.println(String("Publishing message: ") + msg);
  client.publish(topic.c_str(), msg);
}

void MqttManager::tick() {
  client.loop();
}

void MqttManager::callback(char* topic, byte* payload, unsigned int length) {
    JsonDocument doc;
    deserializeJson(doc, payload);
    Serial.println(String("Message arrived on [") + topic + "] len: " + length + " value");
    int deviceId = doc["deviceId"];
    int type = doc["type"];
    int pin = doc["pin"];
    int value = doc["value"];

    if(deviceId == DEVICE_ID) {
      if(type == INPUT_TYPE) {
        Serial.println(value ? "HIGH" : "LOW");
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
      }
      if(type == OUTPUT_TYPE) {
        pinMode(pin, INPUT);
        int isAnalog = doc["isAnalog"];
        double value;
        if(isAnalog) {
          value = analogRead(pin);
        } else {
          value = digitalRead(pin);
        }
        //sendJsonMessage(String("{\"value\":}" + value));
      }
    }
}