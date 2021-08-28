程序文件
<a href="/itop6818-programs-QtE5.7" target="_blank">itop6818-programs-QtE5.7</a>

<a href="/itop6818-fastboot-QtE5.7-custom" target="_blank">itop6818-fastboot-QtE5.7-custom</a>

环境、编译、配置相关文件
<a href="/itop6818-files-QtE5.7" target="_blank">itop6818-files-QtE5.7</a>

<details>
<summary>文件清单</summary>

<a href="https://pan.baidu.com/s/12V6yl21PeEdhKNiYFHKRnw" target="_blank">lollipop-5.1.1_r6_20170512.tar.gz</a> 提取码：8n1e

<a href="http://download.qt.io/new_archive/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run" target="_blank">qt-opensource-linux-x64-5.7.0.run</a>
<a href="http://download.qt.io/new_archive/qt/5.7/5.7.0/single/qt-everywhere-opensource-src-5.7.0.tar.gz" target="_blank">qt-everywhere-opensource-src-5.7.0.tar.gz</a>
<a href="http://www.codesourcery.com/sgpp/lite/arm/portal/package4571/public/arm-none-linux-gnueabi/arm-2014.05-29-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2" target="_blank">arm-2014.05-29-arm-none-Linux-gnueabi-i686-pc-linux-gnu.tar.bz2</a>
<a href="https://github.com/libts/tslib/releases/download/1.4/tslib-1.4.tar.bz2" target="_blank">tslib-1.4.tar.bz2</a>

```
linux_tools.tgz 中的 mkimage、make_ext_4fs
```

</details>

<details>
<summary>gcc、编译安装库、32位库</summary>

<a href="https://blog.csdn.net/ZHUQIUSHI123/article/details/90670126" target="_blank">安装 gcc 指定版本</a>

```sh
# 查看ubuntu系统上g++的版本：
ls /usr/bin/g++*

# 安装新版gcc和g++
sudo apt-get install gcc-4.8 g++-4.8

# 切换不同版本
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 20

# 同理修改g++的方式类似：
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 20
```

通过编号选择 gcc 版本

```sh
sudo update-alternatives --config gcc
sudo update-alternatives --config g++
```

```sh
#tar xvf android_env.tgz -C /
#cd /android_env/ubuntu
#./install-devel-packages.sh
#sudo apt-get install -y default-jre default-jdk realpath
sudo apt-get install -y autoconf automake libtool
sudo apt-get install -y lib32c-dev lib32stdc++6 lib32z1 lib32ncurses5
sudo apt-get install -y lzop u-boot-tools make libstdc++6

#Qt Creator报“/usr/bin/ld: error: cannot find -lGL” 问题
sudo apt-get -y install libgl1-mesa-dev
```

</details>

<details>
<summary>uboot、kernel 和 qt 编译</summary>

<a href="/2020/03/31//articles/2020/03/31/1585657878000.html/" target="_blank">itop6818 修改开发板 emmc 用户区大小</a>

```sh
cd ~/work/
git clone /itop6818-fastboot-QtE5.7-custom

tar xvf lollipop-5.1.1_r6_20170512.tar.gz -C ~/work/

#uboot
cd ~/work/lollipop-5.1.1_r6/u-boot
cp nsih-2G16b-6818.txt nsih.txt
cd ~/work/lollipop-5.1.1_r6/kernel
cp -r config_for_iTOP6818_linux .config
export ARCH=arm

#如果是 2G 核心板，则打开 kernel 目录下
vim ~/work/lollipop-5.1.1_r6/linux/kernel/kernel-3.4.39/arch/arm/plat-s5p6818/topeet/include/cfg_mem.h
#if 1   //对应 1G 内核
//改为
#if 0   //对应 2G 内核

cd ..
./build_android.sh qt

ls result
boot.img  sd_fusing_uboot.sh  system.img  u-boot-iTOP-6818.bin
mkuboot   system              u-boot.bin

#其中有用的是
zip boot.zip boot.img
zip u-boot-iTOP-6818.zip u-boot-iTOP-6818.bin

mv boot.img u-boot-iTOP-6818.zip ~/work/itop6818-fastboot-QtE5.7-custom/;
cd ~/work/itop6818-fastboot-QtE5.7-custom/
git add *

# 注意：关于system.img
#android的用result下的system.img
#而qt的是用make_ext4fs后，在目录~/work/lollipop-5.1.1_r6/qt_system下产生的system.img
```

