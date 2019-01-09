#ifndef app_webserver_h
#define app_webserver_h

#include <ESP8266WebServer.h>

void webserver_setup();
void webserver_execute();

extern ESP8266WebServer _webserver;

#endif