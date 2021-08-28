#include "mqttclient.h"
#include <math.h>

#define HOST "远程服务器ip地址"
#define PORT  1883
#define KEEP_ALIVE 60
#define MSG_MAX_SIZE  512
#define TOPIC_NUM 3
bool session = true;
struct mosquitto *mosq = NULL;
char buff[MSG_MAX_SIZE];
const static char* pub_topic = "topic-uart-read-write";


void init_mqtt()
{

	//libmosquitto
	mosquitto_lib_init();
	//
	mosq = mosquitto_new(NULL, session, NULL);
	if (!mosq) {
		printf("create client failed..\n");
		mosquitto_lib_cleanup();
		return;
	}
	//
	mosquitto_log_callback_set(mosq, my_log_callback);
	mosquitto_connect_callback_set(mosq, my_connect_callback);
	mosquitto_message_callback_set(mosq, my_message_callback);
	mosquitto_subscribe_callback_set(mosq, my_subscribe_callback);


	//
	if (mosquitto_connect(mosq, HOST, PORT, KEEP_ALIVE)) {
		fprintf(stderr, "Unable to connect.\n");
		return;
	}
	//
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
		//
		mosquitto_publish(mosq, NULL, pub_topic, strlen(buff) + 1, buff, 0, 0);
		memset(buff, 0, sizeof(buff));
	}
}

void end_mqtt() {
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
}

void main(void)
{
	init_mqtt();
	pub_mqtt();
	end_mqtt();
}
