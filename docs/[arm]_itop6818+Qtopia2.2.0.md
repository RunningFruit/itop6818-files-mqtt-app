docker+ubuntu

```sh
docker pull ubuntu
docker run --privileged --name qtopia2 -itd -p 8023:22 ubuntu

docker attach qtopia2
apt-get update;apt-get upgrade -y
apt-get install -y vim openssh-server

#设置一个root密码
passwd

#修改配置文件
vim /etc/ssh/sshd_config
#PermitRootLogin prohibit-password
PermitRootLogin yes

/etc/init.d/ssh restart
```

<a href="https://pan.baidu.com/s/12V6yl21PeEdhKNiYFHKRnw" target="_blank">文件资料</a>提取码：8n1e

文件清单

```sh
arm-linux-4.4.1.tar.gz
```

<a href="https://pan.baidu.com/s/1AeqzkboWkJDJjU9HxtXhrA" target="_blank">arm-linux-gcc-5.4.0.tar.gz</a>提取码：uzup
源码方式

```sh
mkdir /usr/local/arm;cd /usr/local/arm;
tar xvf arm-linux-4.4.1.tar.gz;ls 4.4.1;
vim /etc/profile
#export PATH=$PATH:/usr/local/arm/arm-2009q3/bin
export PATH=$PATH:/usr/local/arm/4.4.1/bin
source /etc/profile
arm-none-linux-gnueabi-gcc -v
```

apt-get 方式

```sh
apt-get install -y libx11-dev libxext-dev libxtst-dev
apt-get install -y gcc-arm-linux-gnueabi
arm-linux-gnueabi-gcc -v
```

<a href="https://pan.baidu.com/s/1PUxi_DejVo-bgChQH5bzmA" target="_blank">rootfs_qtopia_qt4-20140103.tar.gz</a>

```
提取码：
2dkz
```

Qtopia2.2.0 源码

```sh
mkdir /root/yizhi;cd /root/yizhi
tar xvf rootfs_qtopia_qt4-20140103.tar.gz
ls qtopia-free-src-2.2.0.tar.gz;ls build
```

触摸的库文件<a href="https://github.com/libts/tslib/releases" target="_blank">tslib</a>

```sh
cd /usr/local/arm
wget https://github.com/libts/tslib/releases/download/1.21/tslib-1.21.tar.gz
tar xvf tslib-1.21.tar.gz;mv tslib-1.21 tslib;ls tslib
```

02-Qtopia 补丁包

```sh
cp \
libXext.so.6.4.0 \
libXmu.so.6.2.0 \
libSM.so.6.0.1 \
libICE.so.6.3.0 \
libXt.so.6.0.0 \
libuuid.so.1.3.0 \
/usr/lib32/

ln -s libXext.so.6.4.0 libXext.so.6
ln -s libXext.so.6 libXext.so
ln -s libXmu.so.6.2.0 libXmu.so.6
ln -s libXmu.so.6 libXmu.so
ln -s libSM.so.6.0.1 libSM.so.6
ln -s libSM.so.6 libSM.so
ln -s libICE.so.6.3.0 libICE.so.6
ln -s libICE.so.6 libICE.so
ln -s libXt.so.6.0.0 libXt.so.6
ln -s libXt.so.6 libXt.so
```

```sh
mkdir /lib32
ln -s libuuid.so.1.3.0 libuuid.so.1
ln -s libuuid.so.1 libuuid.so
```

编译 Qtopia2.2.0 源码

```sh
cd /root/yizhi
./build
ls qtopia-free-2.2.0
```

处理一下字库文件

```sh
cp -r /root/yizhi/qtopia-free-2.2.0/qt2/lib/fonts/helvetica* /root/yizhi/qtopia-free-2.2.0/qtopia/image/opt/Qtopia/lib/fonts/
```

Qtopia

```sh
cp -r /root/yizhi/qtopia-free-2.2.0/qtopia/image/opt/Qtopia /opt
```

打包工具的安装

```sh
cd /
tar xvf linux_tools.tgz
ls /usr/local/bin/
sudo apt-get install -y uboot-mkimage
```

生成文件系统

```sh
cd lollipop-5.1.1_r6/qt_system/system/opt;ls;
cp -r /root/yizhi/qtopia-free-2.2.0/qtopia/image/opt/Qtopia ~/lollipop-5.1.1_r6/qt_system/system/opt
cd ~/lollipop-5.1.1_r6;
build_android.sh qt
```

<a href="https://www.cnblogs.com/wenyihu/articles/7380144.html" target="_blank">迅为 6818 开发板 Qtopia2.2.0 开发环境搭建以及编译镜像</a>
