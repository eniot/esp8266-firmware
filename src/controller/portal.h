#ifndef app_controller_portal_h
#define app_controller_portal_h

#include <ESP8266WebServer.h>
#include "network.h"
#include "views/portal_index.h"
#include "views/portal_network.h"
#include "views/portal_io.h"
#include "views/portal_access.h"
#include "views/portal_mqtt.h"
#include "controller/_utils.h"
#include "webserver.h"
#include "mqtt.h"
#include "io.h"

void portal_controller()
{
    _webserver.on("/", HTTP_GET, [] {
        _webserver.send(200, "text/html", view_portal_index());
    });
    _webserver.on("/", HTTP_POST, [] {
        ioindex_t pin = _webserver.arg("io_pin").toInt();
        io_toggle(pin);
        _webserver.send(200, "text/html", view_portal_index());
    });
    _webserver.on("/network", HTTP_GET, [] {
        _webserver.send(200, "text/html", view_portal_network(config_network_get()));
    });
    _webserver.on("/network", HTTP_POST, [] {
        config_network_t data;
        _update_network_from_web(&data);
        config_network_save(data);
        _webserver.send(200, "text/html", view_portal_network(config_network_get()));
        network_setup();
    });
    _webserver.on("/access", HTTP_GET, [] {
        _webserver.send(200, "text/html", view_portal_access(config_access_get()));
    });
    _webserver.on("/access", HTTP_POST, [] {
        config_access_t data;
        _update_access_from_web(&data);
        config_access_save(data);
        _webserver.send(200, "text/html", view_portal_access(config_access_get()));
    });
    _webserver.on("/io", HTTP_GET, [] {
        _webserver.send(200, "text/html", view_portal_io(config_io_get()));
    });
    _webserver.on("/io", HTTP_POST, [] {
        config_io_t data;
        for (size_t i = 0; i < _IO_COUNT; i++)
        {
            data.gpio[i].label = _webserver.arg(String(i) + "_label");
            data.gpio[i].func = _webserver.arg(String(i) + "_func").toInt();
            data.gpio[i].orient = _webserver.arg(String(i) + "_orient").toInt();
        }
        config_io_save(data);
        _webserver.send(200, "text/html", view_portal_io(config_io_get()));
        io_setup();
    });
    _webserver.on("/mqtt", HTTP_GET, [] {
        _webserver.send(200, "text/html", view_portal_mqtt(config_mqtt_get()));
    });
    _webserver.on("/mqtt", HTTP_POST, [] {
        config_mqtt_t data;
        data.enabled = _webserver.arg("mqtt") == "1";
        if (data.enabled)
        {
            data.server = _webserver.arg("server");
            data.port = _webserver.arg("port").toInt();
            data.topic = _webserver.arg("topic");
            data.username = _webserver.arg("username");
            data.password = _webserver.arg("password");
        }
        config_mqtt_save(data);
        _webserver.send(200, "text/html", view_portal_mqtt(config_mqtt_get()));
        mqtt_setup();
    });
}

#endif