#ifndef app_config_h
#define app_config_h

#define NO 1
#define YES 2

#include <Arduino.h>
#include <IPAddress.h>

struct config_network_t
{
    String wifi_ssid;
    String wifi_password;
    bool dhcp;
    IPAddress ip;
    IPAddress subnet;
    IPAddress gateway;
    bool dns;
    IPAddress dns1;
    IPAddress dns2;
};

struct config_details_t
{
    String name;
};

struct config_access_t
{
    String access;
};

struct config_activation_t : config_network_t, config_details_t, config_access_t
{
};

void config_init();
bool config_activated();
void config_activate(config_activation_t data);
void config_deactivate();
void config_deactivate_await();
config_activation_t config_activation_get();
config_activation_t config_activation_log();

#endif