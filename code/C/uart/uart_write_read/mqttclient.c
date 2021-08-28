#include "mqttclient.h"
#include "cJSON.h"


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <poll.h>
#include <sys/prctl.h>
#include <signal.h>

//串口接收到字符z，则结束
#define EXIT_CHAR 'z'

//子进程运行的标志位，child_signal=0子进程循环通过串口发送，=1子进程结束
static int child_signal;
void handle_signal(int signo) {
	if (signo == SIGHUP) {
		printf("child recv SIGHUP..\n");
		child_signal = 1;
	}
}

//串口初始化函数
int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio, oldtio;
	if  ( tcgetattr( fd, &oldtio)  !=  0) {
		perror("SetupSerial 1");
		return -1;
	}
	bzero( &newtio, sizeof( newtio ) );
	newtio.c_cflag  |=  CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;

	switch ( nBits )
	{
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 8:
		newtio.c_cflag |= CS8;
		break;
	}

	switch ( nEvent )
	{
	case 'O':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'E':
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		break;
	case 'N':
		newtio.c_cflag &= ~PARENB;
		break;
	}

	switch ( nSpeed )
	{
	case 2400:
		cfsetispeed(&newtio, B2400);
		cfsetospeed(&newtio, B2400);
		break;
	case 4800:
		cfsetispeed(&newtio, B4800);
		cfsetospeed(&newtio, B4800);
		break;
	case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	case 115200:
		cfsetispeed(&newtio, B115200);
		cfsetospeed(&newtio, B115200);
		break;
	case 460800:
		cfsetispeed(&newtio, B460800);
		cfsetospeed(&newtio, B460800);
		break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	}
	if ( nStop == 1 )
		newtio.c_cflag &=  ~CSTOPB;
	else if ( nStop == 2 )
		newtio.c_cflag |=  CSTOPB;
	newtio.c_cc[VTIME]  = 0;
	newtio.c_cc[VMIN] = 0;
	tcflush(fd, TCIFLUSH);
	if ((tcsetattr(fd, TCSANOW, &newtio)) != 0)
	{
		perror("com set error");
		return -1;
	}

	//	printf("set done!\n\r");
	return 0;
}


void excuteByCmd(char* type, char* cmd) {
	//判断指令
	if (strcmp(type, "uart_rw") == 0) {
		if (strcmp(cmd, "on") == 0) {

			int fd, ret, nread, count = 0;
			char *uart_innode;
			char *buffer = "hello world!\n";
			char buff[8];
			struct pollfd fds[1];
			child_signal = 0;

			printf("\r\n uart_write_read_test start\r\n");
			if (2 != argc) {
				printf("Usage:	uart_write_read [uart port]\r\n");
				return;
			}
			printf("TestDemon input char 'z',TestDemon exit!\n");

			uart_innode = argv[1];
			if ((fd = open(uart_innode, O_RDWR | O_NOCTTY | O_NDELAY)) < 0) {
				printf("open %s is failed", uart_innode);
				return;
			}
			else {
				printf("open %s is success\n", uart_innode);
				//串口参数配置
				set_opt(fd, 115200, 8, 'N', 1);
				fds[0].fd = fd;
				fds[0].events = POLLIN;
			}

			pid_t pid;
			//创建进程
			pid = fork();
			//创建进程出错
			if (pid == -1) {
				printf("fork failed\n");
				return;
			}
			//子进程
			else if (!pid) {
				//初始化信号函数，当父进程结束，则执行信号函数，修改child_signal的值为1
				//当child_signal=1，则子进程结束
				signal(SIGHUP, handle_signal);
				prctl(PR_SET_PDEATHSIG, SIGHUP);
				while (!child_signal) {
					//串口发送函数，没个1秒发送一次字符串
					write(fd, buffer, strlen(buffer));
					sleep(1);
				}
				printf("child fork exit ...!\n");
				return;
			}
			//父进程
			else {
				while (1) {
					//轮询的方式来确认串口是否接收到数据
					ret = poll(fds, 1, 5000);
					if (ret == -1) {
						printf("poll error!\n");
						return;
					}
					else if (!ret) {
						printf("Time out!\n");
						printf("fds[0].revents is %d!\n", (char)(fds[0].revents));
						printf("recev all count = %d!\n", count);
					}
					else if (fds[0].revents & POLLIN) {
						//串口接收函数
						while ((nread = read(fd, buff, 1)) > 0) {
							count += nread;
							printf("get data count = %d!\n", count);
							//如果收到字符z，则退出程序
							if (EXIT_CHAR == buff[0]) {
								printf("parent fork exit ...!\n");
								close(fd);
								return;
							}
						}
					}
				}
			}
			close(fd);

		} else {
			// close(fd);
		}
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

