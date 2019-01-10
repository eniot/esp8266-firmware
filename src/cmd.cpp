#include "cmd.h"
#include "mqtt.h"
#include "config.h"
#include "io.h"
#include "ArduinoJson.h"

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
    if (cmd.domain == "")
    {
        if (cmd.command.equalsIgnoreCase("get"))        
            return _ok(io_status());        
        else if (cmd.command.equalsIgnoreCase("set"))        
            return io_update(cmd.params) ? _ok(io_status()) : _err("io_update_failed");        
    }
    unsigned int gpioindex = config_gpio_index(cmd.domain);
    if (gpioindex < 0)
        return _err("io_notfound");

    config_gpio_t gpio = config_gpio_get(gpioindex);
    if (gpio.function == IO_UNUSED)
        return _err("io_unused");

    if (cmd.command.equalsIgnoreCase("get"))    
        return _ok(String(digitalRead(gpioindex)));    
    else if (cmd.command.equalsIgnoreCase("set"))    
        return io_update(gpioindex, cmd.params.toInt()) ? _ok(cmd.params) : _err("io_readonly");

    return _err("invalid_io_command");
}

cmd_resp_t _cmd_execute_mqtt(cmd_t cmd)
{
    if (cmd.domain.equals("") && cmd.command.equalsIgnoreCase("ack"))    
        return _ok(MQTT_ACK);    
        
    return _err("invalid_mqtt_command");
}