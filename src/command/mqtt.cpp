#define _MQTT_ACK "hi"
#include "command/mqtt.h"
#include "../mqtt.h"

void command_mqtt_ack()
{
    mqtt_send(_MQTT_ACK);
}