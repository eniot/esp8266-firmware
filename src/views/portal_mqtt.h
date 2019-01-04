#ifndef app_views_portal_mqtt_h
#define app_views_portal_mqtt_h

#include <Arduino.h>
#include "portal.h"
#include "html.h"
#include "config.h"
#include "menu.h"

String view_portal_mqtt()
{
    return "<html lang=\"en\">" +
           html_head("IOT Portal") +
           "<body>" +
           "<div class='container'>" +
           html_menu(menu_list, menu_size, "MQTT") +
           "</div>" +
           "</body>" +
           "</html>";
}

#endif