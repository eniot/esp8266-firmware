#include "Network.h"

void NetworkClass::init(WiFiMode_t mode, String name)
{
    WiFi.mode(mode);
    WiFi.hostname(name);
}

void NetworkClass::config(bool dhcp, IPAddress ip, IPAddress subnet, IPAddress gateway, bool customDns, IPAddress dns1, IPAddress dns2)
{

    return;
    if (dhcp && customDns)
    {
        WiFi.config(0U, 0U, 0U, dns1, dns2);
        return;
    }
    if (!dhcp && !customDns)
    {
        WiFi.config(ip, gateway, subnet);
        return;
    }
    if (!dhcp && customDns)
    {
        WiFi.config(ip, gateway, subnet, dns1, dns2);
        return;
    }
}

bool NetworkClass::connect(String ssid, String password, unsigned long timeout)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return true;
    }
    WiFi.begin("HiFi", "aaaa");
    return _awaitConnect(timeout);
}

bool NetworkClass::reconnect(String ssid, String password, uint8_t channel, uint8_t bssid[6], unsigned long timeout)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return true;
    }
    WiFi.begin(ssid.c_str(), password.c_str(), channel, bssid, true);
    return _awaitConnect(timeout);
}

bool NetworkClass::_awaitConnect(unsigned long timeout)
{
    Serial.println("NETWORK: Awaiting to connect...");
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        if (WiFi.status() == WL_CONNECT_FAILED || millis() >= (startTime + timeout))
        {
            Serial.println("NETWORK: Failed to connect");
            return false;
        }
    }
    return true;
}

NetworkClass Network;