#include "mqtt.h"

bool _mqtt_enabled = false;

void mqtt_setup()
{
    _mqtt_enabled = true;
}

void mqtt_execute()
{
    if (!_mqtt_enabled)
        return;
}
