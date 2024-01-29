#include "MqttManager.h"


MqttManager::MqttManager() : client(espClient) {

}

void MqttManager::establishWifiConnection() {
  randomSeed(micros());
  delay(10);

  Serial.println(String("Connecting to ") + SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

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
void MqttManager::establishMqttConnection(void (*func)(char*, byte*, unsigned int)) {
  IPAddress mqttServer;
  mqttServer.fromString(MQTT_SERVER);
  this->establishWifiConnection();
  client.setServer(mqttServer, PORT);
  client.setCallback(func);
  client.setKeepAlive(2*60*60);
  this->reconnect();
}

//Reconnect to the broker
void MqttManager::reconnect() {

  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Create a random client ID
    String clientId = String("espDevice-")+String(random(0xfffffff), HEX);

    Serial.println(clientId);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(String(TOPIC_INPUT).c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

//Send a message with the Mqtt protocol
void MqttManager::sendJsonMessage(String topic, String jsonMessage) {
  char msg[MSG_BUFFER_SIZE];
  if(!client.connected()) {
    MqttManager::reconnect();
  }

  jsonMessage.toCharArray(msg, MSG_BUFFER_SIZE);
  Serial.println(String("Publishing message: ") + msg);
  client.publish(topic.c_str(), msg);
}

void MqttManager::tick() {
  client.loop();
}