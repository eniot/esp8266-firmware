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
    a {
        text-decoration: none;
        color: #0078e7;
    }
    .mute { color: #999; }
    .active { font-weight: 600; }
    .row {
     	display: flex;
        margin: 5px;
        flex-flow: column;
    }
    .col {
        width: 100%;
    }
    .block {
        padding: 10px;
        font-size: 14px;
        width: 100%;
        display: block;
    }
    select {
        -webkit-appearance: none;
    }
    label, .label {
    	margin-right: 20px;
        padding: 10px 0px;
    }
    .btn{
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
    }
    .menu li a:hover, .menu li.active a {
        text-decoration: none;
        border-bottom: 1px solid #0078e7;
    }
    @media only screen and (min-width: 600px) {
        .row {
            flex-flow: row;
        }
        .col {
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

String html_row(String content)
{
    return "<div class='row'>" + content + "</div>";
}

String html_row(String col1, String col2)
{
    return html_row("<div class='col'>" + col1 + "</div><div class='col'>" + col2 + "</div>");
}

String html_row_with_label(String label, String content)
{
    return html_row("<label class='block'>" + label + "</label>", content);
}

String html_field(String type, String name, String label, String value, String className, bool required)
{
    String reqStr = required ? "required" : "";
    String content = "<input class='block " + className + "' " + reqStr + " type=\"" + type + "\" name=\"" + name + "\" placeholder=\"" + label + "\" value=\"" + value + "\" />";
    return html_row_with_label(label, content);
}

String html_radios(String name, String label, String values[], String displays[], size_t valCount, String value)
{
    String content = "<div class='block'>";
    for (size_t i = 0; i < valCount; i++)
    {
        String checked = value == values[i] ? "checked" : "";
        content += "<input type='radio' name='" + name + "' value='" + values[i] + "' " + checked + "> &nbsp;" + displays[i] + " &nbsp;";
    }
    content += "</div>";
    return html_row_with_label(label, content);
}

String html_select_option(String value, String display, bool selected)
{
    return "<option value='" + value + "' " + (selected ? "selected" : "") + ">" + display + "</option>";
}

String html_select(String name, String label, String options)
{
    return html_row_with_label(label, "<select class='block' name='" + name + "'>" + options + "</select>");
}

String html_select(String name, String label, String values[], String displays[], size_t valCount, String value)
{
    String options = "";
    for (size_t i = 0; i < valCount; i++)
        options += html_select_option(values[i], displays[i], value == values[i]);
    return html_select(name, label, options);
}

String html_button(String text, String type)
{
    return html_row_with_label("", "<button class='block btn' type='" + type + "'>" + text + "</button>");
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
    sprintf(content, "<strong class='block' style='color:%s'>%s</strong>", color.c_str(), value.c_str());
    return html_row_with_label(name, String(content));
}