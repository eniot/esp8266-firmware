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
    void config(IPAddress ip, IPAddress subnet, IPAddress gateway);
    void config(bool dhcp, IPAddress ip, IPAddress subnet, IPAddress gateway, bool customDns, IPAddress dns1, IPAddress dns2);    
    bool reconnect(String ssid, String password, uint8_t channel, uint8_t bssid[6], unsigned long timeout = 3000);
    bool connect(String ssid, String password = "", unsigned long timeout = 10000);
    bool disconnect(bool wifioff = false);
    connection_info_t info();
};

extern NetworkClass Network;

#endif