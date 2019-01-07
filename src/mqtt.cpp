#include "mqtt.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "config.h"
#include "logger.h"

#define _MQTT_ACK "hi"
#define _MQTT_ACK_INTERVAL 10000

bool _mqtt_enabled = false;
config_mqtt_t _mqtt_cfg;
String _mqtt_outTopic;
String _mqtt_inTopic;
String _mqtt_clientId;

WiFiClient _wificlient;
PubSubClient _mqttclient(_wificlient);

void _callback(char *topic, byte *payload, unsigned int length);
void _ack();

void mqtt_setup()
{
    _mqtt_enabled = true;
    _mqtt_cfg = config_mqtt_get();
    _mqtt_clientId = config_name_get();
    _mqtt_inTopic = String("cmd/") + _mqtt_cfg.topic + String("/#");
    _mqtt_outTopic = String("res/") + _mqtt_cfg.topic;
    PRINTSTATUS("MQTT", _mqtt_cfg.server + " port " + _mqtt_cfg.port);
    _mqttclient.setServer(_mqtt_cfg.server.c_str(), _mqtt_cfg.port);
    _mqttclient.setCallback(_callback);
}

void reconnect()
{    
    while (!_mqttclient.connected())
    {
        LOG_INFO("Attempting MQTT connection...");        
        if (_mqttclient.connect(_mqtt_clientId.c_str(), _mqtt_cfg.username.c_str(), _mqtt_cfg.password.c_str()))
        {
            LOG_INFO("MQTT Connected");       
            PRINTSTATUS("Topic IN", _mqtt_inTopic);
            PRINTSTATUS("Topic OUT", _mqtt_outTopic);            
            _mqttclient.publish(_mqtt_outTopic.c_str(), _MQTT_ACK);            
            _mqttclient.subscribe(_mqtt_inTopic.c_str());
        }
        else
        {
            LOG_ERROR("MQTT connection Failed, rc: " + _mqttclient.state());
        }
    }
}

void mqtt_execute()
{
    if (!_mqtt_enabled)
        return;

    if (!_mqttclient.connected())    
        reconnect();

    _mqttclient.loop();
    _ack();
}

bool mqtt_send(const char *payload)
{
    return _mqttclient.publish(_mqtt_outTopic.c_str(), payload);
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
    for (unsigned int i = 0; i < length; i++)
    {
        PRINT((char)payload[i]);
    }
    PRINTLN();
}