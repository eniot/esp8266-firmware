#include "portal.h"
#include <ESP8266WebServer.h>
#include "views/portal_index.h"
#include "views/portal_network.h"
#include "views/portal_io.h"
#include "views/portal_access.h"
#include "views/portal_mqtt.h"
#include "logger.h"
#include "config.h"

ESP8266WebServer portalWebServer(80);

void portal_setup()
{
    portalWebServer.on("/", HTTP_GET, [] {
        portalWebServer.send(200, "text/html", view_portal_index());
    });
    portalWebServer.on("/network", HTTP_GET, [] {
        portalWebServer.send(200, "text/html", view_portal_network());
    });
    portalWebServer.on("/access", HTTP_GET, [] {
        portalWebServer.send(200, "text/html", view_portal_access());
    });
    portalWebServer.on("/io", HTTP_GET, [] {
        portalWebServer.send(200, "text/html", view_portal_io());
    });
    portalWebServer.on("/mqtt", HTTP_GET, [] {
        portalWebServer.send(200, "text/html", view_portal_mqtt());
    });
    portalWebServer.onNotFound([]() {
        portalWebServer.send(404, "text/html", "<code>request not found</code>");
    });
    portalWebServer.begin();
}

void portal_execute()
{
    portalWebServer.handleClient();
}