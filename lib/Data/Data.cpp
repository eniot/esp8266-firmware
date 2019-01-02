#include "Data.h"
#include <EEPROM.h>

void DataClass::write(int addr, String value)
{
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

void DataClass::write(int addr, int value)
{
    EEPROM.write(addr, value);
}

int DataClass::read(int addr)
{
    return EEPROM.read(addr);
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