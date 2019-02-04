#ifndef app_controller_portal_h
#define app_controller_portal_h

#include <ESP8266WebServer.h>
#include "network.h"
#include "views/portal_index.h"
#include "views/portal_io.h"
#include "controller/setup.h"
#include "controller/_utils.h"
#include "webserver.h"
#include "mqtt.h"
#include "io.h"

void portal_controller()
{
    _webserver.on("/", HTTP_GET, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        _webserver.send(200, "text/html", view_portal_index());
    });
    _webserver.on("/", HTTP_POST, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        ioindex_t pin = _webserver.arg("io_pin").toInt();
        io_toggle(pin);
        _webserver.send(200, "text/html", view_portal_index());
    });
    _webserver.on("/setup", HTTP_GET, setup_get_handler);
    _webserver.on("/setup", HTTP_POST, setup_post_handler);
    _webserver.on("/network", HTTP_POST, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        config_network_t data;
        _update_network_from_web(&data);
        config_network_save(data);
        _webserver.send(200, "application/json", "true");
        network_setup();
    });
    _webserver.on("/access", HTTP_GET, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        _webserver.send(200, "application/json", "true");
    });
    _webserver.on("/access", HTTP_POST, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        config_access_t data;
        _update_access_from_web(&data);
        config_access_save(data);
        _webserver.send(200, "application/json", "true");
    });
    _webserver.on("/mqtt", HTTP_POST, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        config_mqtt_t data;
        _update_mqtt_from_web(&data);
        config_mqtt_save(data);
        _webserver.send(200, "application/json", "true");
        mqtt_setup();
    });
    _webserver.on("/io", HTTP_GET, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        _webserver.send(200, "text/html", view_portal_io(config_io_get()));
    });
    _webserver.on("/io", HTTP_POST, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        config_io_t data;
        for (size_t i = 0; i < _IO_COUNT; i++)
        {
            data.gpio[i].label = _webserver.arg(String(i) + "_label");
            data.gpio[i].func = _webserver.arg(String(i) + "_func").toInt();
            data.gpio[i].invert = _webserver.arg(String(i) + "_invert") == "yes";
            data.gpio[i].persist = _webserver.arg(String(i) + "_persist") == "yes";
            data.gpio[i].toggle = _webserver.arg(String(i) + "_toggle") == "yes";
            String readpinStr = _webserver.arg(String(i) + "_readpin");
            readpinStr.trim();
            data.gpio[i].map = readpinStr == "" ? IO_MAP_NONE : readpinStr.toInt();
        }
        config_io_save(data);
        _webserver.send(200, "text/html", view_portal_io(config_io_get()));
        io_setup();
    });
}

#endif