#ifndef app_views_portal_network_h
#define app_views_portal_network_h

#include <Arduino.h>
#include "portal.h"
#include "html.h"
#include "config.h"
#include "menu.h"

String view_portal_network()
{
    return "<html lang=\"en\">" +
           html_head("IOT Portal") +
           "<body>" +
           "<div class='container'>" +
           html_menu(menu_list, menu_size, "Network") +
           "</div>" +
           "</body>" +
           "</html>";
}

#endif