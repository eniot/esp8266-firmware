#ifndef app_views_portal_io_h
#define app_views_portal_io_h

#include <Arduino.h>
#include "portal.h"
#include "html.h"
#include "config.h"
#include "menu.h"

String view_portal_io()
{
    return "<html lang=\"en\">" +
           html_head("IOT Portal") +
           "<body>" +
           "<div class='container'>" +
           html_menu(menu_list, menu_size, "IO") +
           "</div>" +
           "</body>" +
           "</html>";
}

#endif