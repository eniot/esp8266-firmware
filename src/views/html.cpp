#include <Arduino.h>
#include "html.h"

String html_meta(String content)
{
    return "<meta " + content + ">";
}

String html_script()
{
    return R"====(
<script> 
    const dnsRadios = document.getElementsByName('dns');
    const dhcpRadios = document.getElementsByName('dhcp');
    const mqttRadios = document.getElementsByName('mqtt');
    function mqttChange(value)
    {
        const inputs = document.getElementsByClassName('mqtt-input');
        for (var j = 0; j < inputs.length; j++)
        {
            inputs[j].disabled = value === '0';
        }
        document.getElementById('mqtt-section').style.display = value === '0' ? 'none' : 'block';
    }    
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
    for (var i = 0, length = mqttRadios.length; i < length; i++)
    {
        mqttRadios[i].addEventListener('change', function() {
            mqttChange(this.value);
        });
    }
</script>
           )====";
}

String html_head(String title)
{
    return "<head>" +
           html_meta("charset='utf-8'") +
           html_meta("http-equiv='X-UA-Compatible' content='IE=edge'") +
           html_meta("name='viewport' content='width=device-width, initial-scale=1'") +
           "<title>" + title + "</title>" +
           R"====(
<style>
    body {
    	margin: 30px;
        font-size: 14px;
    	font-family: system-ui;
    }
    .row {
     	display: flex;
        margin: 5px;
        flex-flow: column;
    }
    button, .label, .value, label, .radios, input[type="text"],  input[type="password"], input[type="number"] {
        padding: 10px;
        font-size: 14px;
        width: 100%;
    }
    label, .label {
    	margin-right: 20px;
        padding: 10px 0px;
    }
    button {
    	background-color: #0078e7;
    	color: white;
    	border: 0px;
        font-size: 16px;
    	margin: 10px auto;
    }
    .menu {
        list-style: none;
        padding: 0px;
        display: flex;
        justify-content: flex-end;
        margin-bottom: 30px;
    }
    .menu li {
        display: inline-block;
    }
    .menu li a {
        padding: 10px;
        text-decoration: none;
        color: #0078e7;
    }
    .menu li a:hover, .menu li.active a {
        text-decoration: none;
        border-bottom: 1px solid #0078e7;
    }
    @media only screen and (min-width: 600px) {
        .row {
            flex-flow: row;
        }
        button, label, .label, .value, input[type="text"],  input[type="password"], input[type="number"], .radios {
            width: 50%;
        }
    }
    @media only screen and (min-width: 768px) {
        .container {
            max-width: 800px;
            margin: auto;
        }
    }
    .home-io {
        display: flex;
        justify-content: flex-end;
    }
    .home-io form {
        margin: 0px;
    }
    .home-io form button {
        width: 80px;
        height: 150;
        margin: 10px;
        border-radius: 54px;
        color: #0078e7;
        background-color: transparent;
        border: 1px solid #0078e7;
    }
    .home-io form button.btn-on {
        background-color: #0078e7;
        color: white;
        border: white;
    }
</style>
</head>
)====";
}

String html_field(String type, String name, String label, String value, String className, bool required)
{
    String reqStr = required ? "required" : "";
    return "<div class=\"row\"><label>" + label + "</label>" +
           "<input class=\"" + className + "\" " + reqStr + " type=\"" + type + "\" name=\"" + name + "\" placeholder=\"" + label + "\" value=\"" + value + "\" />" +
           "</div>";
}

String html_radios(String name, String label, String values[], String displays[], size_t valCount, String value)
{
    String content = "<div class=\"row\"><label>" + label + "</label>" +
                     "<div class=\"radios\">";

    for (size_t i = 0; i < valCount; i++)
    {
        String checked = value == values[i] ? "checked" : "";
        content += "<input type=\"radio\" name=\"" + name + "\" value=\"" + values[i] + "\" " + checked + "> &nbsp;" + displays[i] + " &nbsp;";
    }

    content += "</div></div>";
    return content;
}

String html_button(String text, String type)
{
    return "<div class=\"row\"><label></label><button type=\"" + type + "\">" + text + "</button></div>";
}

String html_menu(html_menu_t items[], size_t size, String active)
{
    String content = "<ul class='menu'>";
    for (size_t i = 0; i < size; i++)
    {
        String activeClass = items[i].label == active ? "active" : "";
        content += "<li class='" + activeClass + "'><a href='" + items[i].link + "'>" + items[i].label + "</a></li>";
    }
    content += "</ul>";
    return content;
}

String html_display(String name, String value, String color)
{
    char content[400];
    sprintf(content, "<div class='row'><span class='label'>%s</span><strong class='value' style='color:%s'>%s</strong></div>", name.c_str(), color.c_str(), value.c_str());
    return String(content);
}