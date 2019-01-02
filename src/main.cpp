#include <Arduino.h>
#include "activation.h"
#include "config.h"
#include "portal.h"
#include <Data.h>
#include <Console.h>

void setup()
{
    config_init();
    Console.init();
    if (!config_activated())
    {
        LOG_INFO("Device not activated. Executing activating sequence.");
        activation_setup();
    }
    else
    {
        LOG_INFO("Device activated. Connecting to network.");
        portal_setup();
    }
}

void loop()
{
    if (!config_activated())
    {
        activation_execute();
    }
    else
    {
        portal_execute();
    }
}