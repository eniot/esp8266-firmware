#ifndef app_config_h
#define app_config_h

#define NO 1
#define YES 2

#include <Arduino.h>
#include <IPAddress.h>

struct activate_data
{
    String name;
    String access;
    String wifi_ssid;
    String wifi_password;
    bool dhcp;
    IPAddress ip;
    IPAddress subnet;
    IPAddress gateway;
};

struct config_data : activate_data
{
    bool activated;
};

void config_init();
bool config_activated();
void config_activate(activate_data data);
void config_deactivate();
config_data config_get();

#endif