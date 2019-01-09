#include <Data.h>
#include "config/io.h"
#include "config/_addr.h"
#include "config/_constants.h"

config_io_t *_io_cfg_cache = NULL;

void config_io_set(config_io_t data)
{    
    unsigned int addr = _IO_ADDR;
    for(size_t i = 0; i < _IO_COUNT; i++)
    {        
        Data.write(addr, data.gpio[i].function);
        addr += _IO_FUNC_SIZE;
        Data.writeStr(addr, data.gpio[i].label);
        addr += _IO_LABEL_SIZE;
    }    
}

void config_io_save(config_io_t data) 
{
    config_io_set(data);
    Data.save();
    _io_cfg_cache = NULL;
}

config_io_t config_io_get()
{
    if(_io_cfg_cache != NULL)
        return *_io_cfg_cache;

    config_io_t data;
    unsigned int addr = _IO_ADDR;
    for(size_t i = 0; i < _IO_COUNT; i++)
    {        
        data.gpio[i].function = Data.read(addr);
        addr += _IO_FUNC_SIZE;
        data.gpio[i].label = Data.readStr(addr, _IO_LABEL_SIZE);
        addr += _IO_LABEL_SIZE;
    }
    return data;
}

int config_gpio_index(String label)
{
    config_io_t data = config_io_get();
    for(size_t i = 0; i < _IO_COUNT; i++)
    {
        if(data.gpio[i].label == label) 
        {
            return i;
        }
    }
    return -1;
}

config_gpio_t config_gpio_get(int index)
{
    config_io_t data = config_io_get();
    return data.gpio[index];
}

config_io_t config_io_default() 
{
    config_io_t odata;
    for(size_t i = 0; i < _IO_COUNT; i++)
    {
        odata.gpio[i].function = IO_UNUSED;
        char labelBuff[7];
        sprintf(labelBuff, "GPIO%02d", i);
        odata.gpio[i].label = labelBuff;
    }
    return odata;
}