```
Qt 的单独编译可以参考 Android，通过注释 “build_uboot”，“build_kernel” 等来实现。
```

QtE4.7 与 QtE5.7

```sh
# opt文件夹在上述操作中已删除，底下自然也没有了 Qtopia 和 Qt/E4.7 文件夹
ls ~/work/lollipop-5.1.1_r6/qt_system/system/opt


# 在“iTOP-6818开发板之全功能版使用手册_v2.2.pdf”第六章、第七章分别介绍了 Qtopia、Qt/E4.7与Qt/E5.7 的编译

# QtE4.7 需要用到“qt-everywhere-opensource-src-4.7.1.tar.gz”、“arm-linux-gcc-4.3.2.tar.gz”文件，即arm-none-linux-gnueabi-gcc-4.3.2环境

# QtE5.7 必须在 Ubuntu16.04 上编译，需要用到“arm-2014.05-29-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2”、gcc版本4.8
```

安装 arm-none-linux-gnueabi-gcc

```sh
wget -c http://www.codesourcery.com/sgpp/lite/arm/portal/package4571/public/arm-none-linux-gnueabi/arm-2014.05-29-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2
mkdir -p /usr/local/arm
tar jxvf arm-2014.05-29-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2 -C /usr/local/arm/

cat >> /etc/profile <<EOF
export PATH=\$PATH:/usr/local/arm/arm-2014.05/bin
export CROSS_COMPILE=arm-none-linux-gnueabi-
export QT_DEBUG_PLUGINS=1
EOF
source /etc/profile

#检验版本
arm-none-linux-gnueabi-gcc -v
```

</details>

<details>
<summary>tslib-1.4.tar.gz编译和安装，产生/opt/tslib1.4</summary>

<a href="https://blog.csdn.net/lincuiting06/article/details/50352557" target="_blank">tslib-1.4.tar.gz 安装和配置</a>

```sh
sudo apt-get install -y autoconf automake libtool
cd ~/work
wget https://github.com/libts/tslib/releases/download/1.4/tslib-1.4.tar.bz2
tar xvf tslib-1.4.tar.bz2;cd tslib-1.4;
./autogen.sh

./configure \
CC=/usr/local/arm/arm-2014.05/bin/arm-none-linux-gnueabi-gcc \
CXX=/usr/local/arm/arm-2014.05/bin/arm-none-linux-gnueabi-g++ \
--host=arm-none-linux-gnueabi \
--enable-static=yes \
--enable-shared=yes \
--prefix=/opt/tslib1.4 ac_cv_func_malloc_0_nonnull=yes

make -j 8;sudo make install

#修改ts.conf
sudo vim /opt/tslib1.4/etc/ts.conf
#第一行
module_raw input

#追加宏：
sudo vim /opt/tslib1.4/include/tslib.h
#define SYN_MT_REPORT           2
#define ABS_MT_SLOT             0x2f    /* MT slot being modified */
#define ABS_MT_TOUCH_MAJOR      0x30    /* Major axis of touching ellipse */
#define ABS_MT_TOUCH_MINOR      0x31    /* Minor axis (omit if circular) */
#define ABS_MT_WIDTH_MAJOR      0x32    /* Major axis of approaching ellipse */
#define ABS_MT_WIDTH_MINOR      0x33    /* Minor axis (omit if circular) */
#define ABS_MT_ORIENTATION      0x34    /* Ellipse orientation */
#define ABS_MT_POSITION_X       0x35    /* Center X touch position */
#define ABS_MT_POSITION_Y       0x36    /* Center Y touch position */
#define ABS_MT_TOOL_TYPE        0x37    /* Type of touching device */
#define ABS_MT_BLOB_ID          0x38    /* Group a set of packets as a blob */
#define ABS_MT_TRACKING_ID      0x39    /* Unique ID of initiated contact */
#define ABS_MT_PRESSURE         0x3a    /* Pressure on contact area */
#define ABS_MT_DISTANCE         0x3b    /* Contact hover distance */
#define ABS_MT_TOOL_X           0x3c    /* Center X tool position */
#define ABS_MT_TOOL_Y           0x3d    /* Center Y tool position */
```

</details>

<details>
<summary>修改qmake.conf，用于指定arm-none-linux-gnueabi-gcc交叉编译环境</summary>

解压

