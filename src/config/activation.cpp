#include <Data.h>
#include "config/activation.h"
#include "config/_addr.h"
#include "config/_constants.h"
#include "logger.h"

void config_activate(config_activation_t data)
{
    LOG_TRACE("config_activate");
    config_network_set(data);
    config_access_set(data);
    Data.write(_ACTIVATED_ADDR, YES);
    Data.save();
    LOG_INFO("Device Activated.")
}

bool config_activated()
{
    return Data.read(_ACTIVATED_ADDR) == YES;
}

void config_deactivate()
{
    LOG_TRACE("config_deactivate");
    Data.write(_ACTIVATED_ADDR, NO);
    Data.save();
}

void config_deactivate_await()
{
    PRINTLN("Press any key to reset device...");
    long startTime = millis();
    while (!Serial.available())
    {
        long currentTime = millis();
        if ((startTime + 2000) <= currentTime)
            return;
    }
    config_deactivate();
}

config_activation_t config_activation_get()
{
    LOG_TRACE("config_get");
    config_activation_t data;

    config_access_t acc_data = config_access_get();
    data.access = acc_data.access;

    config_network_t net_data = config_network_get();
    data.name = net_data.name;
    data.wifi_ssid = net_data.wifi_ssid;
    data.wifi_password = net_data.wifi_password;
    data.dhcp = net_data.dhcp;
    data.ip = net_data.ip;
    data.subnet = net_data.subnet;
    data.gateway = net_data.gateway;
    data.dns = net_data.dns;
    data.dns1 = net_data.dns1;
    data.dns2 = net_data.dns2;

    return data;
}

config_activation_t config_activation_log()
{
    config_activation_t data = config_activation_get();
    PRINTSTATUS("Name", data.name);
    PRINTSTATUS("WiFi", data.wifi_ssid);

    PRINTSTATUS("DHCP", data.dhcp ? "Enabled" : "Disabled");
    if (!data.dhcp)
    {
        PRINTSTATUS("-IP", data.ip.toString() + " netmask " + data.subnet.toString());
        PRINTSTATUS("-GW", data.gateway.toString());
    }
    PRINTSTATUS("DNS", data.dns ? "Custom" : "DHCP");
    if (data.dns)
    {
        PRINTSTATUS("-DNS1", data.dns1.toString());
        PRINTSTATUS("-DNS2", data.dns1.toString());
    }
    return data;
}


