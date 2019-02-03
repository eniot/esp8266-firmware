#ifndef app_config_io_h
#define app_config_io_h

#include <WString.h>
#include "config/_addr.h"

typedef unsigned int ioindex_t;

struct config_gpio_t
{
    int8_t func;
    String label;
    int8_t value;
    bool invert;
    bool persist;
    bool toggle;
    ioindex_t readpin;
};

struct config_io_t
{
    config_gpio_t gpio[_IO_COUNT];
};

config_io_t config_io_get();
config_io_t config_io_default();
void config_io_set(config_io_t data);
void config_io_save(config_io_t data);
void config_gpio_set(ioindex_t pin, config_gpio_t data);
void config_gpio_save(ioindex_t pin, config_gpio_t data);
ioindex_t config_gpio_index(String label);
config_gpio_t config_gpio_get(ioindex_t index);
void config_io_commit();

#endif