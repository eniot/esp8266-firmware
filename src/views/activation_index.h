#ifndef app_views_activation_index_h
#define app_views_activation_index_h

#include <Arduino.h>
#include "html.h"
#include "config.h"

String view_activation_index_content(config_activation_t data, String actionTitle)
{
    String enable_values[2] = {"1", "0"};
    String enable_displays[2] = {"Yes", "No"};
    return "<form method=\"POST\" action=\"/\">" +
           html_field("text", "name", "Device Name", data.name) +
           html_field("text", "access", "Access Password", data.access) +
           html_field("text", "wifi_ssid", "WiFi SSID", data.wifi_ssid) +
           html_field("text", "wifi_password", "WiFi Password", data.wifi_password) +
           html_radios("dhcp", "DHCP Enable", enable_values, enable_displays, 2, data.dhcp ? "1" : "0") +
           "<div id=\"dhcp-section\">" +
           html_field("text", "ip", "IP Address", data.ip.toString(), "dhcp-input", false) +
           html_field("text", "subnet", "Subnet Mask", data.subnet.toString(), "dhcp-input", false) +
           html_field("text", "gateway", "Default Gateway", data.gateway.toString(), "dhcp-input", false) +
           "</div>" +
           html_radios("dns", "Custom DNS", enable_values, enable_displays, 2, data.dns ? "1" : "0") +
           "<div id=\"dns-section\">" +
           html_field("text", "dns1", "Primary DNS", data.dns1.toString(), "dhcp-input", false) +
           html_field("text", "dns2", "Secondary DNS", data.dns2.toString(), "dhcp-input", false) +
           "</div>" +
           html_button(actionTitle) +
           "</form>";
}

String view_activation_index(config_activation_t data)
{
    return "<html lang=\"en\">" +
           html_head("IOT Activation") +
           "<body>" +
           "<div class='container'>" +
           view_activation_index_content(data, "Activate") +
           "</div>" +
           html_script() +
           "</body></html>";
}

#endif