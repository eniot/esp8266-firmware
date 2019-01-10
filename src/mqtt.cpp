#include "mqtt.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "config.h"
#include "logger.h"
#include "cmd.h"

bool _mqtt_enabled = false;
config_mqtt_t _mqtt_cfg;
char _mqtt_topic_out[50];
char _mqtt_topic_err[50];
char _mqtt_topic_in[50];
String _mqtt_clientid;

WiFiClient _wificlient;
PubSubClient _mqttclient(_wificlient);

void _callback(char *topic, byte *payload, unsigned int length);

void mqtt_setup()
{
    if (!config_activated() || !config_mqtt_enabled())
        return;

    if (_mqttclient.connected())
        _mqttclient.disconnect();

    _mqtt_enabled = true;
    _mqtt_cfg = config_mqtt_get();
    _mqtt_clientid = config_name_get();
    sprintf(_mqtt_topic_in, "cmd/%s/#", _mqtt_cfg.topic.c_str());
    sprintf(_mqtt_topic_out, "res/%s", _mqtt_cfg.topic.c_str());
    sprintf(_mqtt_topic_err, "err/%s", _mqtt_cfg.topic.c_str());
    PRINTSTATUS("MQTT", _mqtt_cfg.server + " port " + _mqtt_cfg.port);
    _mqttclient.setServer(_mqtt_cfg.server.c_str(), _mqtt_cfg.port);
    _mqttclient.setCallback(_callback);
}

bool _mqtt_send(String payload, String topicsuffix)
{
    char topic[50];
    sprintf(topic, "%s/%s", _mqtt_topic_out, topicsuffix.c_str());
    return _mqttclient.publish(topic, payload.c_str());
}

bool _mqtt_err(String payload, String topicsuffix)
{
    char topic[50];
    sprintf(topic, "%s/%s", _mqtt_topic_err, topicsuffix.c_str());
    return _mqttclient.publish(topic, payload.c_str());
}

bool _tryconnect()
{
    if (_mqttclient.connected())
        return true;

    LOG_INFO("Attempting MQTT connection...");
    if (!_mqttclient.connect(_mqtt_clientid.c_str(), _mqtt_cfg.username.c_str(), _mqtt_cfg.password.c_str()))
    {
        LOG_ERROR(String("MQTT connection Failed, rc: ") + _mqttclient.state());
        return false;
    }
    LOG_INFO("MQTT Connected");
    PRINTSTATUS("Topic IN", _mqtt_topic_in);
    PRINTSTATUS("Topic OUT", _mqtt_topic_out);
    PRINTSTATUS("Topic ERR", _mqtt_topic_err);
    _mqtt_send(MQTT_ACK_INIT, "mqtt");
    _mqttclient.subscribe(_mqtt_topic_in);
    return true;
}

void mqtt_execute()
{
    if (!_mqtt_enabled || !_tryconnect())
        return;

    _mqttclient.loop();
}

cmd_t _mqtt_parse_cmd(const char *topic, byte *payload)
{
    cmd_t cmd;
    char tmp[4][32];
    size_t count = sscanf(topic, "%[^'/']/%[^'/']/%[^'/']/%s", tmp[0], tmp[1], tmp[2], tmp[3]);
    if (count >= 4)
        cmd.prop = tmp[3];
    if (count >= 3)
        cmd.domain = tmp[2];
    count = sscanf((char *)payload, "%[^':']:%s", tmp[0], tmp[1]);
    if (count >= 2)
        cmd.param = tmp[1];
    if (count >= 1)
        cmd.cmd = tmp[0];
    return cmd;
}

void _callback(char *topic, byte *payload, size_t length)
{
    LOG_TRACE("MQTT Message arrived");
    payload[length] = '\0';
    cmd_t cmd = _mqtt_parse_cmd(topic, payload);
    PRINTSTATUS("Domain", cmd.domain);
    PRINTSTATUS("Property", cmd.prop);
    PRINTSTATUS("Command", cmd.cmd);
    PRINTSTATUS("Parameter", cmd.param);
    cmd_resp_t resp = cmd_execute(cmd);

    String topicsuffix = cmd.domain;
    if (cmd.prop != "")
        topicsuffix += String("/") + cmd.prop;

    bool succeed = resp.success ? _mqtt_send(resp.msg, topicsuffix) : _mqtt_err(resp.msg, topicsuffix);
    if (!succeed)
    {
        LOG_ERROR("MQTT failed to send message");
    }
}