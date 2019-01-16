#include "controller/_utils.h"
#include "webserver.h"

void _update_network_from_web(config_network_t *data)
{
    data->name = _webserver.arg("name");
    data->wifi_ssid = _webserver.arg("wifi_ssid");
    data->wifi_password = _webserver.arg("wifi_password");
    data->dhcp = _webserver.arg("dhcp") == "1";
    if (!data->dhcp)
    {
        data->ip.fromString(_webserver.arg("ip"));
        data->subnet.fromString(_webserver.arg("subnet"));
        data->gateway.fromString(_webserver.arg("gateway"));
    }
    data->dns = _webserver.arg("dns") == "1";
    if (data->dns)
    {
        data->dns1.fromString(_webserver.arg("dns1"));
        data->dns2.fromString(_webserver.arg("dns2"));
    }
}

void _update_access_from_web(config_access_t *data)
{
    data->access = _webserver.arg("access");
}

bool _check_auth()
{
    String password = config_access_get().access;
    password.trim();
    if(password == "") 
        return true;
    return _webserver.authenticate("admin", password.c_str());
}