#ifndef app_io_h
#define app_io_h

#include <WString.h>
#include "config.h"

String io_status();
bool io_update(String status);
bool io_update(ioindex_t pin, uint8_t val, bool persist = false, bool publish = false);
uint8_t io_fetch(ioindex_t pin);
void io_setup();
void io_setup(ioindex_t pin);
bool io_toggle(ioindex_t pin, bool publish = false);
bool io_valid_pin(ioindex_t pin);

#endif