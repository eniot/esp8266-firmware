#include "Network.h"

void NetworkClass::init(WiFiMode_t mode, String name)
{
    WiFi.mode(mode);
    WiFi.hostname(name);
}

void NetworkClass::config(IPAddress ip, IPAddress subnet, IPAddress gateway, IPAddress dns1, IPAddress dns2)
{
    WiFi.config(ip, gateway, subnet, dns1, dns2);
}

bool NetworkClass::connect(String ssid, String password, unsigned long timeout)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return true;
    }
    WiFi.begin(ssid.c_str(), password.c_str());
    return _awaitConnect(" Connecting to " + ssid, timeout);
}

bool NetworkClass::reconnect(String ssid, String password, uint8_t channel, uint8_t bssid[6], unsigned long timeout)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return true;
    }
    WiFi.begin(ssid.c_str(), password.c_str(), channel, bssid, true);
    return _awaitConnect(" ReConnecting to " + ssid, timeout);
}

bool NetworkClass::_awaitConnect(String message, unsigned long timeout)
{
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        if (WiFi.status() == WL_CONNECT_FAILED || millis() >= (startTime + timeout))
        {
            return false;
        }
    }
    return true;
}

NetworkClass Network;