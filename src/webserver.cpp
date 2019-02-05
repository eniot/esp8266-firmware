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
        webserver_redirect("/");
    });

    _webserver.begin();
}

void webserver_redirect(String path)
{
    _webserver.sendHeader("Location", path, true); //Redirect to our html web page
    _webserver.send(302, "text/plane", "");
}

void webserver_execute()
{
    _webserver.handleClient();
}

bool StartUriRequestHandler::canHandle(HTTPMethod method, String uri)
{
    return uri != NULL && uri.startsWith(_startUri) && method == _method;
}

bool StartUriRequestHandler::handle(ESP8266WebServer &server, HTTPMethod requestMethod, String requestUri)
{
    _handler(requestMethod, requestUri);
    return true;
}