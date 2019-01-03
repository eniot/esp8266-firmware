#ifndef lib_network_h
#define lib_network_h

#include <Arduino.h>
#include <ESP8266WiFi.h>

class NetworkClass
{
  private:
    bool _awaitConnect(String message, unsigned long timeout);

  public:
    void init(WiFiMode_t mode, String name);
    void config(IPAddress ip, IPAddress subnet, IPAddress gateway, IPAddress dns1 = (uint32_t)0x00000000, IPAddress dns2 = (uint32_t)0x00000000);
    bool reconnect(String ssid, String password, uint8_t channel, uint8_t bssid[6], unsigned long timeout);
    bool reconnect(String ssid, String password, uint8_t channel, uint8_t bssid[6])
    {
        return reconnect(ssid, password, channel, bssid, 3000);
    }
    bool connect(String ssid, String password, unsigned long timeout);
    bool connect(String ssid, String password)
    {
        return connect(ssid, password, 10000);
    }
};

extern NetworkClass Network;

#endif