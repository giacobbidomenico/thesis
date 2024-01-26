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
  IPAddress mqttServer;
  mqttServer.fromString(MQTT_SERVER);
  MqttManager::establishWifiConnection();
  client.setServer(mqttServer, PORT);
  client.setCallback(MqttManager::receiveJson);
  client.setKeepAlive(2*60*60);
  MqttManager::reconnect();
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
void MqttManager::sendJsonMessage(String jsonMessage) {
  char msg[MSG_BUFFER_SIZE];
  if(!client.connected()) {
    MqttManager::reconnect();
  }

  jsonMessage.toCharArray(msg, MSG_BUFFER_SIZE);
  Serial.println(String("Publishing message: ") + msg);
  client.publish(String(TOPIC_OUTPUT).c_str(), msg);
}

void MqttManager::tick() {
  client.loop();
}

void MqttManager::receiveJson(char* topic, byte* payload, unsigned int length) {
    JsonDocument doc;
    deserializeJson(doc, payload);
    Serial.println(String("Message arrived on [") + topic + "] len: " + length + " value");

    int deviceId = doc["deviceId"];
    int type = doc["type"];
    int pin = doc["pin"];
    int value = doc["value"];

    Serial.println(type);
    Serial.println(pin);
    Serial.println(value);
    if(deviceId == DEVICE_ID) {
      switch (type)
      {
        case PINMODE_TYPE:
          Serial.println("pin mode");
          if(value == 0) {
            pinMode(pin, OUTPUT);
          } else {
            pinMode(pin, INPUT);
          }
          break;
        case OUTPUT_TYPE:
          Serial.println("Output");
          digitalWrite(pin, value);
          break;
        case INPUT_TYPE:
          Serial.println();
          break;
        default:
          break;
      }
    }

    /*
    int deviceId = doc["deviceId"];
    int input = doc["input"];
    int pin = doc["pin"];
    int value = doc["value"];

    if(deviceId == DEVICE_ID) {
      Serial.println(input);
      switch (input)
      {
        case INPUT_TYPE:
          Serial.println("Output");
          Serial.println(value ? "HIGH" : "LOW");
          pinMode(pin, OUTPUT);
          digitalWrite(pin, value);
          break;
        case OUTPUT_TYPE:
          Serial.println("Input");
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
      */
}