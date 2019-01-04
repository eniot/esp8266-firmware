#ifndef app_views_portal_index_h
#define app_views_portal_index_h

#include <Arduino.h>
#include "portal.h"
#include "html.h"
#include "config.h"

html_menu_t menus[4] = {
    html_menu_t{"Home", "/"},
    html_menu_t{"Network", "/network"},
    html_menu_t{"Access", "/access"},
    html_menu_t{"IO", "/io"}};

String view_portal_index(activate_data data)
{
    return "<html lang=\"en\">" +
           html_head("IOT Portal") +
           "<body>" +
           html_menu(menus, 4, "Home") +
           "</body>" +
           "</html>";
}

#endif