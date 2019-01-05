#ifndef lib_data_h
#define lib_data_h

#include <Arduino.h>

class DataClass
{
public:
  void init(size_t size);
  bool save();
  void writeStr(int addr, String value, bool emptyCheck = false);
  String readStr(int addr, size_t maxlen);
  void write(int addr, int8_t value);
  int8_t read(int addr);
  void write16(int addr, int16_t value);
  int16_t read16(int addr);
};

extern DataClass Data;

#endif