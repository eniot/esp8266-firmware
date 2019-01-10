#ifndef app_mqtt_h
#define app_mqtt_h

#include <WString.h>

#define MQTT_ACK "ack"
#define MQTT_ACK_INIT "init"

void mqtt_setup();
void mqtt_execute();

#endif