#ifndef app_cmd_h
#define app_cmd_h

#include <WString.h>

struct cmd_t
{
    String domain;
    String prop;
    String param;
    String cmd;
};

struct cmd_resp_t
{
    bool success;
    String msg;
};

cmd_resp_t cmd_execute(cmd_t cmd);

#endif