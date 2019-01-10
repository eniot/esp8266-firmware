#include <Arduino.h>
#include "config.h"
#include "webserver.h"
#include "logger.h"
#include "mqtt.h"
#include "network.h"

void setup()
{
    logger_init();
    config_init();
    config_deactivate_await();
    network_setup();
    webserver_setup();
    mqtt_setup();
}

void loop()
{
    webserver_execute();
    mqtt_execute();
}