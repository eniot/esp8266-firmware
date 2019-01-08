#ifndef app_config_io_h
#define app_config_io_h

#include <WString.h>
#include "config/_addr.h"

struct config_gpio_t
{
    int8_t function;
    String label;
};

struct config_io_t
{
    config_gpio_t gpio[_IO_COUNT];
};

config_io_t config_io_get();
config_io_t config_io_default();
void config_io_set(config_io_t data);
void config_io_save(config_io_t data);

#endif