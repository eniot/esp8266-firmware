#include "io.h"
#include "config.h"
#include <Logger.h>
#include <ArduinoJson.h>
#include <Arduino.h>

String io_status()
{
    LOG_TRACE("io_status");
    config_io_t iodata = config_io_get();
    StaticJsonBuffer<400> jb;
    JsonArray &resp = jb.createArray();
    for (size_t i = 0; i < _IO_COUNT; i++)
    {
        if (iodata.gpio[i].func != IO_UNUSED)
        {
            JsonObject &jobj = resp.createNestedObject();
            jobj["io"] = iodata.gpio[i].label.c_str();
            jobj["val"] = io_fetch(i);
            switch (iodata.gpio[i].func)
            {
            case IO_READONLY:
                jobj["mode"] = "ro";
                break;
            case IO_READWRITE:
                jobj["mode"] = "rw";
                break;
            case IO_WRITEONLY:
                jobj["mode"] = io_valid_pin(iodata.gpio[i].readpin) ? "rw" : "wo";
                break;
            }
        }
    }
    String respStr;
    resp.printTo(respStr);
    return respStr;
}

uint8_t io_fetch(ioindex_t pin)
{
    LOG_TRACE("io_fetch");
    config_gpio_t gpio = config_gpio_get(pin);
    uint8_t val = digitalRead(pin);
    if (gpio.invert)
        val = (val == HIGH) ? LOW : HIGH;
    return val;
}

bool io_update(ioindex_t pin, uint8_t val, bool persist)
{
    LOG_TRACE("io_update(pin,val)");
    config_gpio_t gpio = config_gpio_get(pin);
    if (gpio.func != IO_READONLY)
        return false;
    if (gpio.toggle)
        val = HIGH;
    if (gpio.invert)
        val = (val == HIGH) ? LOW : HIGH;
    digitalWrite(pin, val);
    if (gpio.persist)
    {
        gpio.value = val;
        config_gpio_set(pin, gpio);
        if (persist)
            config_io_commit();
    }
    return true;
}

bool io_update(String status)
{
    LOG_TRACE("io_update(status)");
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

        io_update(gpioindex, jobj.get<int>("val"), false);
    }
    config_io_commit();
    return true;
}

void io_setup()
{
    config_io_t cfg = config_io_get();
    for (ioindex_t i = 0; i < _IO_COUNT; i++)
    {
        io_setup(i);
    }
}

void io_setup(ioindex_t pin)
{
    config_gpio_t gpio = config_gpio_get(pin);
    switch (gpio.func)
    {
    case IO_READONLY:
        pinMode(pin, INPUT);
        break;
    case IO_READWRITE:
    case IO_WRITEONLY:
        pinMode(pin, OUTPUT);
        LOGF_TRACE("Set GPIO%d to %d", pin, gpio.value);
        io_update(pin, gpio.value);
    default:
        break;
    }
}

bool io_toggle(ioindex_t pin)
{
    int8_t tval = io_fetch(pin) == HIGH ? LOW : HIGH;
    return io_update(pin, tval, true);
}

bool io_valid_pin(ioindex_t pin)
{
    return pin >= _IO_STARTPIN && pin <= _IO_ENDPIN;
}