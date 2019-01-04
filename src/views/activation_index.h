#ifndef app_views_activation_index_h
#define app_views_activation_index_h

#include <Arduino.h>
#include "html.h"
#include "config.h"

String view_activation_index_content(config_activation_t data, String actionTitle)
{
    String enable_values[2] = {"1", "0"};
    String enable_displays[2] = {"Yes", "No"};
    return "<form class=\"container\" method=\"POST\" action=\"/\">" +
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
           "</form>" +
           R"====(
<script> 
    const dnsRadios = document.getElementsByName('dns');
    const dhcpRadios = document.getElementsByName('dhcp');
    function dnsChange(value)
    {
        const inputs = document.getElementsByClassName('dns-input');
        for (var j = 0; j < inputs.length; j++)
        {
            inputs[j].disabled = value === '0';
        }
        document.getElementById('dns-section').style.display = value === '0' ? 'none' : 'block';
    }
    function dhcpChange(value)
    {
        const inputs = document.getElementsByClassName('dchp-input');
        for (var j = 0; j < inputs.length; j++)
        {
            inputs[j].disabled = value === '1';
        }
        if (value === '0')
        {
            for (var j = 0, length = dnsRadios.length; j < length; j++)
            {
                dnsRadios[j].checked = dnsRadios[j].value === '1';
            }
            dnsChange("1");
        }
        document.getElementById('dhcp-section').style.display = value === '1' ? 'none' : 'block';
    }
    for (var i = 0, length = dnsRadios.length; i < length; i++)
    {
        dnsRadios[i].addEventListener('change', function() {
            dnsChange(this.value);
        });
    }
    for (var i = 0, length = dhcpRadios.length; i < length; i++)
    {
        dhcpRadios[i].addEventListener('change', function() {
            dhcpChange(this.value);
        });
    }
    dnsChange("0");
    dhcpChange("1");
</script>
           )====";
}

String view_activation_index(config_activation_t data)
{
    return "<html lang=\"en\">" +
           html_head("IOT Activation") +
           "<body>" +
           view_activation_index_content(data, "Activate") +
           "</body>" +
           "</html>";
}

#endif