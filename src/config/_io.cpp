#include <Data.h>
#include "config/_io.h"
#include "config/_addr.h"
#include "config/_constants.h"

void config_io_set(config_io_t data)
{    
    unsigned int addr = _IO_ADDR;
    for(size_t i = 0; i < _IO_COUNT; i++)
    {        
        Data.write(addr, data.gpio[i].function);
        addr += _IO_LABEL_SIZE;
        Data.writeStr(addr, data.gpio[i].label);
        addr += _IO_LABEL_SIZE;
    }    
}

void config_io_save(config_io_t data) 
{
    config_io_set(data);
    Data.save();
}

config_io_t config_io_get()
{
    config_io_t data;
    unsigned int addr = _IO_ADDR;
    for(size_t i = 0; i < _IO_COUNT; i++)
    {        
        data.gpio[i].function = Data.read(addr);
        addr += _IO_LABEL_SIZE;
        data.gpio[i].label = Data.readStr(addr, _IO_LABEL_SIZE);
        addr += _IO_LABEL_SIZE;
    }
    return data;
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