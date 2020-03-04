#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <string.h>

void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
void my_connect_callback(struct mosquitto *mosq, void *userdata, int result);
void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos);
void my_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str);


#endif