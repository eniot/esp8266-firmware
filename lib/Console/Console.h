#ifndef ebs_console_h
#define ebs_console_h

#define PRINT(message) Serial.print(message)
#define PRINTLN(message) Serial.println(message)
#define NEWLINE PRINTLN("");
#define PRINTF(message, args...) Serial.printf(message, args)
#define PRINTSTATUS(name, status) PRINT(name); PRINT("\t: "); PRINT(status); NEWLINE;
#define PRINTSTATUS2(name, status) PRINT(name); PRINT("\t\t: "); PRINT(status); NEWLINE;

#ifdef LOG_ENABLED
#define LOG(type, message) PRINT("["); PRINT(type); PRINT("["); PRINT(millis()); PRINT(" : "); PRINTLN(message);
#define LOGF(type, message, args...) PRINT("["); PRINT(type); PRINT("["); PRINT(millis()); PRINT(" : "); PRINTF(message, args); NEWLINE;
#else
#define LOG(type, message) 
#define LOGF(type, message, args...) 
#endif

#define LOG_ERROR(message) LOG("ERROR", message)
#define LOG_INFO(message) LOG("INFO", message)
#define LOG_DEBUG(message) LOG("DEBUG", message)
#define LOG_TRACE(message) LOG("TRACE", message)
#define LOG_WARN(message) LOG("WARN", message)

#define LOGF_ERROR(message, args...) LOGF("ERROR", message, args)
#define LOGF_INFO(message, args...) LOGF("INFO", message, args)
#define LOGF_DEBUG(message, args...) LOGF("DEBUG", message, args)
#define LOGF_TRACE(message, args...) LOGF("TRACE", message, args)
#define LOGF_WARN(message, args...) LOGF("WARN", message, args)

#include <Arduino.h>

class ConsoleClass
{
  private:
    bool _prompt(long msec, const char *prefix);
    bool _insideInterval(long startTime, long msec);
    bool _promptString(String &value, long msec, bool secure);
    void _eraseChars(unsigned int count);

  public:
    void init();
    // int
    bool promptInt(int &value, long msec);
    bool promptInt(int &value)
    {
        return promptInt(value, -1);
    }
    // Any key
    bool promptAnyKey(long msec);
    // String
    bool promptString(String &value, long msec)
    {
        return _promptString(value, msec, false);
    }
    bool promptString(String &value)
    {
        return promptString(value, -1);
    }
    // Secure string
    bool promptSecureString(String &value, long msec)
    {
        return _promptString(value, msec, true);
    }
    bool promptSecureString(String &value)
    {
        return promptSecureString(value, -1);
    }
    // Spinner
    void spinner(unsigned int count, String message);
};

extern ConsoleClass Console;

#endif