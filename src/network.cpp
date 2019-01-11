#include "network.h"
#include "config.h"
#include "logger.h"
#include <ESP8266WiFi.h>

void _network_sta()
{
    config_activation_t cfg_act = config_activation_log();

    if (WiFi.status() == WL_CONNECTED)
        WiFi.disconnect();

    WiFi.mode(WIFI_STA);
    WiFi.hostname(cfg_act.name);
    WiFi.config(cfg_act.ip, cfg_act.gateway, cfg_act.subnet, cfg_act.dns1, cfg_act.dns2);

    LOG_INFO("Connecting to network...");
    WiFi.begin(cfg_act.wifi_ssid.c_str(), cfg_act.wifi_password.c_str());
    unsigned long currTime = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        if (WiFi.status() == WL_CONNECT_FAILED || currTime >= 10000)
        {
            LOG_WARN("Connection failed!");
            LOG_INFO("Restarting...");
            ESP.restart();
        }
        delay(500);
        currTime += 500;
    }
    LOG_INFO("Connected to Network");
    PRINTSTATUS("IP", WiFi.localIP().toString() + " netmask " + WiFi.subnetMask().toString());
    PRINTSTATUS("GW", WiFi.gatewayIP().toString());
    PRINTSTATUS("DNS", WiFi.dnsIP().toString());
}

void _network_ap()
{
    LOG_INFO("Listen on activation AP...")
    String name = config_name_get();
    IPAddress apIP(192, 168, 1, 1);
    String ssid = "(inactive) " + name;
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(ssid.c_str());
    PRINTSTATUS("SSID", ssid)
    PRINTSTATUS("Web", "http://192.168.1.1/")
}

void network_setup()
{
    return config_activated() ? _network_sta() : _network_ap();
}

void network_execute()
{
    if(WiFi.getMode() == WIFI_STA && WiFi.status() != WL_CONNECTED)
        network_setup();
}
