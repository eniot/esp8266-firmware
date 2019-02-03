#ifndef app_controller_setup_h
#define app_controller_setup_h

#include <ESP8266WebServer.h>
#include "config.h"
#include "views/setup_index.h"
#include "controller/_utils.h"
#include "webserver.h"

void setup_get_handler()
{
    if (!_check_auth())
        return _webserver.requestAuthentication();
    _webserver.send(200, "text/html", view_setup_index(config_setup_get()));
}

void setup_post_handler()
{
    if (!_check_auth())
        return _webserver.requestAuthentication();
    bool setup_completed = config_setup_complete();
    config_setup_t data;
    _update_setup_from_web(&data);
    config_setup(data);
    if (setup_completed)
        setup_get_handler();
    else
        ESP.restart();
}

void setup_controller()
{
    _webserver.on("/", HTTP_GET, setup_get_handler);
    _webserver.on("/", HTTP_POST, setup_post_handler);
}

#endif