#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "activation.h"
#include "config.h"
#include "logger.h"
#include "views/activation_index.h"

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer captiveWebServer(80);

void _activation_root_handler();
void _activation_save_handler();

void activation_setup()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("InactiveIOT");

    // if DNSServer is started with "*" for domain name, it will reply with
    // provided IP to all DNS request
    dnsServer.start(DNS_PORT, "*", apIP);

    captiveWebServer.on("/", HTTP_GET, _activation_root_handler);
    captiveWebServer.on("/", HTTP_POST, _activation_save_handler);
    // replay to all requests with same HTML
    captiveWebServer.onNotFound([]() {
        captiveWebServer.sendHeader("Location", "/", true); //Redirect to our html web page
        captiveWebServer.send(302, "text/plane", "");
    });
    captiveWebServer.begin();
}

void activation_execute()
{
    dnsServer.processNextRequest();
    captiveWebServer.handleClient();
}

void _activation_root_handler()
{
    LOG_TRACE("_activation_root_handler");
    captiveWebServer.send(200, "text/html", view_activation_index(config_get()));
}

void _activation_save_handler()
{
    LOG_TRACE("_activation_save_handler");
    activate_data data;
    data.name = captiveWebServer.arg("name");
    data.access = captiveWebServer.arg("access");
    data.wifi_ssid = captiveWebServer.arg("wifi_ssid");
    data.wifi_password = captiveWebServer.arg("wifi_password");
    data.dhcp = captiveWebServer.arg("dhcp") == "1";
    if (!data.dhcp)
    {
        data.ip.fromString(captiveWebServer.arg("ip"));
        data.subnet.fromString(captiveWebServer.arg("subnet"));
        data.gateway.fromString(captiveWebServer.arg("gateway"));
    }
    data.dns = captiveWebServer.arg("dns") == "1";
    if (data.dns)
    {
        data.dns1.fromString(captiveWebServer.arg("dns1"));
        data.dns2.fromString(captiveWebServer.arg("dns2"));
    }
    config_activate(data);
    LOG_INFO("Device Activated. Restarting...")
    ESP.restart();
}