```sh
cd ~/work
wget http://download.qt.io/new_archive/qt/5.7/5.7.0/single/qt-everywhere-opensource-src-5.7.0.tar.gz
tar xvf qt-everywhere-opensource-src-5.7.0.tar.gz;cd qt-everywhere-opensource-src-5.7.0
```

替换

```sh
sed -i "s/arm-linux-gnueabi-/arm-none-linux-gnueabi-/g" qtbase/mkspecs/linux-arm-gnueabi-g++/qmake.conf
```

qtbase/mkspecs/linux-arm-gnueabi-g++/qmake.conf

```
## qmake configuration for building with arm-linux-gnueabi-g++
#
MAKEFILE_GENERATOR = UNIX
CONFIG += incremental
QMAKE_INCREMENTAL_STYLE = sublib
#QT_QPA_DEFAULT_PLATFORM = linux #eglfs
QT_QPA_DEFAULT_PLATFORM = linuxfb

QMAKE_CFLAGS_RELEASE += -O2 -march=armv7-a
QMAKE_CXXFLAGS_RELEASE += -O2 -march=armv7-a

include(../common/linux.conf)
include(../common/gcc-base-unix.conf)
include(../common/g++-unix.conf)

# modifications to g++.conf
QMAKE_CC = arm-none-linux-gnueabi-gcc
QMAKE_CXX = arm-none-linux-gnueabi-g++
QMAKE_LINK = arm-none-linux-gnueabi-g++
QMAKE_LINK_SHLIB = arm-none-linux-gnueabi-g++

QMAKE_LFLAGS = -static

# modifications to linux.conf
QMAKE_AR = arm-none-linux-gnueabi-ar cqs
QMAKE_OBJCOPY = arm-none-linux-gnueabi-objcopy
QMAKE_NM = arm-none-linux-gnueabi-nm -P
QMAKE_STRIP = arm-none-linux-gnueabi-strip
load(qt_config)
```

</details>

<details>
<summary>执行脚本文件autoconfigure.sh编译QtE5.7源码，产生/opt/qt5.7.0</summary>

```sh
vim /etc/profile

export PATH=$PATH:/usr/local/arm/arm-2014.05/bin
export CROSS_COMPILE=arm-none-linux-gnueabi-

source /etc/profile
```

qt-everywhere-opensource-src-5.7.0/autoconfigure.sh

```sh
#!/bin/sh
./configure \
-v \
-static \
-confirm-license \
-prefix /opt/qt5.7.0 \
-release \
-opensource \
-make libs \
-xplatform linux-arm-gnueabi-g++ \
-optimized-qmake \
-pch \
-qt-sql-sqlite \
-qt-libjpeg \
-qt-zlib \
-qt-xcb \
-no-opengl \
-skip qt3d \
-skip qtcanvas3d \
-skip qtpurchasing \
-no-sse2 \
-no-openssl \
-no-nis \
-no-cups \
-no-glib \
-no-iconv \
-nomake examples \
-nomake tools \
-skip qtvirtualkeyboard \
-I/opt/tslib1.4/include \
-L/opt/tslib1.4/lib

exit
```

执行 autoconfigure.sh

```sh
cd ~/work/qt-everywhere-opensource-src-5.7.0
apt-get install -y python
chmod 777 autoconfigure.sh
./autoconfigure.sh
make -j 8
make install
ls /opt/qt5.7.0
```

</details>

<details>
<summary>生成 QtE5.7 镜像 system.img</summary>

解压 mkimage、make_ext_4fs

```sh
sudo tar -vxf ~/work/itop6818-files-QtE5.7/linux_tools.tgz -C /
ls /usr/local/bin/
```

放置 qt5.7.0、tslib1.4

```sh
cd ~/work/lollipop-5.1.1_r6/qt_system/system

mkdir opt/qt5.7.0/ opt/tslib1.4/

cp -r /opt/qt5.7.0/* opt/qt5.7.0/
cp -r /opt/tslib1.4/* opt/tslib1.4/

rm -rf opt/qt-4.7.1
rm -rf opt/Qtopia
```

修改环境变量

