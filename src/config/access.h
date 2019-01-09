#ifndef app_config_access_h
#define app_config_access_h

#include <WString.h>

struct config_access_t
{
    String access;
};

config_access_t config_access_get();
config_access_t config_access_default();
void config_access_set(config_access_t data);
void config_access_save(config_access_t data);

#endif
