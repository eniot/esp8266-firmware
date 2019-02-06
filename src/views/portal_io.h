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

String _io_map_options;

String _io_display(String label, ioindex_t pin)
{
    String display = "GPIO" + String(pin);
    if (label != display)
        display += " (" + label + ")";
    return display;
}

String _io_field(config_gpio_t data, ioindex_t pin)
{
    return html_select("func", "<b>GPIO" + String(pin) + "</b>", _io_func_values, _io_func_displays, 4, String(data.func)) +
           html_radios("invert", "Invert", _io_yesno_values, _io_yesno_displays, 2, data.invert ? "yes" : "no") +
           html_radios("persist", "Persist", _io_yesno_values, _io_yesno_displays, 2, data.persist ? "yes" : "no") +
           html_radios("toggle", "Toggle", _io_yesno_values, _io_yesno_displays, 2, data.toggle ? "yes" : "no") +
           html_select("map", "Pin Map", _io_map_options) +
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
        String rowContent = "";
        if (data.gpio[i].func != IO_UNUSED)
        {
            rowContent += "<ul>";
            if (data.gpio[i].invert)
                rowContent += "<li>Inverted</li>";
            if (data.gpio[i].persist)
                rowContent += "<li>Persisted, value: " + String(data.gpio[i].value) + "</li>";
            if (data.gpio[i].toggle)
                rowContent += "<li>Toggle Mode</li>";
            if (io_valid_pin(data.gpio[i].map))
                rowContent += "<li>Mapped to " + _io_display(data.gpio[data.gpio[i].map].label, data.gpio[i].map) + "</li>";
            rowContent += "</ul>";
        }
        String className = (data.gpio[i].func == IO_UNUSED) ? "" : "active";
        String labelContent = "<a class='" + className + "' href='/io/" + String(i) + "'>" + _io_display(data.gpio[i].label, i) + "</a><br/>";
        switch (data.gpio[i].func)
        {
        case IO_READONLY:
            labelContent += "Read Only";
            break;
        case IO_READWRITE:
            labelContent += "Read/Write";
            break;
        case IO_WRITEONLY:
            labelContent += "Write Only";
            break;
        }
        content += html_row(labelContent, rowContent);
    }
    content += "</div></body></html>";
    return content;
}

String view_portal_io(config_gpio_t data, ioindex_t pin)
{
    _io_map_options = html_select_option("100", "Unmapped", data.map == 100);
    config_io_t iodata = config_io_get();
    for (ioindex_t i = 0; i < _IO_COUNT; i++)
        _io_map_options += html_select_option(String(i), _io_display(iodata.gpio[i].label, i), data.map == i);

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