#ifndef __CONFIGURATION__
#define __CONFIGURATION__

#define SSID "esp"
#define PASSWORD "mioEsp1833"

#define DEVICE_ID 2
#define MQTT_SERVER "broker.emqx.io"
#define PORT 1883

#define TOPIC_INPUT "thesisIngSInfUnibo2024Input"
#define TOPIC_OUTPUT "thesisIngSInfUnibo2024Output"

#define MSG_BUFFER_SIZE 50

#define BOUND_RATE 115200

#define ADD_COMMAND_TYPE 0
#define INPUT_TYPE 1
#define OUTPUT_TYPE 2

#endif