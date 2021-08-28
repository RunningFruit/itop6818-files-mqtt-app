说明

```
（1）编译后的 server 程序远程的服务器上
（2）编译后的 client 程序放本地联网的linux主板
（3）使用时，在远程服务服务器开放 1883 端口
```

远程服务器安装 mosquitto

```sh
apt-get install -y g++ libssl-dev libc-ares-dev uuid-dev

wget -c https://mosquitto.org/files/source/mosquitto-1.5.5.tar.gz

# 解压
tar zxvf mosquitto-1.5.5.tar.gz;cd mosquitto-1.5.5

# 编译
make;make install
```

解决编译问题

```sh
# 创建链接
ln -s /usr/local/lib/libmosquitto.so.1 /usr/lib/libmosquitto.so.1

# 更新链接
ldconfig
```

<a href="/2020/02/15//articles/2020/02/15/1581731864000.html/" target="_blank">mosquitto 配置与启动</a><br>

```sh
mosquitto_passwd -c /etc/mosquitto/pwfile admin

mosquitto_sub -h localhost -t mytopic -u admin -P admin
mosquitto_pub -h localhost -t mytopic -m "test message" -u admin -P admin
```

<a href="https://github.com/yuanabc/ziot-mqtt.git" target="_blank">参考 | 使用 mqtt 服务 的 java 后端程序</a>

```
请求发送数据接口
/send/data?sendData=123456


前缀为/mqtt/devices/，结尾为 led?sendData=on，代表请求开启led灯
/mqtt/devices/led?sendData=on
```

<a href="https://www.cnblogs.com/founderswitch/p/10791931.html" target="_blank">MQTT 在 react-native 中的运行</a><br>

```sh
git clone https://github.com/ljunb/react-native-iShiWuPai.git

cd react-native-iShiWuPai

yarn add react_native_mqtt

npm install

react-native run-android
```

<details>
<summary>cJSON 缺少pow、floor、fabs函数解决办法</summary>

安装 math 库

```sh
apt-get install libc6-dev
```

<a href="https://www.jianshu.com/p/9aa57961b17b" target="_blank">cJSON 库使用</a><br>

```sh
git clone https://github.com/DaveGamble/cJSON
cd cJSON
make all CC=arm-linux-gnueabihf-gcc
make install

gcc test.c -lcjson -o test
```

```sh
gcc -c cJSON.c -lm -o cJSON.o
#或
gcc -c cJSON.c -lm
ar -rc cJSON.a cJSON.o
```

<a href="https://blog.csdn.net/dengcanjun6/article/details/80624889" target="_blank">cJSON 移植缺少 C 库解决方法</a><br>

```C
#define INT_MAX 2147483647
#define INT_MIN (-INT_MAX - 1)
#define DBL_EPSILON 0.000001

static double pow(double x, int n) {
    int index = n;
    double result = 1;
    double sqr;
    if (x == 0)
        return x;
    if (n == 1)
        return x;
    if (n == 0)
        return 1;
    if (n < 0)
        index = -n;
    if (index % 2 == 0) {
        // index is even
        sqr = pow(x, index / 2);
        result = sqr * sqr;
    }
    else {
        // index is odd
        sqr = pow(x, (index + 1) / 2);
        if (x == 0)
            return 1;
        result = sqr * sqr / x;
    }
    if (result == 0)
        return 0;
    if (n < 0) {
        result = 1 / result;
    }
    return result;
}
static double fabs(double dnumber) {
    *( ( (int *) & dnumber) + 1) &= 0x7FFFFFFF;
    return dnumber;
}

static int floor(float a)
{
    int b = 0;
    if (a >= 0)
    {b = (int)a;}
    else
    {b = (int)a - 1;}
    return b;
}
```

</details>

编译 C 程序 mqtt

```sh
make
```

<details>
<summary>cJSON 解析json数据</summary>

<a href="https://blog.csdn.net/whik1194/article/details/84798365" target="_blank">C 语言解析天气</a><br>
<a href="https://wcc-blog.oss-cn-beijing.aliyuncs.com/BlogFile/MyJSON.rar" target="_blank">MyJSON.rar</a><br>
<a href="https://www.jianshu.com/p/4fcb49b55ff6" target="_blank">cJSON 解析一个 JSON 数据</a><br>

```C
#include <stdio.h>
#include <stdlib.h>
#include "cjson/cJSON.h"

int main(void)
{
    char *cjson_str = NULL;
    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();
    cJSON * next =  cJSON_CreateObject();

    cJSON_AddItemToObject(root, "rc", cJSON_CreateNumber(0));//根节点下添加
    cJSON_AddItemToObject(root, "operation", cJSON_CreateString("CALL"));
    cJSON_AddItemToObject(root, "service", cJSON_CreateString("telephone"));
    cJSON_AddItemToObject(root, "text", cJSON_CreateString("打电话给张三"));
    cJSON_AddItemToObject(root, "semantic", item);//root节点下添加semantic节点
    cJSON_AddItemToObject(item, "slots", next);//semantic节点下添加item节点
    cJSON_AddItemToObject(next, "name", cJSON_CreateString("张三"));//添加name节点

    cjson_str = cJSON_Print(root);
    printf("first json:\n%s\n", cjson_str);
    free(cjson_str);

    cJSON_AddStringToObject(next, "number", "13423452334");
    cJSON_AddNumberToObject(next, "age", 35);
    cJSON_AddBoolToObject(next, "man", 1);

    cjson_str = cJSON_Print(root);
    printf("second json:\n%s\n", cjson_str);
    free(cjson_str);

    cJSON_Delete(root);

    return 0;
}
```

</details>
