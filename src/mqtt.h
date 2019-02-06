#ifndef app_mqtt_h
#define app_mqtt_h

#include <WString.h>
#include <PubSubClient.h>

#define MQTT_ACK "ack"
#define MQTT_ACK_INIT "init"

void mqtt_setup();
void mqtt_execute();
bool mqtt_send(String payload, String topicPostFix);
bool mqtt_err(String payload, String topicPostFix);

String mqtt_status();
String mqtt_state_str();

#endif