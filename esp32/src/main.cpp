#include <WiFi.h>
#include <PubSubClient.h>
#include "Configuration.h"
#include "communication/MqttClientManager.h"

/*
const char *ssid = "esp";
const char *password = "mioEsp1234";

const char *mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char *mqtt_user = "";
const char *mqtt_password = "";



WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.println("Messaggio ricevuto su topic: " + String(topic));

  // Costruisci una stringa dal payload del messaggio
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.println("Contenuto del messaggio: " + message);
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Riconnessione al broker MQTT...");
    
    String clientId = String("sensorBoardId-")+String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("Connesso al broker MQTT");
      client.subscribe("thesisUnibo");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


void setup() {
  Serial.begin(BOUND_RATE);

/*
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connessione al WiFi...");
  }
  Serial.println("Connesso al WiFi");

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  client.setKeepAlive(2*60*60);
}



void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}*/

MqttClientManager* mqttManager;


void setup() {
  Serial.begin(BOUND_RATE);
  mqttManager = new MqttClientManager(SSID, PASSWORD, TOPIC, MQTT_SERVER, PORT);
  mqttManager->establishMqttConnection();
}

void loop() {
  mqttManager->tick();
}