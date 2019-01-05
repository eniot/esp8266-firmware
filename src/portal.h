#ifndef app_portal_h
#define app_portal_h

#include <ESP8266WebServer.h>
#include "config.h"

void update_network_from_web(ESP8266WebServer *server, config_network_t *data);
void update_access_from_web(ESP8266WebServer *server, config_access_t *data);

void portal_setup();
void portal_execute();

#endif