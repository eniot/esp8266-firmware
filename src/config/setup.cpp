#include <Data.h>
#include "config/setup.h"
#include "config/_addr.h"
#include "config/_constants.h"
#include <Logger.h>

void config_setup(config_setup_t data)
{
    LOG_TRACE("config_setup");
    config_network_set(data);
    config_access_set(data);
    config_mqtt_set(data);
    Data.write(_ACTIVATED_ADDR, YES);
    Data.save();
    LOG_INFO("Device Activated.")
}

bool config_setup_complete()
{
    return Data.read(_ACTIVATED_ADDR) == YES;
}

void config_setup_reset()
{
    LOG_TRACE("config_setup_reset");
    Data.write(_ACTIVATED_ADDR, NO);
    Data.save();
}

config_setup_t config_setup_get()
{
    LOG_TRACE("config_setup_get");
    config_setup_t data;

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

    config_mqtt_t mqtt_data = config_mqtt_get();
    data.mqtt = mqtt_data.mqtt;
    data.server = mqtt_data.server;
    data.port = mqtt_data.port;
    data.topic = mqtt_data.topic;
    data.username = mqtt_data.username;
    data.password = mqtt_data.password;

    return data;
}

config_setup_t config_setup_log()
{
    config_setup_t data = config_setup_t();
    PRINTSTATUS("Name", data.name)
    PRINTSTATUS("WiFi", data.wifi_ssid)

    PRINTSTATUS("DHCP", data.dhcp ? "Enabled" : "Disabled")
    if (!data.dhcp)
    {
        PRINTSTATUS("-IP", data.ip.toString() + " netmask " + data.subnet.toString())
        PRINTSTATUS("-GW", data.gateway.toString())
    }
    PRINTSTATUS("DNS", data.dns ? "Custom" : "DHCP")
    if (data.dns)
    {
        PRINTSTATUS("-DNS1", data.dns1.toString())
        PRINTSTATUS("-DNS2", data.dns1.toString())
    }
    PRINTSTATUS("MQTT", data.mqtt ? "Enabled" : "Disabled");
    if (data.mqtt)
    {
        PRINTSTATUS("-Server", data.server + ":" + String(data.port))
        PRINTSTATUS("-Topic", data.topic)
    }
    return data;
}
