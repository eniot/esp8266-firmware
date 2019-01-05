#include <Arduino.h>
#include <Network.h>
#include <Data.h>
#include "activation.h"
#include "config.h"
#include "portal.h"
#include "logger.h"
#include "mqtt.h"

void setup()
{
    logger_init();
    config_init();

    config_deactivate_await();

    if (!config_activated())
    {
        LOG_INFO("Executing activating sequence.");
        activation_setup();
        return;
    }

    config_activation_t cfg_act = config_activation_log();
    Network.init(WIFI_STA, cfg_act.name);
    Network.config(cfg_act.dhcp, cfg_act.ip, cfg_act.subnet, cfg_act.gateway, cfg_act.dns, cfg_act.dns1, cfg_act.dns2);
    LOG_INFO("Connecting to network...");
    if (!Network.connect(cfg_act.wifi_ssid, cfg_act.wifi_password))
    {
        LOG_WARN("Connection failed!");
        LOG_INFO("Restarting...");
        ESP.restart();
    }
    LOG_INFO("Connected to Network");
    connection_info_t cinfo = Network.info();
    PRINTSTATUS("IP", cinfo.ip.toString() + " netmask " + cinfo.subnet.toString());
    PRINTSTATUS("GW", cinfo.gateway.toString());
    PRINTSTATUS("DNS", cinfo.dns.toString());
    portal_setup();
    if (config_mqtt_enabled())
        mqtt_setup();
}

void loop()
{
    activation_execute();
    portal_execute();
    mqtt_execute();
}