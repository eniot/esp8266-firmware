#ifndef app_views_portal_io_h
#define app_views_portal_io_h

#include <Arduino.h>
#include "html.h"
#include "config.h"
#include "menu.h"

String _io_func_values[4] = {String(IO_UNUSED), String(IO_READONLY), String(IO_WRITEONLY), String(IO_READWRITE)};
String _io_func_displays[4] = {"Unused", "Read", "Write", "Read/Write"};

String _io_yesno_values[2] = {"no", "yes"};
String _io_yesno_displays[2] = {"No", "Yes"};

String _io_field(config_gpio_t data, ioindex_t pin)
{
    char label[25];
    sprintf(label, "<b>GPIO %02d</b>", pin);

    return html_radios("func", label, _io_func_values, _io_func_displays, 4, String(data.func)) +
           html_radios("invert", "Invert", _io_yesno_values, _io_yesno_displays, 2, data.invert ? "yes" : "no") +
           html_radios("persist", "Persist", _io_yesno_values, _io_yesno_displays, 2, data.persist ? "yes" : "no") +
           html_radios("toggle", "Toggle", _io_yesno_values, _io_yesno_displays, 2, data.toggle ? "yes" : "no") +
           html_field("number", "map", "Pin Map", String(data.map)) +
           html_field("text", "label", "Label", data.label);
}

String view_portal_io(config_io_t data)
{
    String content = "<html lang=\"en\">" +
                     html_head("IOT Portal - IO") +
                     "<body><div class='container'>" +
                     html_menu(menu_list, menu_size, "IO");

    for (size_t i = 0; i < _IO_COUNT; i++)
    {
        content += "<a href='" + String(i) + "'>" + data.gpio[i].label + "</a><br/>";
    }
    content += "</div></body></html>";
    return content;
}

String view_portal_io(config_gpio_t data, ioindex_t pin)
{
    String content = "<html lang=\"en\">" +
                     html_head("IOT Portal - IO") +
                     "<body><div class='container'><form method='POST'>" +
                     html_menu(menu_list, menu_size, "IO");
    content += _io_field(data, pin);
    content += html_button("Save");
    content += "</form></div></body></html>";
    return content;
}

#endif