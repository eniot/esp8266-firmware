#include "webserver.h"
#include "views/setup_index.h"
#include "controller/portal.h"
#include "controller/setup.h"
#include "controller/api.h"
#include <Logger.h>
#include "config.h"
#include "mqtt.h"
#include "network.h"

ESP8266WebServer _webserver(80);

void webserver_setup()
{
    if (config_setup_complete())
        portal_controller();
    else
        setup_controller();

    api_controller();

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
