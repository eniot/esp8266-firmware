#ifndef app_controller_activation_h
#define app_controller_activation_h

#include <ESP8266WebServer.h>
#include "net.h"
#include "config.h"
#include "views/activation_index.h"
#include "controller/_utils.h"
#include "webserver.h"

void activation_controller()
{
    _webserver.on("/", HTTP_GET, [] {
        _webserver.send(200, "text/html", view_activation_index(config_activation_get()));
    });
    _webserver.on("/", HTTP_POST, [] {        
        config_activation_t data;
        _update_access_from_web(&data);
        _update_network_from_web(&data);
        config_activate(data);
        ESP.restart();
    });
}

#endif