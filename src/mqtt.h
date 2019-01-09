#ifndef app_mqtt_h
#define app_mqtt_h

#include <WString.h>

#define MQTT_ACK "ACK"

void mqtt_setup();
void mqtt_execute();
bool mqtt_send(String payload, String domain = "");

#endif