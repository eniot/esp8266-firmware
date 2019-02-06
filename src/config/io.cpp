#include <Data.h>
#include "config/io.h"
#include "config/_addr.h"
#include "config/_constants.h"
#include "Logger.h"

config_io_t *_io_cfg_cache = NULL;

void config_io_set(config_io_t data)
{
    LOG_TRACE("config_io_set");
    for (ioindex_t i = 0; i < _IO_COUNT; i++)
        config_gpio_set(i, data.gpio[i]);
}

void config_io_save(config_io_t data)
{
    config_io_set(data);
    config_io_commit();
}

void config_gpio_set(ioindex_t pin, config_gpio_t data)
{
    config_gpio_t currVal = config_gpio_get(pin);

    if (currVal.func != data.func)
        Data.write(_IO_FUNC_ADDR(pin), data.func);

    if (currVal.map != data.map)
        Data.write(_IO_MAP_ADDR(pin), data.map);

    uint8_t flags = 0b00000000;
    if (data.invert)
        flags |= IO_FLG_INVERT;
    if (data.persist)
        flags |= IO_FLG_PERSIST;
    if (data.toggle)
        flags |= IO_FLG_TOGGLE;
    Data.write(_IO_FLAG_ADDR(pin), flags);

    if (currVal.value != data.value)
        Data.write(_IO_VAL_ADDR(pin), data.value);

    if (currVal.label != data.label)
        Data.writeStr(_IO_LABEL_ADDR(pin), data.label);
}

void config_gpio_save(ioindex_t pin, config_gpio_t data)
{
    config_gpio_set(pin, data);
    config_io_commit();
}

config_io_t config_io_get()
{
    if (_io_cfg_cache != NULL)
        return *_io_cfg_cache;

    config_io_t data;
    for (size_t i = 0; i < _IO_COUNT; i++)
    {
        uint8_t flags = Data.read(_IO_FLAG_ADDR(i));
        data.gpio[i].invert = (flags & IO_FLG_INVERT) == IO_FLG_INVERT;
        data.gpio[i].persist = (flags & IO_FLG_PERSIST) == IO_FLG_PERSIST;
        data.gpio[i].toggle = (flags & IO_FLG_TOGGLE) == IO_FLG_TOGGLE;

        data.gpio[i].func = Data.read(_IO_FUNC_ADDR(i));
        data.gpio[i].value = Data.read(_IO_VAL_ADDR(i));
        data.gpio[i].map = Data.read(_IO_MAP_ADDR(i));
        data.gpio[i].label = Data.readStr(_IO_LABEL_ADDR(i), _IO_LABEL_SIZE);
    }
    return data;
}

ioindex_t config_gpio_index(String label)
{
    config_io_t data = config_io_get();
    for (ioindex_t i = 0; i < _IO_COUNT; i++)
    {
        if (data.gpio[i].label == label)
        {
            return i;
        }
    }
    return -1;
}

config_gpio_t config_gpio_get(ioindex_t index)
{
    config_io_t data = config_io_get();
    return data.gpio[index];
}

config_io_t config_io_default()
{
    config_io_t odata;
    for (ioindex_t i = 0; i < _IO_COUNT; i++)
    {
        odata.gpio[i].func = IO_UNUSED;
        odata.gpio[i].invert = false;
        odata.gpio[i].persist = false;
        odata.gpio[i].toggle = false;
        odata.gpio[i].map = IO_MAP_NONE;
        odata.gpio[i].value = LOW;
        char labelBuff[7];
        sprintf(labelBuff, "GPIO%d", i);
        odata.gpio[i].label = labelBuff;
    }
    return odata;
}

void config_io_commit()
{
    Data.save();
    _io_cfg_cache = NULL;
}