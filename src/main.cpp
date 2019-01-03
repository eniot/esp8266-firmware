#include <Arduino.h>
#include <Network.h>
#include <Data.h>
#include "activation.h"
#include "config.h"
#include "portal.h"
#include "logger.h"

config_data data;

void setup()
{
    logger_init();
    config_init();

    if (!config_activated())
    {
        LOG_INFO("Device not activated. Executing activating sequence.");
        activation_setup();
    }
    else
    {
        data = config_data();
        PRINTSTATUS("Name", data.name);
        PRINTSTATUS("WiFi", data.wifi_ssid);

        PRINTSTATUS("DHCP", data.dhcp ? "Enabled" : "Disabled");
        if (!data.dhcp)
        {
            PRINTSTATUS("- IP", data.ip.toString() + " netmask " + data.subnet.toString());
            PRINTSTATUS("- Gateway", data.gateway.toString());
        }
        LOG_INFO("Device activated. Connecting to network.");
        Network.init(WIFI_STA, data.name);
        if (!data.dhcp)
        {
            Network.config(data.ip, data.subnet, data.gateway);
        }
        if (!Network.connect(data.wifi_ssid, data.wifi_password))
        {
            LOG_WARN("Cannot connect to network. Deactivating device.")
            config_deactivate();
            ESP.restart();
        }
        LOG_INFO("Connected to Network " + data.wifi_ssid);
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