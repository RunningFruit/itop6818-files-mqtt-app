

远程服务器安装mosquitto
```
apt-get install -y g++ libssl-dev libc-ares-dev uuid-dev
wget -c https://mosquitto.org/files/source/mosquitto-1.5.5.tar.gz
#解压
tar zxvf mosquitto-1.5.5.tar.gz;cd mosquitto-1.5.5
make;make install
```
解决编译问题
```
#创建链接
ln -s /usr/local/lib/libmosquitto.so.1 /usr/lib/libmosquitto.so.1
#更新链接
ldconfig
```


<a href="https://brightereyer2.github.io/2020/02/15//articles/2020/02/15/1581731864000.html/" target="_blank">mosquitto配置与启动</a><br>
```
mosquitto_passwd -c /etc/mosquitto/pwfile admin

mosquitto_sub -h localhost -t mytopic -u admin -P admin
mosquitto_pub -h localhost -t mytopic -m "test message" -u admin -P admin
```

<a href="https://github.com/yuanabc/ziot-mqtt.git" target="_blank">ziot-mqtt</a>
```
http://localhost:8080/send/data?sendData=123456
https://brightereyer.com/mqtt/devices/led?sendData=on
```

<a href="https://www.cnblogs.com/founderswitch/p/10791931.html" target="_blank">MQTT在react-native中的运行</a><br>
```
git clone https://github.com/ljunb/react-native-iShiWuPai.git
cd react-native-iShiWuPai 
yarn add react_native_mqtt
npm install
react-native run-android
```

armhf math
```
apt-get install --force-yes liblapack-dev libblas-dev libboost-dev libarpack2-dev libarmadillo-dev

apt-get install libc6-dev-i386
```

```
#include包含的顺序错误了
#要先包函stdlib.h stdio.h

gcc -c cJSON.c -lm -o cJSON.o

gcc -c cJSON.c -lm
ar -rc cJSON.a cJSON.o
```

<a href="https://www.jianshu.com/p/9aa57961b17b" target="_blank">cJSON库使用</a>
```
git clone https://github.com/DaveGamble/cJSON
cd cJSON
make all CC=arm-linux-gnueabihf-gcc
make install

gcc test.c -lcjson -o test
```

JsonCpp
```
apt-get install libjsoncpp-dev
```

<a href="https://www.jianshu.com/p/9cb4f3394e58" target="_blank">c语言实现pow(x,y)函数</a>
```
原来是个叫libm的东西，gcc的就叫glibm
那glibm在哪里呢？在glibc，也就是c标准库里
是在是不想再在mac里找各种文件了，我再重新下载一个吧
libm.so位于 libc6-dev 这个包里，那就到pkgs.org上找找

#apt-get install libc6-dev-i386
apt-get install libc6-dev

apt-get install locate
updatedb
locate libm.so
```

编译C程序mqtt
```
make
```

pow2.c
```
#include <stdio.h>
#define ACCURACY 100
double func1(double t,int n);
double func2(double b,int n);
double pow2(double a,double b);
int main() {
    printf("%lf",pow2(5.21,4.11));
    return 0;
}

double pow2(double a,double b){
    if(a==0&&b>0){
        return 0;
    }
    else if(a==0&&b<=0){
        return 1/0;
    }
    else if(a<0&&!(b-(int)b<0.0001||(b-(int)b>0.999))){
        return 1/0;
    }

    if(a<=2&&a>=0){
        double t=a-1;
        double answer=1;
        for(int i=1;i<ACCURACY;i++){
            answer=answer+func1(t,i)*func2(b,i);
        }
        return answer;
    }
    
    else if(a>2){
        int time=0;
        
        while(a>2){
            a=a/2;
            time++;
        }
        
        return pow2(a,b)*pow2(2,b*time);
    }
    
    else{
        if((int)b%2==0){
            return pow2(-a,b);
        }
        else {
            return -pow2(-a,b);
        }
    }
}
double func1(double t,int n){
    double answer=1;
    for(int i=0;i<n;i++){
        answer=answer*t;
    }
    
    return answer;
}
double func2(double b,int n){
    double answer=1;
    for(int i=1;i<=n;i++){
        answer=answer*(b-i+1)/i;
    }
    
    return answer;
}
```


<a href="https://blog.csdn.net/whik1194/article/details/84798365" target="_blank">C语言解析天气</a>
<a href="https://wcc-blog.oss-cn-beijing.aliyuncs.com/BlogFile/MyJSON.rar" target="_blank">MyJSON.rar</a>


<a href="https://www.jianshu.com/p/4fcb49b55ff6" target="_blank">cJSON解析一个JSON数据</a>
```
#include <stdio.h>
#include <stdlib.h>
#include "cjson/cJSON.h"

char text[] = "{\"timestamp\":\"2019-03-03 08:45:57\", \"value\":1}";

int main(int argc, const char *argv[])
{
    cJSON *json, *json_value, *json_timestamp;

    json = cJSON_Parse(text);
    if(NULL == json)
    {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        return -1;
    }

    json_value = cJSON_GetObjectItem(json, "value");
    if(json_value->type == cJSON_Number)
    {
        printf("value: %d\n", json_value->valueint);
    }

    json_timestamp = cJSON_GetObjectItem(json, "timestamp");
    if(json_timestamp->type == cJSON_String)
    {
        printf("%s\n", json_timestamp->valuestring);
    }

    cJSON_Delete(json);

    return 0;
}
```

