#include "MqttClientManager.h"

MqttClientManager::MqttClientManager(String ssid, String password, String topic, String mqttServer, int port) : client(espClient) {
  this->ssid = ssid;
  this->password  = password;
  this->topic = topic;
  this->mqttServer = mqttServer;
  this->port = port;
  randomSeed(micros());
}

MqttClientManager::~MqttClientManager() {

}

//Configure the MQTT connection
void MqttClientManager::initConnection() {
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

  client.setServer(mqttServer.c_str(), port);
  client.setCallback(MqttClientManager::callback);
  client.setKeepAlive(2*60*60);
}

//Reconnect to the broker
void MqttClientManager::reconnect() {

  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Create a random client ID
    String clientId = String("sensorBoardId-")+String(random(0xffff), HEX);

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
void MqttClientManager::sendJsonMessage(String jsonMessage) {

  if(!client.connected()) {
    reconnect();
  }

  client.loop();


  jsonMessage.toCharArray(msg, MSG_BUFFER_SIZE);
  Serial.println(String("Publishing message: ") + msg);
  client.publish(topic.c_str(), msg);
}