```sh
#mkdir -p ~/work/lollipop-5.1.1_r6/qt_system/system/etc
vim ~/work/lollipop-5.1.1_r6/qt_system/system/etc/profile

# Ash profile
# vim: syntax=sh
# No core files by default
ulimit -S -c 0 > /dev/null 2>&1
USER="`id -un`"
LOGNAME=$USER
PS1='[$USER@$HOSTNAME]# '
PATH=$PATH
HOSTNAME=`/bin/hostname`
export USER LOGNAME PS1 PATH
export TSLIB_ROOT=/opt/tslib1.4
export QT_ROOT=/opt/qt5.7.0
export TSLIB_TSDEVICE=/dev/input/event2
export TSLIB_TSEVENTTYPE=input
export TSLIB_CONFFILE=/opt/tslib1.4/etc/ts.conf
export TSLIB_PLUGINDIR=/opt/tslib1.4/lib/ts
export TSLIB_CONSOLEDEVICE=none
export TSLIB_FBDEVICE=/dev/fb0
export QWS_MOUSE_PROTO=tslib:/dev/input/event2
export LD_LIBRARY_PATH=/lib:/usr/lib:/usr/local/lib:$QT_ROOT/lib:$QT_ROOT/plugins/imageformats:$QT_ROOT/plugins/platforms:$TSLIB_ROOT/lib
export QT_QPA_PLATFORM_PLUGIN_PATH=$QT_ROOT/plugins
export QT_QPA_PLATFORM=linuxfb:tty=/dev/fb0
export QT_QPA_FONTDIR=$QT_ROOT/lib/fonts
export QT_QPA_GENERIC_PLUGINS=tslib
```

修改开机启动脚本，并放置 qt5.7 到/bin/qt5.7

```sh
#mkdir -p ~/work/lollipop-5.1.1_r6/qt_system/system/etc/init.d/
vim ~/work/lollipop-5.1.1_r6/qt_system/system/etc/init.d/rcS
#/bin/qt4 &
/bin/qt5.7 &

#将迅为提供的 “qt5.7” 文件拷贝到 “bin” 目录下，并修改 qt5.7 的权限为 777
cp ~/work/itop6818-files-QtE5.7/qt5.7 ~/work/lollipop-5.1.1_r6/qt_system/system/bin/
cd ~/work/lollipop-5.1.1_r6/qt_system/system/bin/
chmod 777 qt5.7
```

打包 qt 系统镜像
<a href="/2020/03/31//articles/2020/03/31/1585657878000.html/" target="_blank">itop6818 修改开发板 emmc 用户区大小</a>

```sh
cd ~/work/lollipop-5.1.1_r6/qt_system

# du -h --max-depth=1
# 6.2M	./system.linux
# 738M	./linux-x86
# 12M	./boot
# 495M	./system
# 1.4G	.
# make_ext4fs -s -l 500M -a root -L linux system.img system


# 1024 * 1024 * 300 = 314572800
# make_ext4fs -s -l 314572800 -a root -L linux system.img system
# make_ext4fs -s -l 300M -a root -L linux system.img system


# 1024 * 1024 * 1024 * 13 = 13958643712
# make_ext4fs -s -l 13958643712 -a root -L linux system.img system
make_ext4fs -s -l 13G -a root -L linux system.img system

zip system.zip system.img
mv system.zip ~/work/itop6818-fastboot-QtE5.7-custom/;
cd ~/work/itop6818-fastboot-QtE5.7-custom/
git add *
git commit -m "* 13G"
git push origin master

xdg-open .

xdg-open ~/work/lollipop-5.1.1_r6/qt_system
xdg-open ~/work/lollipop-5.1.1_r6/result
```

</details>

<details>
<summary>开发板上的基本配置</summary>

<a href="/2020/03/07//articles/2020/03/07/1583547575000.html/" target="_blank">QtE 系统 TF 卡、SD 卡热插拔</a>

wifi 上网

```sh
wpa_passphrase XXX "YYY" > /etc/wpa_supplicant.conf
/etc/init.d/mt6620 &

# 开机连wifi
vi /etc/init.d/rcS
/etc/init.d/mt6620 &

ifconfig
```

4G 手机卡上网

```sh
# 先插上手机卡
/etc/ppp/peers/netec20

# 详细
pppd call wcdma &
echo "nameserver 111.11.1.3" >> /etc/resolv.conf
route add default gw 10.14.165.1
```

测试上网

```sh
ping www.baidu.com -c 4
```

</details>

<details>
<summary>ubuntu16虚拟机上的qt5.7开发工具、qt5.7的arm交叉编译环境配置、编译测试程序</summary>

开发工具<a href="http://download.qt.io/new_archive/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run" target="_blank">qt-opensource-linux-x64-5.7.0.run</a>

