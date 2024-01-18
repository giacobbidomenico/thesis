#include "MqttManager.h"

WiFiClient MqttManager::espClient;
PubSubClient MqttManager::client(MqttManager::espClient);

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
void MqttManager::establishMqttConnection() {
  MqttManager::establishWifiConnection();
  client.setServer(String(MQTT_SERVER).c_str(), PORT);
  client.setCallback(MqttManager::callback);
  client.setKeepAlive(2*60*60);
  MqttManager::reconnect();
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
      client.subscribe(String(TOPIC_OUTPUT).c_str());
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
  char msg[MSG_BUFFER_SIZE];
  if(!client.connected()) {
    MqttManager::reconnect();
  }

  jsonMessage.toCharArray(msg, MSG_BUFFER_SIZE);
  Serial.println(String("Publishing message: ") + msg);
  client.publish(String(TOPIC_INPUT).c_str(), msg);
}

void MqttManager::tick() {
  client.loop();
}

void MqttManager::callback(char* topic, byte* payload, unsigned int length) {
    JsonDocument doc;
    deserializeJson(doc, payload);
    Serial.println(String("Message arrived on [") + topic + "] len: " + length + " value");
    int deviceId = doc["deviceId"];
    int input = doc["input"];
    int pin = doc["pin"];
    int value = doc["value"];

    if(deviceId == DEVICE_ID) {
      Serial.println(input);
      switch (input)
      {
        case INPUT_TYPE:
          Serial.println(value ? "HIGH" : "LOW");
          pinMode(pin, OUTPUT);
          digitalWrite(pin, value);
          break;
        case OUTPUT_TYPE:
          Serial.println("Output");
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
    }
}