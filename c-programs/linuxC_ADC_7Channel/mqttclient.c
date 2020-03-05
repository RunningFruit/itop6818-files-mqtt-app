#include "mqttclient.h"
#include "cJSON.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <math.h>

char iiotype[8][32] = {
	"in_voltage0_raw",
	"in_voltage1_raw",
	"in_voltage2_raw",
	"in_voltage3_raw",
	"in_voltage4_raw",
	"in_voltage5_raw",
	"in_voltage6_raw",
	"in_voltage7_raw",
};

//adc_channel 0 			:slide Res
//adc_channel 2 			:temperature
//adc_channel 3,4 			:connector J38 ADC3 ADC4
//others adc_channel 1,5,6,7:reserved
int read_adc_channel(int *value, int adc_channel)
{
	int ret = 0;
	char filepath[100];
	FILE *fp;
	char buf[20];

	sprintf(filepath, "/sys/bus/iio/devices/iio\:device0/%s", iiotype[adc_channel]);

	printf("filepath is  %s!\n", filepath);
	printf("adc_channel is  %d!\n", adc_channel);

	fp = fopen(filepath, "rt");
	if (fp == NULL) {
		printf("open %s fail!\n", filepath);
		*value = 0;
		ret = -1;
		return ret;
	}
	printf("open %s success!\n", filepath);

	ret = fread( buf, 1, sizeof(buf), fp);
	if (ret < 0) {
		printf("fread %s fail!\n", filepath);
		return -1;
	}
	printf("fread %s finish!\n", filepath);

	fclose(fp);
	//printf("return value is %s !\n", buf);
	*value	= atoi(buf);

	return ret;
}

void excuteByCmd(char* type, char* cmd) {
	//判断指令
	if (strcmp(type, "adcChannel") == 0) {

		int ret = 0;
		int value;

		printf("Usage: [%s] [0-7] \n", cmd);
		printf("type 0 :channel0:slide Res \n");
		printf("type 2 :cpu temperature \n");
		printf("type 3 or 4 :connector J38 ADC3 ADC4 \n");
		printf("type 1,5,6,7 :reserved \n");

		ret = read_adc_channel(&value, atoi(cmd));
		if (ret < 0)
		{
			printf("read channel%d failed!\n", atoi(cmd));
		}
		printf("return value is %d!\n", value);

	}
}

void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
	if (message->payloadlen) {
		printf("%s \n %s \n", message->topic, (char *)message->payload);

		/**************************
		 *      json的解析
		 **************************/
		char jsonString[1024];
		strcpy(jsonString, (char *)message->payload);
		cJSON *json = 0, *json_type = 0, *json_cmd = 0;
		json = cJSON_Parse(jsonString);
		//如果解析失败
		if (!json) {
			printf("Error Before:", cJSON_GetErrorPtr());
		} else {
			json_type = cJSON_GetObjectItem(json, "type");
			//如果类型是 字符串
			if (json_type->type == cJSON_String) {
				printf("类型：%s\n", json_type->valuestring);
			}
			json_cmd = cJSON_GetObjectItem(json, "cmd");
			//如果类型是 字符串
			if (json_cmd->type == cJSON_String) {
				printf("指令：%s\n", json_cmd->valuestring);
			}
			//释放内存
			//cJSON_Delete(json);
		}

		excuteByCmd(json_type->valuestring, json_cmd->valuestring);

	} else {
		printf("%s (null)\n", message->topic);
	}
	fflush(stdout);
}

void my_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
	// int i;
	if (!result) {
		/* Subscribe to broker information topics on successful connect. */
		mosquitto_subscribe(mosq, NULL, sub_topic, 2);
	} else {
		fprintf(stderr, "Connect failed\n");
	}
}

void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
	int i;
	printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
	for (i = 1; i < qos_count; i++) {
		printf(", %d", granted_qos[i]);
	}
	printf("\n");
}

void my_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
	/* Pring all log messages regardless of level. */
	printf("%s\n", str);
}

