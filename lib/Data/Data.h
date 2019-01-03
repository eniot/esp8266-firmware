#ifndef lib_data_h
#define lib_data_h

#include <Arduino.h>

class DataClass
{
public:
  void init(size_t size);
  bool save();
  void write(int addr, String value);
  String readStr(int addr, size_t maxlen);
  void write(int addr, int value);
  int read(int addr);
};

extern DataClass Data;

#endif