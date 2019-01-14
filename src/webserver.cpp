#include "webserver.h"
#include "views/activation_index.h"
#include "controller/portal.h"
#include "controller/activation.h"
#include <Logger.h>
#include "config.h"
#include "mqtt.h"
#include "network.h"

ESP8266WebServer _webserver(80);

void webserver_setup()
{
    if (config_activated())
        portal_controller();
    else
        activation_controller();

    _webserver.onNotFound([]() {
        _webserver.sendHeader("Location", "/", true); //Redirect to our html web page
        _webserver.send(302, "text/plane", "");
    });

    _webserver.begin();
}

void webserver_execute()
{
    _webserver.handleClient();
}
