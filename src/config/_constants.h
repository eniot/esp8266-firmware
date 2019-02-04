#ifndef app_config_constants_h
#define app_config_constants_h

#define NO 0x01
#define YES 0x02

#define IO_UNUSED 0x00
#define IO_READWRITE 0x01
#define IO_READONLY 0x02
#define IO_WRITEONLY 0x03

#define IO_MAP_NONE 100

#define IO_FLG_INVERT 0b10000000
#define IO_FLG_PERSIST 0b01000000
#define IO_FLG_TOGGLE 0b00100000

#endif