<a href="/2020/03/26/articles/2020/03/26/1585230859000.html/" target="_blank">[arm] qt5.7 配置 arm 开发环境</a>

拉取程序

```sh
cd ~/work
git clone /itop6818-programs-QtE5.7
cd ~/work/itop6818-programs-QtE5.7
```

使用/opt/qt5.7.0/bin/qmake 生成 arm 程序

```sh
cd tuixiangzi
/opt/qt5.7.0/bin/qmake
make
file tuixiangzi

#显示以下信息
tuixiangzi: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-, for GNU/Linux 2.6.16, not stripped
```

U 盘挂载、运行 leds 程序

```sh
#创建目录用于挂载
mkdir /mnt/udisk/
#挂载U盘
mount /dev/sda1 /mnt/udisk/
cd /mnt/udisk/

chmod 777 leds
#运行
leds -qws
```

tf 卡挂载、运行 helloworld 程序

```sh
#创建目录用于挂载
mdkir /mnt/sdcard/
#挂载tf卡
mount /dev/mmcblk0p1 /mnt/sdcard/
cd /mnt/sdcard/

#可执行
chmod 777 helloworld
#运行
helloworld -qws
```

</details>

<details>
<summary>在ubuntu16虚拟机上 或 在移植了qt5.7的itop6818开发板终端编译执行helloworld qt程序</summary>

```sh
chmod a+x /opt/qt5.7.0/bin/qmake

mkdir helloworld;cd helloworld

cat > main.cpp <<EOF
#include <QCoreApplication>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
}
EOF

cat > helloworld.pro <<EOF
QT += core
QT -= gui

CONFIG += c++11

TARGET = helloworld
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp
EOF


/opt/qt5.7.0/bin/qmake
ls
#多了一个Makefile

make

#查看文件信息
file helloworld
```

</details>

<details>
<summary>终端执行qt程序遇见的问题</summary>

```
error while loading shared libraries: libQt5Widgets.so.5: cannot open shared object file: No such file or directory
```

虚拟机上查找 libQt5Widgets.so.5

```
locate libQt5Widgets.so.5

#所在目录
/opt/qt5.7.0/lib/libQt5Widgets.so.5
```

</details>

<details>
<summary>在ubuntu16虚拟机上用qt工具交叉编译程序遇见的问题</summary>

编译 qt 程序碰见的问题：

```
error while loading shared libraries: libXXX.so.X: cannot open shared object file: No such file
```

解决方法 1：

```sh
sudo vim /etc/ld.so.conf
# 在新的一行中加入库文件所在目录
/usr/lib
/opt/qt5.7.0/plugins/imageformats
/opt/qt5.7.0/plugins/platforms
/opt/tslib1.4/lib

# 更新/etc/ld.so.cache文件
sudo ldconfig
```

解决方法 2：

```sh
# 1.将用户用到的库统一放到一个目录，如 /usr/local/lib
cp libXXX.so.X /usr/local/lib/

# 2.向库配置文件中，写入库文件所在目录
vim /etc/ld.so.conf.d/usr-libs.conf
/usr/local/lib

# 3.更新/etc/ld.so.cache文件
sudo ldconfig
```

解决方法 3：

```sh
vim /etc/profile

export TSLIB_ROOT=/opt/tslib1.4
export QT_ROOT=/opt/qt5.7.0
export TSLIB_TSDEVICE=/dev/input/event2
export TSLIB_TSEVENTTYPE=input
export TSLIB_CONFFILE=/opt/tslib1.4/etc/ts.conf
export TSLIB_PLUGINDIR=/opt/tslib1.4/lib/ts
export TSLIB_CONSOLEDEVICE=none
export TSLIB_FBDEVICE=/dev/fb0
export QWS_MOUSE_PROTO=tslib:/dev/input/event2
export LD_LIBRARY_PATH=/lib:/usr/lib:/usr/local/lib:$QT_ROOT/lib:$QT_ROOT/plugins/imageformats:$QT_ROOT/plugins/platforms:$TSLIB_ROOT/lib
export QT_QPA_PLATFORM_PLUGIN_PATH=$QT_ROOT/plugins
export QT_QPA_PLATFORM=linuxfb:tty=/dev/fb0
export QT_QPA_FONTDIR=$QT_ROOT/lib/fonts
export QT_QPA_GENERIC_PLUGINS=tslib
```

