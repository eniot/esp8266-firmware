#ifndef app_views_portal_index_h
#define app_views_portal_index_h

#include <Arduino.h>
#include "html.h"
#include "config.h"
#include "menu.h"

String view_portal_index()
{
    return "<html lang=\"en\">" +
           html_head("IOT Portal") +
           "<body>" +
           "<div class='container'>" +
           html_menu(menu_list, menu_size, "Home") +
           "</div>" +
           "</body>" +
           "</html>";
}

#endif