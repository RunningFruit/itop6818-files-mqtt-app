

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


<a href="https://github.com/yuanabc/ziot-mqtt.git" target="_blank">springboot+mqtt</a>


<a href="https://www.cnblogs.com/founderswitch/p/10791931.html" target="_blank">MQTT在react-native中的运行</a><br>
```
git clone https://github.com/ljunb/react-native-iShiWuPai.git
cd react-native-iShiWuPai 
yarn add react_native_mqtt
npm install
react-native run-android
```


编译C程序mqtt
```
make
```
