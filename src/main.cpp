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

    //config_deactivate();
    if (!config_activated())
    {
        LOG_INFO("Device not activated. Executing activating sequence.");
        activation_setup();
    }
    else
    {
        data = config_get();
        PRINTSTATUS("Name", data.name);
        PRINTSTATUS("WiFi", data.wifi_ssid);

        PRINTSTATUS("DHCP", data.dhcp ? "Enabled" : "Disabled");
        if (!data.dhcp)
        {
            PRINTSTATUS("- IP", data.ip.toString() + " netmask " + data.subnet.toString());
            PRINTSTATUS("- GW", data.gateway.toString());
        }
        PRINTSTATUS("DNS", data.dns ? "Custom" : "DHCP");
        if (data.dns)
        {
            PRINTSTATUS("- DNS1", data.dns1.toString());
            PRINTSTATUS("- DNS2", data.dns1.toString());
        }

        LOG_INFO("Device activated. Connecting to network.");
        Network.init(WIFI_STA, data.name);
        Network.config(data.dhcp, data.ip, data.subnet, data.gateway, data.dns, data.dns1, data.dns2);

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