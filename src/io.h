#ifndef app_io_h
#define app_io_h

#include <WString.h>

String io_status();
bool io_update(String status);
bool io_update(uint8_t pin, uint8_t val);

#endif