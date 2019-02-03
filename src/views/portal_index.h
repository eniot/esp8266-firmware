#ifndef app_views_portal_index_h
#define app_views_portal_index_h

#include <Arduino.h>
#include "html.h"
#include "config.h"
#include "menu.h"
#include "../io.h"

String view_portal_index()
{
    String content = "<html lang=\"en\">" +
                     html_head("IOT Portal") +
                     "<body>" +
                     "<div class='container'>" +
                     html_menu(menu_list, menu_size, "Home") +
                     "<div class='home-io'>";

    for (ioindex_t pin = 0; pin < _IO_COUNT; pin++)
    {
        config_gpio_t gpio = config_gpio_get(pin);
        if (gpio.func == IO_UNUSED)
            continue;
        String disabled = (gpio.func == IO_READONLY) ? "" : "disabled";

        String className = io_fetch(pin) == HIGH ? "on" : "off";
        content += "<form method='POST'><input type='hidden' name='io_pin' value='";
        content += pin;
        content += "' />";
        content += "<button type='submit' " + disabled + " class='btn-" + className + "'>" + gpio.label + "</button></form>";
    }

    content += "</div></div></body></html>";
    return content;
}

#endif