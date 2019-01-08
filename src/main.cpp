#include <Arduino.h>
#include "activation.h"
#include "config.h"
#include "portal.h"
#include "logger.h"
#include "mqtt.h"
#include "net.h"

void setup()
{
    logger_init();
    config_init();

    config_deactivate_await();

    if (!config_activated())
    {
        activation_setup();
        return;
    }
    network_setup();
    portal_setup();
    if (config_mqtt_enabled())
        mqtt_setup();
}

void loop()
{
    activation_execute();
    portal_execute();
    mqtt_execute();
}