#include <Arduino.h>
#include <EEPROM.h>
#include <Data.h>
#include "config.h"
#include <Logger.h>

void config_init()
{
    Data.init(EEPROM_SIZE);
}

void config_reset()
{
    LOG_INFO("Reseting to default settings");
    //Initial execution - Setup default values
    config_network_set(config_network_default());
    config_access_set(config_access_default());
    config_mqtt_set(config_mqtt_default());
    config_io_set(config_io_default());
    Data.write(_ACTIVATED_ADDR, NO);
    Data.save();
}