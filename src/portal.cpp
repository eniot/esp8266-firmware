#include "portal.h"
#include <ESP8266WebServer.h>
#include "views/portal_index.h"
#include "logger.h"
#include "config.h"

ESP8266WebServer portalWebServer(80);

void _portal_root_handler();
void _portal_update_network_handler();
void _portal_update_io_handler();

void portal_setup()
{
    portalWebServer.on("/", HTTP_GET, _portal_root_handler);
    portalWebServer.on("/network", HTTP_POST, _portal_update_network_handler);
    portalWebServer.on("/io", HTTP_POST, _portal_update_io_handler);
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
    LOG_TRACE("_activation_root_handler");
    portalWebServer.send(200, "text/html", view_portal_index(config_get()));
}

void _portal_update_network_handler()
{
}
void _portal_update_io_handler()
{
}