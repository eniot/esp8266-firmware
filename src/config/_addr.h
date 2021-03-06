#ifndef app_config_addr_h
#define app_config_addr_h

// Activated
#define _ACTIVATED_ADDR 0
#define _ACTIVATED_SIZE 1

// WIFI SSID
#define _WIFI_SSID_ADDR (_ACTIVATED_ADDR + _ACTIVATED_SIZE)
#define _WIFI_SSID_SIZE 32

// WIFI PASSWORD
#define _WIFI_PASSWORD_ADDR (_WIFI_SSID_ADDR + _WIFI_SSID_SIZE)
#define _WIFI_PASSWORD_SIZE 32

// DHCP
#define _DHCP_ADDR (_WIFI_PASSWORD_ADDR + _WIFI_PASSWORD_SIZE)
#define _DHCP_SIZE 1

// IP
#define _IP_ADDR (_DHCP_ADDR + _DHCP_SIZE)
#define _IP_SIZE 16

// SUBNET
#define _SUBNET_ADDR (_IP_ADDR + _IP_SIZE)
#define _SUBNET_SIZE 16

// GATEWAY
#define _GATEWAY_ADDR (_SUBNET_ADDR + _SUBNET_SIZE)
#define _GATEWAY_SIZE 16

// ACCESS PASSWORD
#define _ACCESS_PASSWORD_ADDR (_GATEWAY_ADDR + _GATEWAY_SIZE)
#define _ACCESS_PASSWORD_SIZE 32

// Device name
#define _NAME_ADDR (_ACCESS_PASSWORD_ADDR + _ACCESS_PASSWORD_SIZE)
#define _NAME_SIZE 32

// DNS
#define _DNS_ADDR (_NAME_ADDR + _NAME_SIZE)
#define _DNS_SIZE 1

// DNS1
#define _DNS1_ADDR (_DNS_ADDR + _DNS_SIZE)
#define _DNS1_SIZE 16

// DNS2
#define _DNS2_ADDR (_DNS1_ADDR + _DNS1_SIZE)
#define _DNS2_SIZE 16

// MQTT_ENABLED
#define _MQTT_ENABLED_ADDR (_DNS2_ADDR + _DNS2_SIZE)
#define _MQTT_ENABLED_SIZE 1

// MQTT_SERVER
#define _MQTT_SERVER_ADDR (_MQTT_ENABLED_ADDR + _MQTT_ENABLED_SIZE)
#define _MQTT_SERVER_SIZE 64

// MQTT_PORT
#define _MQTT_PORT_ADDR (_MQTT_SERVER_ADDR + _MQTT_SERVER_SIZE)
#define _MQTT_PORT_SIZE 2

// MQTT_USERNAME
#define _MQTT_USERNAME_ADDR (_MQTT_PORT_ADDR + _MQTT_PORT_SIZE)
#define _MQTT_USERNAME_SIZE 32

// MQTT_PASSWORD
#define _MQTT_PASSWORD_ADDR (_MQTT_USERNAME_ADDR + _MQTT_USERNAME_SIZE)
#define _MQTT_PASSWORD_SIZE 32

// MQTT_TOPIC
#define _MQTT_TOPIC_ADDR (_MQTT_PASSWORD_ADDR + _MQTT_PASSWORD_SIZE)
#define _MQTT_TOPIC_SIZE 32

// IO
#define _IO_ADDR (_MQTT_TOPIC_ADDR + _MQTT_TOPIC_SIZE)
#define _IO_STARTPIN 0
#define _IO_ENDPIN 16
#define _IO_COUNT (_IO_ENDPIN + 1)

#define _IO_FUNC_SIZE 1
#define _IO_FUNC_ADDR(ioindex) (_IO_ADDR + ioindex)
#define _IO_FUNC_ADDR_END (_IO_FUNC_ADDR(_IO_ENDPIN) + _IO_FUNC_SIZE)

#define _IO_FLAG_SIZE 1
#define _IO_FLAG_ADDR(ioindex) (_IO_FUNC_ADDR_END + ioindex)
#define _IO_FLAG_ADDR_END (_IO_FLAG_ADDR(_IO_ENDPIN) + _IO_FLAG_SIZE)

#define _IO_VAL_SIZE 1
#define _IO_VAL_ADDR(ioindex) (_IO_FLAG_ADDR_END + ioindex)
#define _IO_VAL_ADDR_END (_IO_VAL_ADDR(_IO_ENDPIN) + _IO_VAL_SIZE)

#define _IO_MAP_SIZE 1
#define _IO_MAP_ADDR(ioindex) (_IO_VAL_ADDR_END + ioindex)
#define _IO_MAP_ADDR_END (_IO_MAP_ADDR(_IO_ENDPIN) + _IO_MAP_SIZE)

#define _IO_LABEL_SIZE 16
#define _IO_LABEL_ADDR(ioindex) (_IO_MAP_ADDR_END + (ioindex * _IO_LABEL_SIZE))
#define _IO_LABEL_ADDR_END (_IO_LABEL_ADDR(_IO_ENDPIN) + _IO_LABEL_SIZE)

// END EEPROM
#define _END_ADDR (_IO_ADDR + _IO_LABEL_ADDR_END)

// Public constants
#define EEPROM_SIZE (_END_ADDR + 1)

#endif