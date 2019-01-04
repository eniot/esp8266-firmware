
#ifndef app_views_html_h
#define app_views_html_h

#include <Arduino.h>

String html_meta(String content);
String html_head(String title);
String html_field(String type, String name, String label, String value = "", String className = "", bool required = true);
String html_radios(String name, String label, String values[], String displays[], size_t valCount, String value = "");
String html_button(String text, String type = "submit");

struct html_menu_t
{
    String label;
    String link;
};

String html_menu(html_menu_t items[], size_t size, String active);

#endif