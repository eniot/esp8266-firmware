#include <Arduino.h>
#include "html.h"

String html_meta(String content)
{
    return "<meta " + content + ">";
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
    button, label, .radios, input[type="text"],  input[type="password"] {
        padding: 10px;
        font-size: 14px;
        width: 100%;
    }
    label {
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
    }
    .menu li {
        display: inline-block;
    }
    .menu li a {
        padding: 10px;
        text-decoration: none;
        color: #0078e7;
    }
    .menu li a:hover {
        text-decoration: none;
        border-bottom: 1px solid #0078e7;
    }
    .menu li.active a {
        font-weight: 600;
        border-bottom: 2px solid #0078e7;
    }
    @media only screen and (min-width: 600px) {
        .row {
            flex-flow: row;
        }
        button, label, input[type="text"],  input[type="password"], .radios {
            width: 50%;
        }
    }
    @media only screen and (min-width: 768px) {
        .container {
            max-width: 800px;
            margin: auto;
        }
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