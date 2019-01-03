#ifndef lib_logger_h
#define lib_logger_h

#include <Arduino.h>

void logger_init()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }
}

#define PRINT(message) Serial.print(message)
#define PRINTLN(message) Serial.println(message)
#define NEWLINE PRINTLN("");
#define PRINTF(message, args...) Serial.printf(message, args)
#define PRINTSTATUS(name, status) \
    PRINT(name);                  \
    PRINT("\t: ");                \
    PRINT(status);                \
    NEWLINE;
#define PRINTSTATUS2(name, status) \
    PRINT(name);                   \
    PRINT("\t\t: ");               \
    PRINT(status);                 \
    NEWLINE;

#ifdef LOG_ENABLED
#define LOG(type, message) \
    PRINT("[");            \
    PRINT(type);           \
    PRINT("[");            \
    PRINT(millis());       \
    PRINT(" : ");          \
    PRINTLN(message);
#define LOGF(type, message, args...) \
    PRINT("[");                      \
    PRINT(type);                     \
    PRINT("[");                      \
    PRINT(millis());                 \
    PRINT(" : ");                    \
    PRINTF(message, args);           \
    NEWLINE;
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

#endif