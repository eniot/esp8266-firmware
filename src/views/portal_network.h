#ifndef app_views_portal_network_h
#define app_views_portal_network_h

#include <Arduino.h>
#include "html.h"
#include "config.h"
#include "menu.h"

String view_portal_network(config_network_t data)
{
    bool connected = WiFi.status() == WL_CONNECTED;
    String ip = WiFi.localIP().toString();
    String enable_values[2] = {"1", "0"};
    String enable_displays[2] = {"Yes", "No"};
    return "<html lang=\"en\">" +
           html_head("IOT Portal") +
           "<body>" +
           "<div class='container'>" +
           html_menu(menu_list, menu_size, "Network") +
           "<form method='POST'>" +
           html_display("Status", connected ? "connected" : "not_connected", connected ? "green" : "orange") +
           (connected ? html_display("Local IP", ip) : "") +
           html_field("text", "name", "Device Name", data.name) +
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
           html_button("Save") +
           "</form>" +
           "</div>" +
           html_script() +
           "<script>dnsChange('" + (data.dns ? "1" : "0") + "');dhcpChange('" + (data.dhcp ? "1" : "0") + "');</script>" +
           "</body></html>";
}

#endif