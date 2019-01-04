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

// END EEPROM
#define _END_ADDR (_DNS2_ADDR + _DNS2_SIZE)

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

void config_activate(config_activation_t data)
{
    LOG_TRACE("config_activate");
    Data.write(_NAME_ADDR, data.name);
    Data.write(_ACCESS_PASSWORD_ADDR, data.access);
    Data.write(_WIFI_SSID_ADDR, data.wifi_ssid);
    Data.write(_WIFI_PASSWORD_ADDR, data.wifi_password);
    Data.write(_DHCP_ADDR, data.dhcp ? YES : NO);
    if (!data.dhcp)
    {
        Data.write(_IP_ADDR, data.ip.toString());
        Data.write(_SUBNET_ADDR, data.subnet.toString());
        Data.write(_GATEWAY_ADDR, data.gateway.toString());
    }
    Data.write(_DNS_ADDR, data.dns ? YES : NO);
    if (data.dns)
    {
        Data.write(_DNS1_ADDR, data.dns1.toString());
        Data.write(_DNS2_ADDR, data.dns2.toString());
    }
    Data.write(_ACTIVATED_ADDR, YES);
    Data.save();
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
    data.name = Data.readStr(_NAME_ADDR, _NAME_SIZE);
    data.wifi_ssid = Data.readStr(_WIFI_SSID_ADDR, _WIFI_SSID_SIZE);
    data.wifi_password = Data.readStr(_WIFI_PASSWORD_ADDR, _WIFI_PASSWORD_SIZE);
    data.access = Data.readStr(_ACCESS_PASSWORD_ADDR, _ACCESS_PASSWORD_SIZE);
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