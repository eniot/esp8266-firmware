#include "cmd.h"
#include "mqtt.h"
#include "config.h"

cmd_resp_t _cmd_execute_io(cmd_t cmd);
cmd_resp_t _cmd_execute_mqtt(cmd_t cmd);

cmd_resp_t _ok(String msg = "", String domain = "")
{
    cmd_resp_t resp;
    resp.success = true;
    resp.msg = msg;
    resp.domain = domain;
    return resp;
}

cmd_resp_t _err(String msg = "", String domain = "")
{
    cmd_resp_t resp;
    resp.success = false;
    resp.msg = msg;
    resp.domain = domain;
    return resp;
}

cmd_resp_t cmd_execute(cmd_t cmd)
{
    if (cmd.domain_type.equalsIgnoreCase("io"))
        return _cmd_execute_io(cmd);
    if (cmd.domain_type.equalsIgnoreCase("mqtt"))
        return _cmd_execute_mqtt(cmd);
    return _err("invalid_domain_type");
}

cmd_resp_t _cmd_execute_io(cmd_t cmd)
{
    unsigned int gpioindex = config_gpio_index(cmd.domain);
    if (gpioindex < 0)
        return _err("io_notfound");

    config_gpio_t gpio = config_gpio_get(gpioindex);
    if (gpio.function == IO_UNUSED)
        return _err("io_unused");

    String _domain = cmd.domain_type + "/" + cmd.domain;

    if (cmd.command.equalsIgnoreCase("get"))
    {
        return _ok(String(digitalRead(gpioindex)), _domain);
    }
    else if (cmd.command.equalsIgnoreCase("set"))
    {
        if (gpio.function != IO_OUTPUT)
            return _err("io_readonly");

        digitalWrite(gpioindex, cmd.params.equalsIgnoreCase("1") ? HIGH : LOW);
        return _ok(cmd.params, _domain);
    }
    return _err("invalid_io_command");
}

cmd_resp_t _cmd_execute_mqtt(cmd_t cmd)
{
    if (cmd.domain.equals("") && cmd.command.equalsIgnoreCase("ack"))
    {
        return _ok(MQTT_ACK);
    }
    return _err("invalid_mqtt_command");
}