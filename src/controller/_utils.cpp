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

void _update_mqtt_from_web(config_mqtt_t *data)
{
    data->mqtt = _webserver.arg("mqtt") == "1";
    if (data->mqtt)
    {
        data->server = _webserver.arg("server");
        data->port = _webserver.arg("port").toInt();
        data->topic = _webserver.arg("topic");
        data->username = _webserver.arg("username");
        data->password = _webserver.arg("password");
    }
}

bool _check_auth()
{
    if (!config_setup_complete())
        return true;
    String password = config_access_get().access;
    password.trim();
    if (password == "")
        return true;
    return _webserver.authenticate("admin", password.c_str());
}

void _update_setup_from_web(config_setup_t *data)
{
    _update_access_from_web(data);
    _update_network_from_web(data);
    _update_mqtt_from_web(data);
}

void _update_gpio_from_web(config_gpio_t *data)
{
    data->label = _webserver.arg("label");
    data->func = _webserver.arg("func").toInt();
    data->invert = _webserver.arg("invert") == "yes";
    data->persist = _webserver.arg("persist") == "yes";
    data->toggle = _webserver.arg("toggle") == "yes";
    String pinMap = _webserver.arg("map");
    pinMap.trim();
    data->map = pinMap == "" ? IO_MAP_NONE : pinMap.toInt();
}

ioindex_t _fetch_iopin(String uri)
{
    char cpin[3];
    sscanf(uri.c_str(), "/io/%s", cpin);
    String spin(cpin);
    return spin.toInt();
}