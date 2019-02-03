#ifndef app_views_portal_io_h
#define app_views_portal_io_h

#include <Arduino.h>
#include "html.h"
#include "config.h"
#include "menu.h"

String _io_func_values[4] = {String(IO_UNUSED), String(IO_READONLY), String(IO_READWRITE), String(IO_WRITEONLY)};
String _io_func_displays[4] = {"Unused", "Read Only", "Read/Write", "Write Only"};

String _io_yesno_values[2] = {"no", "yes"};
String _io_yesno_displays[2] = {"No", "Yes"};

String _io_field(unsigned int ioIndex, config_gpio_t data)
{
    char label[25];
    sprintf(label, "<b>GPIO %02d</b>", ioIndex);

    char name_func[10];
    sprintf(name_func, "%d_func", ioIndex);
    char name_invert[15];
    sprintf(name_invert, "%d_invert", ioIndex);
    char name_persist[15];
    sprintf(name_persist, "%d_persist", ioIndex);
    char name_toggle[15];
    sprintf(name_toggle, "%d_toggle", ioIndex);
    char name_label[10];
    sprintf(name_label, "%d_label", ioIndex);

    return html_radios(name_func, label, _io_func_values, _io_func_displays, 4, String(data.func)) +
           html_radios(name_invert, "Invert", _io_yesno_values, _io_yesno_displays, 2, data.invert ? "yes" : "no") +
           html_radios(name_persist, "Persist", _io_yesno_values, _io_yesno_displays, 2, data.persist ? "yes" : "no") +
           html_radios(name_toggle, "Toggle", _io_yesno_values, _io_yesno_displays, 2, data.toggle ? "yes" : "no") +
           html_field("text", name_label, "Label", data.label);
}

String view_portal_io(config_io_t data)
{
    String content = "<html lang=\"en\">" +
                     html_head("IOT Portal - IO") +
                     "<body><div class='container'><form method='POST'>" +
                     html_menu(menu_list, menu_size, "IO");

    for (size_t i = 0; i < _IO_COUNT; i++)
    {
        content += "<br/>" + _io_field(i, data.gpio[i]);
    }
    content += html_button("Save");
    content += "</form></div></body></html>";
    return content;
}

#endif