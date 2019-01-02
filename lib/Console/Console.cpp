#include "Console.h"

void ConsoleClass::init()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }
}

bool ConsoleClass::_prompt(long msec, const char *prefix)
{
    PRINT(prefix);
    long startTime = millis();
    while (!Serial.available())
    {
        if (!_insideInterval(startTime, msec))
            return false;
        ;
    }
    return true;
}

bool ConsoleClass::_insideInterval(long startTime, long msec)
{
    if (msec < 0)
        return true;
    long currentTime = millis();
    return (startTime + msec) > currentTime;
}

bool ConsoleClass::_promptString(String &value, long msec, bool secure)
{
    if (_prompt(msec, "#> "))
    {
        char ch = '\0';        
        do
        {
            if (Serial.available())
            {
                ch = Serial.read();
                if (isprint(ch))
                {
                    PRINT(secure ? '*' : ch);
                    value += ch;
                }
            }
        } while (ch != '\n');
        PRINTLN("");
        return true;
    }
    return false;
}

void ConsoleClass::_eraseChars(unsigned int count)
{
    for (size_t i = 0; i < count; i++)
    {
        PRINT('\b');
    }
}

bool ConsoleClass::promptInt(int &value, long msec)
{
    if (_prompt(msec, "#> "))
    {
        value = Serial.parseInt();
        PRINTLN(value);
        return true;
    }
    return false;
}

bool ConsoleClass::promptAnyKey(long msec)
{
    bool out = _prompt(msec, "...");
    // Clean buffer
    while (Serial.available())
    {
        Serial.read();
    }
    return out;
}

void ConsoleClass::spinner(unsigned int count, String message)
{    
    PRINT(" " + message);
    _eraseChars(message.length() + 1);
    for (unsigned int i = 0; i < count; i++)
    {
        PRINT("⣾");
        delay(50);
        PRINT("\b⣽");
        delay(50);
        PRINT("\b⣻");
        delay(50);
        PRINT("\b⢿");
        delay(50);
        PRINT("\b⡿");
        delay(50);
        PRINT("\b⣟");
        delay(50);
        PRINT("\b⣯");
        delay(50);
        PRINT("\b⣷");
        delay(50);
        PRINT("\b");
    }    
}

ConsoleClass Console;