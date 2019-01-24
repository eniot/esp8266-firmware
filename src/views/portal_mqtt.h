#ifndef app_views_portal_mqtt_h
#define app_views_portal_mqtt_h

#include <Arduino.h>
#include "html.h"
#include "config.h"
#include "../mqtt.h"
#include "menu.h"

String view_portal_mqtt(config_mqtt_t data)
{
    String state = mqtt_state_str();
    String enable_values[2] = {"1", "0"};
    String enable_displays[2] = {"Yes", "No"};
    return "<html lang=\"en\">" +
           html_head("IOT Portal") +
           "<body>" +
           "<div class='container'>" +
           html_menu(menu_list, menu_size, "MQTT") +
           "<form method='POST'>" +
           html_display("Status", state, state == "connected" ? "green" : "orange") +
           html_radios("mqtt", "MQTT Enable", enable_values, enable_displays, 2, data.enabled ? "1" : "0") +
           "<div id=\"mqtt-section\">" +
           html_field("text", "server", "Server (Broker)", data.server, "mqtt-input") +
           html_field("number", "port", "Port", String(data.port), "mqtt-input") +
           html_field("text", "topic", "Topic", data.topic, "mqtt-input", false) +
           html_field("text", "username", "Username", data.username, "mqtt-input", false) +
           html_field("text", "password", "Password", data.password, "mqtt-input", false) +
           "</div>" +
           html_button("Save") +
           "</form>" +
           "</div>" +
           html_script() +
           "<script>mqttChange('" + (data.enabled ? "1" : "0") + "');</script>" +
           "</body></html>";
}

#endif