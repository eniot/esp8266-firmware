#ifndef app_controller_api_h
#define app_controller_api_h

#include "webserver.h"
#include "mqtt.h"
#include "network.h"
#include "io.h"

void api_controller()
{
    _webserver.on("/api", HTTP_GET, [] {
        _webserver.send(200, "application/json", "[\"/api/network\",\"/api/mqtt\",\"/api/io\"]");
    });
    _webserver.on("/api/network", HTTP_GET, [] {
        _webserver.send(200, "application/json", network_status());
    });
    _webserver.on("/api/mqtt", HTTP_GET, [] {
        _webserver.send(200, "application/json", mqtt_status());
    });
    _webserver.on("/api/io", HTTP_GET, [] {
        _webserver.send(200, "application/json", io_status());
    });
}

#endif