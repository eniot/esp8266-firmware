#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "activation.h"
#include "config.h"

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
    String response = R"=====(

<html lang="en" class="route-index">
<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Setup IOT</title>
</head>
<body>
<form method="POST" action="/">
    <input type="text" name="access" placeholder="Access Password"/>
    <hr/>
    <input type="text" name="wifi_ssid" placeholder="WIFI SSID"/>
    <br/>
    <input type="text" name="wifi_password" placeholder="WIFI Password"/>
    <hr/>
    <input type="radio" name="dhcp" value="1">DHCP Enable<br>
    <input type="radio" name="dhcp" value="0">DHCP Disable<br>
    <hr/>
    <input type="text" name="ip" placeholder="IP Address"/>
    <br/>
    <input type="text" name="subnet" placeholder="Subnet Mask"/>
    <br/>
    <input type="text" name="gateway" placeholder="Gateway"/>
    <hr/>
    <button type="submit">Activate</button>
</form>
<body>
</html>

)=====";
    captiveWebServer.send(200, "text/html", response);
}

void _activation_save_handler()
{
    activate_data data;
    data.access = captiveWebServer.arg("access");
    data.wifi_ssid = captiveWebServer.arg("wifi_ssid");
    data.wifi_password = captiveWebServer.arg("wifi_password");
    data.dhcp = captiveWebServer.arg("dhcp") == "1";
    data.ip.fromString(captiveWebServer.arg("ip"));
    data.subnet.fromString(captiveWebServer.arg("subnet"));
    data.gateway.fromString(captiveWebServer.arg("gateway"));
    config_activate(data);
    ESP.restart();
}