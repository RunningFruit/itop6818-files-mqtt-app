#include "mqttclient.h"
#include "cJSON.h"

#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define HOST "47.98.119.123"
#define PORT  1883
#define KEEP_ALIVE 60
#define MSG_MAX_SIZE  512
#define TOPIC_NUM 3
bool session = true;
const static char* topic[TOPIC_NUM] =
{
	"Gai爷:",
	"topic",
	"topic-led"
};

#define MAX 3

int fd;
char *buzzer = "/dev/buzzer_ctl";

void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
	if (message->payloadlen) {
		printf("%s %s", message->topic, (char *)message->payload);

		/**************************
		 *      json的解析
		 **************************/
		char jsonString[];
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
			if (json_cmd->cmd == cJSON_String) {
				printf("指令：%s\n", json_name->valuestring);
			}
			//释放内存
			cJSON_Delete(json);
		}

		//判断指令
		if (strcmp(json_type, "led") == 0) {

			if (strcmp(json_cmd, "on") == 0) {
				if ((fd = open(buzzer, O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
					printf("open %s failed\n", buzzer);
				else
				{
					printf("open %s success\r\n", buzzer);
					ioctl(fd, 1);	//parameter 2 is cmd ,cmd = 1 buzzer on
					sleep(1);
					ioctl(fd, 0);
					sleep(1);
				}
			} else {
				close(fd);
			}

		}

	} else {
		printf("%s (null)\n", message->topic);
	}
	fflush(stdout);
}

void my_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
	int i;
	if (!result) {
		/* Subscribe to broker information topics on successful connect. */
		mosquitto_subscribe(mosq, NULL, topic[1], 2);
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


struct mosquitto *mosq = NULL;
char buff[MSG_MAX_SIZE];

void init_mqtt()
{

	//libmosquitto 库初始化
	mosquitto_lib_init();
	//创建mosquitto客户端
	mosq = mosquitto_new(NULL, session, NULL);
	if (!mosq) {
		printf("create client failed..\n");
		mosquitto_lib_cleanup();
		return;
	}
	//设置回调函数，需要时可使用
	mosquitto_log_callback_set(mosq, my_log_callback);
	mosquitto_connect_callback_set(mosq, my_connect_callback);
	mosquitto_message_callback_set(mosq, my_message_callback);
	mosquitto_subscribe_callback_set(mosq, my_subscribe_callback);


	//连接服务器
	if (mosquitto_connect(mosq, HOST, PORT, KEEP_ALIVE)) {
		fprintf(stderr, "Unable to connect.\n");
		return;
	}
	//开启一个线程，在线程里不停的调用 mosquitto_loop() 来处理网络信息
	int loop = mosquitto_loop_start(mosq);
	if (loop != MOSQ_ERR_SUCCESS)
	{
		printf("mosquitto loop error\n");
		return;
	}

}

void pub_mqtt() {
	while (fgets(buff, MSG_MAX_SIZE, stdin) != NULL)
	{
		//发布消息
		mosquitto_publish(mosq, NULL, topic[2], strlen(buff) + 1, buff, 0, 0);
		memset(buff, 0, sizeof(buff));
	}
}

void end_mqtt() {
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
}