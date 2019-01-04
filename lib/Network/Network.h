#ifndef lib_network_h
#define lib_network_h

#include <Arduino.h>
#include <ESP8266WiFi.h>

struct connection_info_t
{
    IPAddress ip;
    IPAddress subnet;
    IPAddress gateway;
    IPAddress dns;
};

class NetworkClass
{
  private:
    bool _awaitConnect(unsigned long timeout);

  public:
    void init(WiFiMode_t mode, String name);
    void config(bool dhcp, IPAddress ip, IPAddress subnet, IPAddress gateway, bool customDns, IPAddress dns1, IPAddress dns2);
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
    connection_info_t info();
};

extern NetworkClass Network;

#endif