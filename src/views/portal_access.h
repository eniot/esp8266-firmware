#ifndef app_views_portal_access_h
#define app_views_portal_access_h

#include <Arduino.h>
#include "portal.h"
#include "html.h"
#include "config.h"
#include "menu.h"

String view_portal_access()
{
    return "<html lang=\"en\">" +
           html_head("IOT Portal") +
           "<body>" +
           "<div class='container'>" +
           html_menu(menu_list, menu_size, "Access") +
           "</div>" +
           "</body>" +
           "</html>";
}

#endif