#include <Data.h>
#include "config/_addr.h"
#include "config/network.h"
#include "config/_constants.h"

config_network_t config_network_get()
{
    config_network_t data;
    data.name = config_name_get();
    data.wifi_ssid = Data.readStr(_WIFI_SSID_ADDR, _WIFI_SSID_SIZE);
    data.wifi_password = Data.readStr(_WIFI_PASSWORD_ADDR, _WIFI_PASSWORD_SIZE);
    data.dhcp = Data.read(_DHCP_ADDR) == YES;
    data.ip.fromString(Data.readStr(_IP_ADDR, _IP_SIZE));
    data.subnet.fromString(Data.readStr(_SUBNET_ADDR, _SUBNET_SIZE));
    data.gateway.fromString(Data.readStr(_GATEWAY_ADDR, _GATEWAY_SIZE));
    data.dns = Data.read(_DNS_ADDR) == YES;
    data.dns1.fromString(Data.readStr(_DNS1_ADDR, _DNS1_SIZE));
    data.dns1.fromString(Data.readStr(_DNS2_ADDR, _DNS2_SIZE));
    return data;
}

void config_network_set(config_network_t data)
{
    if (data.dhcp)    
        data.ip = data.gateway = data.subnet = 0U;
    if(!data.dns)    
        data.dns1 = data.dns2 = 0U;
    
    Data.writeStr(_NAME_ADDR, data.name);
    Data.writeStr(_WIFI_SSID_ADDR, data.wifi_ssid);
    Data.writeStr(_WIFI_PASSWORD_ADDR, data.wifi_password);
    Data.write(_DHCP_ADDR, data.dhcp ? YES : NO);    
    Data.writeStr(_IP_ADDR, data.ip.toString(), true);
    Data.writeStr(_SUBNET_ADDR, data.subnet.toString(), true);
    Data.writeStr(_GATEWAY_ADDR, data.gateway.toString());    
    Data.write(_DNS_ADDR, data.dns ? YES : NO);
    Data.writeStr(_DNS1_ADDR, data.dns1.toString(), true);
    Data.writeStr(_DNS2_ADDR, data.dns2.toString());    
}

void config_network_save(config_network_t data)
{
    config_network_set(data);
    Data.save();
}

String config_name_gen()
{
    String name;
    for (size_t i = 0; i < 4; i++)
    {
        name += char('a' + random(26));
    }
    name += "-iot";
    return name;
}

String config_name_get()
{
    String name = Data.readStr(_NAME_ADDR, _NAME_SIZE);
    name.trim();
    if (name == "")
    {
        name = config_name_gen();
        Data.writeStr(_NAME_ADDR, name);
        Data.save();
    }
    return name;
}

config_network_t config_network_default()
{
    config_network_t ndata;
    ndata.dhcp = true;
    ndata.dns = false;
    ndata.name = config_name_gen();
    ndata.wifi_ssid = "";
    ndata.wifi_password = "";    
    return ndata;
}