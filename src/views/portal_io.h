#ifndef app_views_portal_io_h
#define app_views_portal_io_h

#include <Arduino.h>
#include "portal.h"
#include "html.h"
#include "config.h"
#include "menu.h"

String _io_func_values[3] = { String(IO_UNUSED), String(IO_INPUT), String(IO_OUTPUT) };
String _io_func_displays[3] = { "Unused", "Input", "Output" };

String _io_field(unsigned int ioIndex, config_gpio_t data) 
{
    char namefunc[10];
    sprintf(namefunc, "%d_function", ioIndex);
    char labelfunc[10];
    sprintf(labelfunc, "<strong>GPIO %02d</strong>", ioIndex);     
    char namelbl[10];
    sprintf(namelbl, "%d_label", ioIndex);    
    return html_radios(namefunc, labelfunc, _io_func_values, _io_func_displays, 3, String(data.function)) + 
        html_field("text", namelbl, "Label", data.label);
}

String view_portal_io(config_io_t data)
{
    String content = "<html lang=\"en\">" +
        html_head("IOT Portal - IO") +
        "<body><div class='container'><form method='POST'>" +
        html_menu(menu_list, menu_size, "IO");
    
    for(size_t i = 0; i < _IO_COUNT; i++)
    {
        content += "<br/>" + _io_field(i, data.gpio[i]);
    }
    content += html_button("Save");
    content += "</form></div></body></html>";
    return content;
}

#endif