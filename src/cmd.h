#ifndef app_cmd_h
#define app_cmd_h

#include <Arduino.h>

struct cmd_t
{
    String topic_cmd;
    String topic_name;
    String domain_type;
    String domain;
    String params;
    String command;
};

struct cmd_resp_t
{
    bool success;
    String msg;
    String domain;
};

cmd_resp_t cmd_execute(cmd_t cmd);

#endif