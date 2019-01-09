#include <Data.h>
#include "config/mqtt.h"
#include "config/_addr.h"
#include "config/_constants.h"
#include "config/network.h"

bool config_mqtt_enabled()
{
    return Data.read(_MQTT_ENABLED_ADDR) == YES;
}

String _config_mqtt_topic_get()
{
    String topic = Data.readStr(_MQTT_TOPIC_ADDR, _MQTT_TOPIC_SIZE);
    topic.trim();
    if (topic == "")
    {
        topic = config_name_get();
        Data.writeStr(_MQTT_TOPIC_ADDR, topic);
        Data.save();
    }
    return topic;
}

config_mqtt_t config_mqtt_get()
{
    config_mqtt_t data;
    data.enabled = config_mqtt_enabled();
    data.server = Data.readStr(_MQTT_SERVER_ADDR, _MQTT_SERVER_SIZE);
    data.port = Data.read16(_MQTT_PORT_ADDR);
    data.username = Data.readStr(_MQTT_USERNAME_ADDR, _MQTT_USERNAME_SIZE);
    data.password = Data.readStr(_MQTT_PASSWORD_ADDR, _MQTT_PASSWORD_SIZE);
    data.topic = _config_mqtt_topic_get();
    return data;
}

void config_mqtt_set(config_mqtt_t data) 
{
    Data.write(_MQTT_ENABLED_ADDR, data.enabled ? YES : NO);
    if (data.enabled)
    {
        Data.writeStr(_MQTT_SERVER_ADDR, data.server, true);
        Data.write16(_MQTT_PORT_ADDR, data.port);
        Data.writeStr(_MQTT_USERNAME_ADDR, data.username);
        Data.writeStr(_MQTT_PASSWORD_ADDR, data.password);
        Data.writeStr(_MQTT_TOPIC_ADDR, data.topic, true);
    }
}

void config_mqtt_save(config_mqtt_t data)
{
    config_mqtt_set(data);
    Data.save();
}

config_mqtt_t config_mqtt_default() 
{
    config_mqtt_t mdata;
    mdata.enabled = false;
    mdata.server = "mqtt.local";
    mdata.port = 1883;
    mdata.topic = config_name_get();
    mdata.username = "";
    mdata.password = "";
    return mdata;
}