#include "portal.h"
#include <ESP8266WebServer.h>
#include "views/portal_index.h"
#include "views/portal_network.h"
#include "views/portal_io.h"
#include "views/portal_access.h"
#include "views/portal_mqtt.h"
#include "logger.h"
#include "config.h"
#include "mqtt.h"
#include "net.h"

ESP8266WebServer portalWebServer(80);

void portal_setup()
{
    portalWebServer.on("/", HTTP_GET, [] {
        portalWebServer.send(200, "text/html", view_portal_index());
    });
    portalWebServer.on("/network", HTTP_GET, [] {
        portalWebServer.send(200, "text/html", view_portal_network(config_network_get()));
    });
    portalWebServer.on("/network", HTTP_POST, [] {
        config_network_t data;
        update_network_from_web(&portalWebServer, &data);
        config_network_save(data);
        portalWebServer.send(200, "text/html", view_portal_network(config_network_get()));
        network_setup();
    });
    portalWebServer.on("/access", HTTP_GET, [] {
        portalWebServer.send(200, "text/html", view_portal_access(config_access_get()));
    });
    portalWebServer.on("/access", HTTP_POST, [] {
        config_access_t data;
        update_access_from_web(&portalWebServer, &data);
        config_access_save(data);
        portalWebServer.send(200, "text/html", view_portal_access(config_access_get()));
    });
    portalWebServer.on("/io", HTTP_GET, [] {
        portalWebServer.send(200, "text/html", view_portal_io(config_io_get()));
    });
    portalWebServer.on("/io", HTTP_POST, [] {
        config_io_t data;
        for(size_t i = 0; i < _IO_COUNT; i++)
        {
            data.gpio[i].label = portalWebServer.arg(String(i) + "_label");
            data.gpio[i].function = portalWebServer.arg(String(i) + "_function").toInt();
        }
        config_io_save(data);
        portalWebServer.send(200, "text/html", view_portal_io(config_io_get()));
    });
    portalWebServer.on("/mqtt", HTTP_GET, [] {
        portalWebServer.send(200, "text/html", view_portal_mqtt(config_mqtt_get()));
    });
    portalWebServer.on("/mqtt", HTTP_POST, [] {
        config_mqtt_t data;
        data.enabled = portalWebServer.arg("mqtt") == "1";
        if (data.enabled)
        {
            data.server = portalWebServer.arg("server");
            data.port = portalWebServer.arg("port").toInt();
            data.topic = portalWebServer.arg("topic");
            data.username = portalWebServer.arg("username");
            data.password = portalWebServer.arg("password");
        }
        config_mqtt_save(data);
        portalWebServer.send(200, "text/html", view_portal_mqtt(config_mqtt_get()));
        mqtt_setup();
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

void update_network_from_web(ESP8266WebServer *server, config_network_t *data)
{
    data->name = server->arg("name");
    data->wifi_ssid = server->arg("wifi_ssid");
    data->wifi_password = server->arg("wifi_password");
    data->dhcp = server->arg("dhcp") == "1";
    if (!data->dhcp)
    {
        data->ip.fromString(server->arg("ip"));
        data->subnet.fromString(server->arg("subnet"));
        data->gateway.fromString(server->arg("gateway"));
    }
    data->dns = server->arg("dns") == "1";
    if (data->dns)
    {
        data->dns1.fromString(server->arg("dns1"));
        data->dns2.fromString(server->arg("dns2"));
    }
}

void update_access_from_web(ESP8266WebServer *server, config_access_t *data)
{
    data->access = server->arg("access");
}
