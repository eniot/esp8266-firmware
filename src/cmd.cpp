#include "cmd.h"
#include "mqtt.h"
#include "config.h"
#include "network.h"
#include "io.h"
#include <Logger.h>

cmd_resp_t _cmd_execute_io(cmd_t cmd);
cmd_resp_t _cmd_execute_mqtt(cmd_t cmd);
cmd_resp_t _cmd_execute_network(cmd_t cmd);

cmd_resp_t _ok(String msg)
{
    cmd_resp_t resp;
    resp.success = true;
    resp.msg = msg;
    return resp;
}

cmd_resp_t _err(String msg)
{
    cmd_resp_t resp;
    resp.success = false;
    resp.msg = msg;
    return resp;
}

cmd_resp_t cmd_execute(cmd_t cmd)
{
    if (cmd.domain.equalsIgnoreCase("io"))
        return _cmd_execute_io(cmd);
    if (cmd.domain.equalsIgnoreCase("mqtt"))
        return _cmd_execute_mqtt(cmd);
    if (cmd.domain.equalsIgnoreCase("network"))
        return _cmd_execute_network(cmd);
    return _err("invalid_domain");
}

cmd_resp_t _cmd_execute_io(cmd_t cmd)
{
    LOG_TRACE("_cmd_execute_io")
    if (cmd.prop == "")
    {
        if (cmd.cmd.equalsIgnoreCase("get"))
            return _ok(io_status());
        else if (cmd.cmd.equalsIgnoreCase("set"))
            return io_update(cmd.param) ? _ok(io_status()) : _err("io_update_failed");
    }
    else
    {
        unsigned int gpioindex = config_gpio_index(cmd.prop);
        if (gpioindex < 0)
            return _err("io_notfound");

        config_gpio_t gpio = config_gpio_get(gpioindex);
        if (gpio.func == IO_UNUSED)
            return _err("io_unused");

        if (cmd.cmd.equalsIgnoreCase("get"))
            return _ok(String(io_fetch(gpioindex)));
        else if (cmd.cmd.equalsIgnoreCase("set"))
            return io_update(gpioindex, cmd.param.toInt()) ? _ok(cmd.param) : _err("io_readonly");
    }
    return _err("invalid_io_command");
}

cmd_resp_t _cmd_execute_mqtt(cmd_t cmd)
{
    LOG_TRACE("_cmd_execute_mqtt");
    if (cmd.prop.equals("") && cmd.cmd.equalsIgnoreCase("ack"))
        return _ok(MQTT_ACK);
    if (cmd.prop.equals("") && cmd.cmd.equalsIgnoreCase("get"))
        return _ok(mqtt_status());

    return _err("invalid_mqtt_command");
}

cmd_resp_t _cmd_execute_network(cmd_t cmd)
{
    LOG_TRACE("_cmd_execute_network");
    if (cmd.prop.equals("") && cmd.cmd.equalsIgnoreCase("get"))
        return _ok(network_status());

    return _err("invalid_network_command");
}