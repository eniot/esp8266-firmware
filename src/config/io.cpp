#include <Data.h>
#include "config/io.h"
#include "config/_addr.h"
#include "config/_constants.h"

config_io_t *_io_cfg_cache = NULL;

void config_io_set(config_io_t data)
{
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
    unsigned int addr = _GPIO_ADDR(pin);

    if (currVal.func != data.func)
        Data.write(addr, data.func);

    addr += _IO_FUNC_SIZE;
    
    uint8_t flags = 0b00000000;
    if (data.invert) flags |= IO_FLG_INVERT;
    if (data.persist) flags |= IO_FLG_PERSIST;
    Data.write(addr, flags);

    addr += _IO_FLAG_SIZE;

    if (currVal.value != data.value)
        Data.write(addr, data.value);

    addr += _IO_VAL_SIZE;

    if (currVal.label != data.label)
        Data.writeStr(addr, data.label);
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
    unsigned int addr = _IO_ADDR;
    for (size_t i = 0; i < _IO_COUNT; i++)
    {
        data.gpio[i].func = Data.read(addr);
        addr += _IO_FUNC_SIZE;
        
        uint8_t flags = Data.read(addr);
        data.gpio[i].invert = (flags & IO_FLG_INVERT) == IO_FLG_INVERT;
        data.gpio[i].persist = (flags & IO_FLG_PERSIST) == IO_FLG_PERSIST;
        addr += _IO_FLAG_SIZE;
        
        data.gpio[i].value = Data.read(addr);
        addr += _IO_VAL_SIZE;
        
        data.gpio[i].label = Data.readStr(addr, _IO_LABEL_SIZE);
        addr += _IO_LABEL_SIZE;
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