#include "Data.h"
#include <EEPROM.h>

void DataClass::writeStr(int addr, String value, bool emptyCheck)
{
    if (emptyCheck)
    {
        value.trim();
        if (value == "")
            return;
    }
    int curAddr = addr;
    for (size_t i = 0; i < value.length(); i++)
    {
        EEPROM.write(curAddr, value[i]);
        curAddr++;
    }
    EEPROM.write(curAddr, '\0');
}

String DataClass::readStr(int addr, size_t maxlen)
{
    String value;
    int cAddr = addr;
    char ch = '\0';
    do
    {
        ch = char(EEPROM.read(cAddr));
        if (ch != '\0')
            value += ch;
        cAddr++;
    } while (ch != '\0' && maxlen >= value.length());
    return value;
}

void DataClass::write(int addr, int8_t value)
{
    EEPROM.write(addr, value);
}

int8_t DataClass::read(int addr)
{
    return EEPROM.read(addr);
}

void DataClass::write16(int addr, int16_t value)
{
    EEPROM.write(addr, value >> 8);
    EEPROM.write(addr + 1, value);
}

int16_t DataClass::read16(int addr)
{
    int16_t val = EEPROM.read(addr);
    val = val << 8 | EEPROM.read(addr + 1);
    return val;
}

void DataClass::init(size_t size)
{
    EEPROM.begin(size);
}

bool DataClass::save()
{
    return EEPROM.commit();
}

DataClass Data;