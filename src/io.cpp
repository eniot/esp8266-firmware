#include "io.h"
#include "config.h"
#include <Logger.h>
#include <ArduinoJson.h>
#include <Arduino.h>

String io_status()
{
    LOG_TRACE("io_status")
    config_io_t iodata = config_io_get();
    StaticJsonBuffer<400> jb;
    JsonArray &resp = jb.createArray();
    for (size_t i = 0; i < _IO_COUNT; i++)
    {
        if (iodata.gpio[i].func != IO_UNUSED)
        {
            JsonObject &jobj = resp.createNestedObject();
            jobj["io"] = iodata.gpio[i].label.c_str();
            jobj["val"] = digitalRead(i);
            switch (iodata.gpio[i].func)
            {
            case IO_INPUT:
                jobj["func"] = "ro";
                break;
            case IO_OUTPUT:
                jobj["func"] = "rw";
                break;
            }
        }
    }
    String respStr;
    resp.printTo(respStr);
    return respStr;
}

bool io_update(uint8_t pin, uint8_t val)
{
    LOG_TRACE("io_update(pin,val)")
    config_gpio_t gpio = config_gpio_get(pin);
    if (gpio.func != IO_OUTPUT)
        return false;
    digitalWrite(pin, val == 0x01 ? HIGH : LOW);
    return true;
}

bool io_update(String status)
{
    LOG_TRACE("io_update(status)")
    StaticJsonBuffer<400> jb;
    JsonArray &jstat = jb.parseArray(status);
    if (!jstat.success())
        return false;

    for (size_t i = 0; i < jstat.size(); i++)
    {
        if (!jstat.is<JsonObject>(i))
            continue;

        JsonObject &jobj = jstat.get<JsonObject>(i);
        unsigned int gpioindex = config_gpio_index(jobj.get<String>("io"));

        if (gpioindex < 0)
            continue;

        io_update(gpioindex, jobj.get<int>("val"));
    }
    return true;
}