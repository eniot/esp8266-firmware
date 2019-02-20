#include "cmd.h"
#include "mqtt.h"
#include "config.h"
#include "network.h"
#include "io.h"
#include <Logger.h>
#include <ESP8266httpUpdate.h>

cmd_resp_t _cmd_execute_io(cmd_t cmd);
cmd_resp_t _cmd_execute_mqtt(cmd_t cmd);
cmd_resp_t _cmd_execute_network(cmd_t cmd);
cmd_resp_t _cmd_execute_system(cmd_t cmd);

cmd_resp_t _ok(String msg, uint8_t action = CMD_RESP_ACTION_NONE)
{
    cmd_resp_t resp;
    resp.success = true;
    resp.msg = msg;
    resp.action = action;
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
    if (cmd.domain.equalsIgnoreCase("system"))
        return _cmd_execute_system(cmd);
    return _err("invalid_domain");
}

cmd_resp_t _cmd_execute_io(cmd_t cmd)
{
    LOG_TRACE("_cmd_execute_io");
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

cmd_resp_t _cmd_execute_system(cmd_t cmd)
{
    LOG_TRACE("_cmd_execute_system");
    if (cmd.cmd.equalsIgnoreCase("ver"))
        return _ok(VERSION);
    if (cmd.cmd.equalsIgnoreCase("update"))
    {
        String fingerprint = "CA 06 F5 6B 25 8B 7A 0D 4F 2B 05 47 09 39 47 86 51 15 19 84";
        char host[50] = "github.com";
        char url[100];
        sprintf(url, "/eniot/esp8266-firmware/releases/download/%s/firmware.bin", cmd.param.c_str());

        WiFiClientSecure client;
        client.verify(fingerprint.c_str(), host);
        if (client.connect(host, 443))
        {
            client.print(String("GET ") + String(url) + " HTTP/1.1\r\n" +
                         "Host: " + String(host) + " \r\n" +
                         "User-Agent: Eniot\r\n" +
                         "Connection: close\r\n\r\n");
            while (client.connected())
            {
                String line = client.readStringUntil('\n');
                if (line.startsWith("Location: "))
                {
                    t_httpUpdate_return ret = ESPhttpUpdate.update(line.substring(10), VERSION, fingerprint);
                    switch (ret)
                    {
                    case HTTP_UPDATE_FAILED:
                        LOG_ERROR(ESPhttpUpdate.getLastErrorString());
                        return _err(String("update_failed:") + ESPhttpUpdate.getLastErrorString());

                    case HTTP_UPDATE_NO_UPDATES:
                        LOG_WARN("No updates");
                        return _err("update_not_found");

                    case HTTP_UPDATE_OK:
                        LOG_INFO("Firmware updated, rebooting");
                        return _ok("updated", CMD_RESP_ACTION_RESTART);
                    }                    
                }
            }
        }
        return _err("update_unknown");
    }
    return _err("invalid_system_command");
}

cmd_resp_t _cmd_execute_network(cmd_t cmd)
{
    LOG_TRACE("_cmd_execute_network");
    if (cmd.prop.equals("") && cmd.cmd.equalsIgnoreCase("get"))
        return _ok(network_status());

    return _err("invalid_network_command");
}