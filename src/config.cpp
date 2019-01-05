#include <Arduino.h>
#include <EEPROM.h>
#include <Data.h>
#include "config.h"
#include "logger.h"

// Activated
#define _ACTIVATED_ADDR 0
#define _ACTIVATED_SIZE 1

// WIFI SSID
#define _WIFI_SSID_ADDR (_ACTIVATED_ADDR + _ACTIVATED_SIZE)
#define _WIFI_SSID_SIZE 32

// WIFI PASSWORD
#define _WIFI_PASSWORD_ADDR (_WIFI_SSID_ADDR + _WIFI_SSID_SIZE)
#define _WIFI_PASSWORD_SIZE 32

// DHCP
#define _DHCP_ADDR (_WIFI_PASSWORD_ADDR + _WIFI_PASSWORD_SIZE)
#define _DHCP_SIZE 1

// IP
#define _IP_ADDR (_DHCP_ADDR + _DHCP_SIZE)
#define _IP_SIZE 16

// SUBNET
#define _SUBNET_ADDR (_IP_ADDR + _IP_SIZE)
#define _SUBNET_SIZE 16

// GATEWAY
#define _GATEWAY_ADDR (_SUBNET_ADDR + _SUBNET_SIZE)
#define _GATEWAY_SIZE 16

// ACCESS PASSWORD
#define _ACCESS_PASSWORD_ADDR (_GATEWAY_ADDR + _GATEWAY_SIZE)
#define _ACCESS_PASSWORD_SIZE 32

// Device name
#define _NAME_ADDR (_ACCESS_PASSWORD_ADDR + _ACCESS_PASSWORD_SIZE)
#define _NAME_SIZE 32

// DNS
#define _DNS_ADDR (_NAME_ADDR + _NAME_SIZE)
#define _DNS_SIZE 1

// DNS1
#define _DNS1_ADDR (_DNS_ADDR + _DNS_SIZE)
#define _DNS1_SIZE 16

// DNS2
#define _DNS2_ADDR (_DNS1_ADDR + _DNS1_SIZE)
#define _DNS2_SIZE 16

// MQTT_ENABLED
#define _MQTT_ENABLED_ADDR (_DNS2_ADDR + _DNS2_SIZE)
#define _MQTT_ENABLED_SIZE 1

// MQTT_SERVER
#define _MQTT_SERVER_ADDR (_MQTT_ENABLED_ADDR + _MQTT_ENABLED_SIZE)
#define _MQTT_SERVER_SIZE 64

// MQTT_PORT
#define _MQTT_PORT_ADDR (_MQTT_SERVER_ADDR + _MQTT_SERVER_SIZE)
#define _MQTT_PORT_SIZE 2

// MQTT_USERNAME
#define _MQTT_USERNAME_ADDR (_MQTT_PORT_ADDR + _MQTT_PORT_SIZE)
#define _MQTT_USERNAME_SIZE 32

// MQTT_PASSWORD
#define _MQTT_PASSWORD_ADDR (_MQTT_USERNAME_ADDR + _MQTT_USERNAME_SIZE)
#define _MQTT_PASSWORD_SIZE 32

// MQTT_TOPIC
#define _MQTT_TOPIC_ADDR (_MQTT_PASSWORD_ADDR + _MQTT_PASSWORD_SIZE)
#define _MQTT_TOPIC_SIZE 32

// END EEPROM
#define _END_ADDR (_MQTT_TOPIC_ADDR + _MQTT_TOPIC_SIZE)

// Public constants
#define EEPROM_SIZE (_END_ADDR + 1)

