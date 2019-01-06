#ifndef app_mqtt_h
#define app_mqtt_h

#include <Arduino.h>

void mqtt_setup();
void mqtt_execute();
bool mqtt_send(String msg);

#endif