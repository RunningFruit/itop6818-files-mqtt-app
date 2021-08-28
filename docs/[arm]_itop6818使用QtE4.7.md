<a href="https://pan.baidu.com/s/12V6yl21PeEdhKNiYFHKRnw" target="_blank">文件资料</a>提取码：8n1e

<a href="/itop6818-files-QtE4.7" target="_blank">itop6818-QtE4-files</a>

文件清单

```sh
arm-linux-gcc-4.3.2.tar.gz
qt-everywhere-opensource-src-4.7.1.tar.gz
lollipop-5.1.1_r6
linux_tools.tgz
android_env.tgz
```

<a href="http://master.qt.io/archive/qt/5.3/5.3.2/qt-opensource-linux-x64-android-5.3.2.run" target="_blank">qt-opensource-linux-x64-android-5.3.2.run</a>

```
拨码开关编号 1 2 3
EMMC 启动 1 0 1
TF 卡启动 1 0 0
```

```sh
#tar xvf android_env.tgz -C /
#cd /android_env/ubuntu
#./install-devel-packages.sh
#sudo apt-get install -y default-jre default-jdk realpath
sudo apt-get install -y autoconf automake libtool
sudo apt-get install -y lib32c-dev lib32stdc++6
```

交叉编译器编译工具 arm-none-linux-gnueabi-gcc-4.3.2

```sh
mkdir /usr/local/arm;
tar xvf arm-linux-gcc-4.3.2.tar.gz -C /usr/local/arm
```

交叉编译器编译工具 arm-none-linux-gnueabi-gcc-4.3.2 环境变量

```sh
sudo apt-get install -y libx11-dev libxext-dev libxtst-dev lib32z1

vim /etc/profile
export PATH=$PATH:/usr/local/arm/4.3.2/bin
source /etc/profile

#输入arm + tab
#出现编译器“arm-none-linux-gnueabi-gcc-4.3.2”，这就说明编译器路径设置正确

apt-get install -y uboot-mkimage
```

编译 QtE4.7

```sh
mkdir /root/yizhi;cd /root/yizhi
tar -vxf qt-everywhere-opensource-src-4.7.1.tar.gz;
```

build-all

```sh
#/bin/bash

export PATH=/usr/local/arm/4.3.2/bin:$PATH
export PKG_CONFIG_PREFIX=$TOOLCHAIN/arm-none-linux-gnueabi
export TB_CC_PREFIX=arm-linux-
export TOOLCHAIN=/usr/local/arm/4.3.2

rm -fr qt-everywhere-opensource-src-4.7.1
rm -fr /opt/qt-4.7.1

tar xfvz qt-everywhere-opensource-src-4.7.1.tar.gz

cd qt-everywhere-opensource-src-4.7.1

echo yes | ./configure -opensource -embedded arm -xplatform qws/linux-arm-g++  -no-webkit -qt-libtiff -qt-libmng  -qt-mouse-tslib -qt-mouse-pc -qt-gfx-transformed -no-mouse-linuxtp -prefix /opt/qt-4.7.1   -I /usr/local/tslib/include -L /usr/local/tslib/lib

make 2>&1 | tee ../qte4.7.1Makelog && make install
```

执行编译

```sh
chmod 777 build-all
./build-all

#编译完成后
ls /opt/qt-4.7.1

#拷贝qt-4.7.1
cd ~/lollipop-5.1.1_r6/qt_system/system/opt
cp -r /opt/qt-4.7.1 ./

cd ~/lollipop-5.1.1_r6/kernel
#缺省文件配置
sudo cp -r config_for_iTOP6818_linux .config

cd ~/lollipop-5.1.1_r6
sudo ./build_android.sh qt

ls result
boot.img  sd_fusing_uboot.sh  system.img  u-boot-iTOP-6818.bin
mkuboot   system              u-boot.bin
```

打开窗口目录

```sh
xdg-open ~/lollipop-5.1.1_r6/result
```

生成所需的文件、目录

```sh
ls ~/lollipop-5.1.1_r6/result/u-boot-iTOP-6818.bin
ls ~/lollipop-5.1.1_r6/result/boot.img
ls ~/lollipop-5.1.1_r6/result/system.img
```

Qt/E4.7 和 Qtopia 的切换

```sh
vim ~/lollipop-5.1.1_r6/qt_system/system/etc/init.d/rcS
#最后一行改为
#/bin/qtopia &
/bin/qt4 &
```

Qt/E4.7 和 Qtopia2.2.0 的触摸校准

```sh
如果用户烧写镜像后，第一次运行正常，断电重启后，文件系统出现:
Segmentation fault

rm -rf /etc/point*
reboot
```

切换到 Qt/E4.7

```sh
#超级终端中找出和 Qtopia2.2.0 文件系统相关的 ID 号

ps|grep /opt/Qtopia/

kill <ID>

#切换到 Qt/E4.7
qt4

ps|grep qt
```

切换到 Qtopia2.2.0

```sh
ps|grep qt

kill <ID>

#切换到 Qtopia2.2.0
qtopia
```

QtE 库的编译配置选项简介（build-all）

```sh
cd ~/yizhi/;ls
./build-all

./configure \
-opensource \
-embedded \
-xplatform qws/linux-arm-g++ \
-no-webkit \
-qt-libtiff \
-qt-libmng \
-qt-mouse-tslib \
-qt-mouse-pc \
-no-mouse-linuxtp \
-I/usr/local/tslib/include \
-L/usr/local/tslib/lib \
-verbose \
-static
```
