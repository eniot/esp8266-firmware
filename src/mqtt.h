#ifndef app_mqtt_h
#define app_mqtt_h

#include <Arduino.h>

struct mqtt_command_t
{
    String topic_cmd;
    String topic_name;
    String domain_type;
    String domain;
    String params;
    String command;
};

void mqtt_setup();
void mqtt_execute();
bool mqtt_send(String msg);

#endif