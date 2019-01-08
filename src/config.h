#ifndef app_config_h
#define app_config_h

#define NO 1
#define YES 2

#define UNUSED 0xFF

#include <Arduino.h>
#include <IPAddress.h>

struct config_network_t
{
    String name;
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

struct config_access_t
{
    String access;
};

struct config_activation_t : config_network_t, config_access_t
{
};

struct config_mqtt_t
{
    bool enabled;
    String server;
    int16_t port;
    String username;
    String password;
    String topic;
};

struct config_gpio_t
{
    int8_t function;
    String label;
};

struct config_io_t
{
    config_gpio_t gpio[17];
};

void config_init();
bool config_activated();
void config_activate(config_activation_t data);
void config_deactivate();
void config_deactivate_await();
String config_name_get();
config_activation_t config_activation_get();
config_activation_t config_activation_log();
config_network_t config_network_get();
void config_network_set(config_network_t data);
config_access_t config_access_get();
void config_access_set(config_access_t data);
config_mqtt_t config_mqtt_get();
void config_mqtt_set(config_mqtt_t data);
bool config_mqtt_enabled();
config_io_t config_io_get();
void config_io_set(config_io_t data);

#endif