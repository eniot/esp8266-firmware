#include <Arduino.h>
#include <EEPROM.h>
#include <Data.h>
#include "config.h"
#include "logger.h"

void config_init()
{
    Data.init(EEPROM_SIZE);
    uint8_t _actraw = Data.read(_ACTIVATED_ADDR);
    if(_actraw != YES && _actraw != NO) 
    {
        //Initial execution - Setup default values
        config_network_set(config_network_default());
        config_access_set(config_access_default());
        config_mqtt_set(config_mqtt_default());
        config_io_set(config_io_default());
        Data.write(_ACTIVATED_ADDR, NO);
        Data.save();
    }
}