解决方法 4，直接拷贝 shared object file 到输出目录：

```sh
locate libts-1.4.so.0|grep tslib1.4
ln -s /opt/tslib1.4/lib/libts-1.4.so.0 libts-1.4.so.0
```

<a href="https://blog.csdn.net/lusanshui/article/details/84988017" target="_blank">Qt 平台插件问题 1：</a>

```
This application failed to start because it could not find or load the Qt platform plugin "xcb"
in "".
```

<a href="https://blog.csdn.net/sinat_26106275/article/details/82778951" target="_blank">关于 qt 缺少 xcb 问题终极解决办法:</a>

```
export QT_DEBUG_PLUGINS=1
```

解决办法 1，执行程序时追加-platform 与值：

```sh
locate libqxcb.so
ldd libqxcb.so
locate libQt5XcbQpa.so.5
locate libQt5XcbQpa.so.5.7.0

cd /opt/qt5.7.0/plugins/platforms/
#cd /usr/lib/x86_64-linux-gnu
ln -s $PWD/libQt5XcbQpa.so.5.7.0 libQt5XcbQpa.so.5
ln -s /usr/lib/x86_64-linux-gnu/libQt5XcbQpa.so.5.7.0 /opt/qt5.7.0_static/plugins/platforms/libQt5XcbQpa.so.5

cd /opt/qt5.7.0/lib/
ln -s $PWD/libQt5DBus.so.5.7.0 libQt5DBus.so.5


ls /opt/qt5.7.0/plugins/platforms/
libqlinuxfb.a    libqminimal.a    libqoffscreen.a
libqlinuxfb.prl  libqminimal.prl  libqoffscreen.prl

# 解决方式是后面明确带platform参数
./qtest -platform linuxfb
```

解决办法 2，修改 qt 工程的 pro 文件配置

```sh
CONFIG += static
LIBS += -L/opt/qt5.7.0/plugins/platforms -lqlinuxfb -lqminimal -lqoffscreen
LIBS += /opt/tslib1.4/lib/libts-1.4.so.0
```

Qt 平台插件问题 2：

```
This application failed to start because it could not find or load the Qt platform plugin "linux #eglfs"
in "".

Available platform plugins are: linuxfb, minimal, offscreen.

Reinstalling the application may fix this problem.
Aborted
```

解决办法 1，修改 LD_LIBRARY_PATH：

```sh
# /etc/profile结尾追加：

export QT_ROOT=/opt/qt5.7.0/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$QT_ROOT/plugins/imageformats:$QT_ROOT/plugins/platforms

source /etc/profile
```

解决办法 2，在 qmake.conf 中修改“linux #eglfs”为“linuxfb”：

```sh
qtbase/mkspecs/linux-arm-gnueabi-g++/qmake.conf

## qmake configuration for building with arm-linux-gnueabi-g++
#
MAKEFILE_GENERATOR = UNIX
CONFIG += incremental
QMAKE_INCREMENTAL_STYLE = sublib
QT_QPA_DEFAULT_PLATFORM = linuxfb
```

</details>

<details>
<summary>其它</summary>

可能会导致是 64 位的
libstdc++将放至 system 的/opt/qt5.7.0/lib/目录

```
sudo apt-get install -y lib32c-dev lib32stdc++6

cd /usr/local/arm/arm-2014.05/arm-none-linux-gnueabi/libc/usr/lib/

cp libstdc++.a /opt/qt5.7.0/lib/
cp libstdc++.so /opt/qt5.7.0/lib/
cp libstdc++.so.6.0.19 /opt/qt5.7.0/lib/

cd /opt/qt5.7.0/lib/
ln -s libstdc++.so.6.0.19 libstdc++.so.6
```

glibc2.9 将放至 system 的/lib 目录

```
cd ~/work/lollipop-5.1.1_r6/qt_system/system/lib/
# libc.so.6含有glibc2.9
strings libc.so.6 | grep GLIBC_2.9

mv libc.so.6 libc.so.6.bak
cp -r /usr/local/arm/arm-2014.05/arm-none-linux-gnueabi/libc/lib/* .
```

<a href="https://blog.csdn.net/qq_38639612/article/details/90646791" target="_blank">Linux 下 QT 开发平台的安装和交叉编译移植</a>
<a href="https://blog.csdn.net/wangliang0633/article/details/77217424" target="_blank">Qt5.7 移植到 ARM 开发板</a>

</details>
