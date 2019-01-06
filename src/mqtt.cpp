#include "mqtt.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "config.h"
#include "logger.h"

#define _MQTT_ACK "hi"
#define _MQTT_ACK_INTERVAL 10000

bool _mqtt_enabled = false;
config_mqtt_t _mqtt_cfg;
const char *_mqtt_outTopic;
const char *_mqtt_inTopic;
const char *_mqtt_clientId;

WiFiClient espClient;
PubSubClient client(espClient);

void _callback(char *topic, byte *payload, unsigned int length);
bool _connect();
void _ack();

void mqtt_setup()
{
    _mqtt_enabled = true;
    _mqtt_cfg = config_mqtt_get();
    _mqtt_outTopic = ("res_" + _mqtt_cfg.topic).c_str();
    _mqtt_inTopic = ("cmd_" + _mqtt_cfg.topic).c_str();
    _mqtt_clientId = config_name_get().c_str();
    PRINTSTATUS("MQTT", _mqtt_cfg.server + " port " + _mqtt_cfg.port);
    client.setServer(_mqtt_cfg.server.c_str(), _mqtt_cfg.port);
    client.setCallback(_callback);
}

void mqtt_execute()
{
    if (!_mqtt_enabled || !_connect())
        return;

    client.loop();
    _ack();
}

bool mqtt_send(String msg)
{
    return client.publish(_mqtt_outTopic, msg.c_str());
}

bool _connect()
{
    if (client.connected())
        return true;

    LOG_INFO("Attempting MQTT connection...");
    if (client.connect(_mqtt_clientId))
    {
        LOG_INFO("MQTT connected");
        client.subscribe(_mqtt_inTopic);
        client.publish(_mqtt_outTopic, _MQTT_ACK);
        return true;
    }
    String msg = "MQTT connection failed - ";
    msg += client.state();
    LOG_ERROR(msg);
    return false;
}

unsigned long _next_ack_time = 0;
void _ack()
{
    unsigned long currTime = millis();
    if (currTime >= _next_ack_time)
    {
        LOG_TRACE("MQTT Sending ACK...");
        _next_ack_time = currTime + _MQTT_ACK_INTERVAL;
        if (!mqtt_send(_MQTT_ACK))
        {
            LOG_ERROR("MQTT ACK Failed");
        }
    }
}

void _callback(char *topic, byte *payload, unsigned int length)
{
    LOG_TRACE("MQTT Message arrived");
    for (int i = 0; i < length; i++)
    {
        PRINT((char)payload[i]);
    }
    PRINTLN();
}