void config_init()
{
    Data.init(EEPROM_SIZE);
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
String config_name_get()
{
    String name = Data.readStr(_NAME_ADDR, _NAME_SIZE);
    name.trim();
    if (name == "")
    {
        for (size_t i = 0; i < 4; i++)
        {
            name += char('a' + random(26));
        }
        name += "-iot";
        Data.writeStr(_NAME_ADDR, name);
        Data.save();
    }
    return name;
}
config_network_t config_network_get()
{
    config_network_t data;
    data.name = config_name_get();
    data.wifi_ssid = Data.readStr(_WIFI_SSID_ADDR, _WIFI_SSID_SIZE);
    data.wifi_password = Data.readStr(_WIFI_PASSWORD_ADDR, _WIFI_PASSWORD_SIZE);
    data.dhcp = Data.read(_DHCP_ADDR) == YES;
    if (!data.dhcp)
    {
        data.ip.fromString(Data.readStr(_IP_ADDR, _IP_SIZE));
        data.subnet.fromString(Data.readStr(_SUBNET_ADDR, _SUBNET_SIZE));
        data.gateway.fromString(Data.readStr(_GATEWAY_ADDR, _GATEWAY_SIZE));
    }
    data.dns = Data.read(_DNS_ADDR) == YES;
    if (data.dns)
    {
        data.dns1.fromString(Data.readStr(_DNS1_ADDR, _DNS1_SIZE));
        data.dns1.fromString(Data.readStr(_DNS2_ADDR, _DNS2_SIZE));
    }
    return data;
}

void _config_network_set(config_network_t data)
{
    Data.writeStr(_NAME_ADDR, data.name);
    Data.writeStr(_WIFI_SSID_ADDR, data.wifi_ssid, true);
    Data.writeStr(_WIFI_PASSWORD_ADDR, data.wifi_password);
    Data.write(_DHCP_ADDR, data.dhcp ? YES : NO);
    if (!data.dhcp)
    {
        Data.writeStr(_IP_ADDR, data.ip.toString(), true);
        Data.writeStr(_SUBNET_ADDR, data.subnet.toString(), true);
        Data.writeStr(_GATEWAY_ADDR, data.gateway.toString());
    }
    Data.write(_DNS_ADDR, data.dns ? YES : NO);
    if (data.dns)
    {
        Data.writeStr(_DNS1_ADDR, data.dns1.toString(), true);
        Data.writeStr(_DNS2_ADDR, data.dns2.toString());
    }
}

void config_network_set(config_network_t data)
{
    _config_network_set(data);
    Data.save();
}

config_access_t config_access_get()
{
    config_access_t data;
    data.access = Data.readStr(_ACCESS_PASSWORD_ADDR, _ACCESS_PASSWORD_SIZE);
    return data;
}

void _config_access_set(config_access_t data)
{
    Data.writeStr(_ACCESS_PASSWORD_ADDR, data.access, true);
}

void config_access_set(config_access_t data)
{
    _config_access_set(data);
    Data.save();
}

String _config_mqtt_topic_get()
{
    String topic = Data.readStr(_MQTT_TOPIC_ADDR, _MQTT_TOPIC_SIZE);
    topic.trim();
    if (topic == "")
    {
        topic = config_name_get();
    }
    Data.writeStr(_MQTT_TOPIC_ADDR, topic);
    Data.save();
    return topic;
}

config_mqtt_t config_mqtt_get()
{
    config_mqtt_t data;
    data.enabled = Data.read(_MQTT_ENABLED_ADDR);
    data.server = Data.readStr(_MQTT_SERVER_ADDR, _MQTT_SERVER_SIZE);
    data.port = Data.read16(_MQTT_PORT_ADDR);
    data.username = Data.readStr(_MQTT_USERNAME_ADDR, _MQTT_USERNAME_SIZE);
    data.password = Data.readStr(_MQTT_PASSWORD_ADDR, _MQTT_PASSWORD_SIZE);
    data.topic = _config_mqtt_topic_get();
    return data;
}

void config_mqtt_set(config_mqtt_t data)
{
    Data.write(_MQTT_ENABLED_ADDR, data.enabled);
    if (data.enabled)
    {
        Data.writeStr(_MQTT_SERVER_ADDR, data.server, true);
        Data.write16(_MQTT_PORT_ADDR, data.port);
        Data.writeStr(_MQTT_USERNAME_ADDR, data.username);
        Data.writeStr(_MQTT_PASSWORD_ADDR, data.password);
        Data.writeStr(_MQTT_TOPIC_ADDR, data.topic, true);
    }
    Data.save();
}

void config_activate(config_activation_t data)
{
    LOG_TRACE("config_activate");
    _config_network_set(data);
    _config_access_set(data);
    Data.write(_ACTIVATED_ADDR, YES);
    Data.save();
}