#ifndef app_views_portal_access_h
#define app_views_portal_access_h

#include <Arduino.h>
#include "portal.h"
#include "html.h"
#include "config.h"
#include "menu.h"

String view_portal_access(config_access_t data)
{
    return "<html lang=\"en\">" +
           html_head("IOT Portal") +
           "<body>" +
           "<div class='container'>" +
           html_menu(menu_list, menu_size, "Access") +
           "<form method='POST'>" +
           html_field("text", "access", "Access Password", data.access) +
           html_button("Change Password") +
           "</form>" +
           "</div>" +
           html_script() +
           "</body></html>";
}

#endif