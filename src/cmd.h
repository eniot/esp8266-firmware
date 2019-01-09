#ifndef app_cmd_h
#define app_cmd_h

#include <Arduino.h>

struct command_t
{
    String topic_cmd;
    String topic_name;
    String domain_type;
    String domain;
    String params;
    String command;
};

struct command_response_t
{
    bool success;
    String messgage;
};

bool cmd_execute(command_t cmd);

#endif