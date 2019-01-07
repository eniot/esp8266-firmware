#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "activation.h"
#include "config.h"
#include "logger.h"
#include "portal.h"
#include "views/activation_index.h"

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer captiveWebServer(80);

void _activation_root_handler();
void _activation_save_handler();

bool _activated = true;

void activation_setup()
{
    LOG_INFO("Executing activating sequence.");
    _activated = false;
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    String ssid = "(inactive) " + config_name_get();
    WiFi.softAP(ssid.c_str());

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
    if (_activated)
        return;

    dnsServer.processNextRequest();
    captiveWebServer.handleClient();
}

void _activation_root_handler()
{
    LOG_TRACE("_activation_root_handler");
    captiveWebServer.send(200, "text/html", view_activation_index(config_activation_get()));
}

void _activation_save_handler()
{
    LOG_TRACE("_activation_save_handler");
    config_activation_t data;
    update_access_from_web(&captiveWebServer, &data);
    update_network_from_web(&captiveWebServer, &data);
    config_activate(data);
    LOG_INFO("Device Activated. Restarting...")
    ESP.restart();
}