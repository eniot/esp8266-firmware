#ifndef app_config_activation_h
#define app_config_activation_h

#include "config/_access.h"
#include "config/_network.h"

struct config_activation_t : config_network_t, config_access_t
{
};

bool config_activated();
void config_activate(config_activation_t data);
void config_deactivate();
void config_deactivate_await();
config_activation_t config_activation_get();
config_activation_t config_activation_log();

#endif