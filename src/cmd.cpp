#include "cmd.h"
#include "mqtt.h"
#include "config.h"

bool _cmd_execute_io(command_t cmd);
bool _cmd_execute_mqtt(command_t cmd);

bool cmd_execute(command_t cmd)
{
    if (cmd.domain_type.equalsIgnoreCase("io"))
        return _cmd_execute_io(cmd);
    if (cmd.domain_type.equalsIgnoreCase("mqtt"))
        return _cmd_execute_mqtt(cmd);
    return false;
}

bool _cmd_execute_io(command_t cmd)
{
    if (cmd.command.equalsIgnoreCase("get"))
    {
        unsigned int gpioindex = config_gpio_index(cmd.domain);
        if (gpioindex < 0)
            return false;
        config_gpio_t gpio = config_gpio_get(gpioindex);
        }
    return false;
}

bool _cmd_execute_mqtt(command_t cmd)
{
    if (cmd.domain.equals("") && cmd.command.equalsIgnoreCase("ack"))
    {
        return mqtt_send(MQTT_ACK);
    }
    return false;
}