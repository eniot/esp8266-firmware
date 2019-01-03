#include "portal.h"
#include <ESP8266WebServer.h>

ESP8266WebServer portalWebServer(80);

void _portal_root_handler();
void _portal_update_handler();

void portal_setup()
{
    portalWebServer.on("/", HTTP_GET, _portal_root_handler);
    portalWebServer.on("/", HTTP_PUT, _portal_update_handler);
    portalWebServer.onNotFound([]() {
        portalWebServer.send(404, "text/html", "<code>request not found</code>");
    });
    portalWebServer.begin();
}

void portal_execute()
{
    portalWebServer.handleClient();
}

void _portal_root_handler()
{
}

void _portal_update_handler()
{
}