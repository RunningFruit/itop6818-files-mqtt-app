<a href="/itop6818-files-QtE5.7" target="_blank">itop6818-files-QtE5.7</a>

<details>
<summary>安装编译器</summary>

```
sudo apt-get install lib32z1 lib32ncurses5

wget -c http://www.codesourcery.com/sgpp/lite/arm/portal/package4571/public/arm-none-linux-gnueabi/arm-2014.05-29-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2
mkdir -p /usr/local/arm
tar jxvf arm-2014.05-29-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2 -C /usr/local/arm/

#编辑.bashrc
vim /etc/profile
export PATH=$PATH:/usr/local/arm/arm-2014.05/bin
export CROSS_COMPILE=arm-none-linux-gnueabi-
source /etc/profile

#检验版本
arm-none-linux-gnueabi-gcc -v
```

</details>

<details>
<summary>itop6818-files-QtE5.7中的qt5.7+tslib1.4</summary>

```
tar xvf itop6818-files-QtE5.7/qt5.7+tslib1.4/qt5.7.tar.gz
rm -rf /opt/qt5.7.0
mv qt-5.7.0 /opt/qt5.7.0

tar xvf itop6818-files-QtE5.7/qt5.7+tslib1.4/tslib1.4.tar.gz
rm -rf /opt/tslib1.4
mv tslib1.4 /opt/tslib1.4

unzip itop6818-files-QtE5.7/fonts.zip
mv fonts /opt/qt5.7.0/lib/

cd /usr/local/arm/arm-2014.05/arm-none-linux-gnueabi/libc/usr/lib/

cp libstdc++.a /opt/qt5.7.0/lib/
cp libstdc++.so /opt/qt5.7.0/lib/
cp libstdc++.so.6.0.19 /opt/qt5.7.0/lib/

cd /opt/qt5.7.0/lib/
ln -s libstdc++.so.6.0.19 libstdc++.so.6

sudo apt-get install -y lib32c-dev lib32stdc++6
```

</details>

<details>
<summary>解压源码 tslib-1.4.tar.gz</summary>

```
sudo apt-get install -y autoconf automake libtool

git clone /itop6818-files-QtE5.7;cd itop6818-files-QtE5.7
tar xvf tslib-1.4.tar.gz;cd tslib
./autogen.sh
./configure CC=arm-none-linux-gnueabi-gcc CXX=arm-none-linux-gnueabi-g++ --host=arm-none-linux-gnueabi --prefix=/opt/tslib1.4 ac_cv_func_malloc_0_nonnull=yes
make
sudo make install

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
<summary>安装gcc指定版本</summary>

<a href="https://blog.csdn.net/ZHUQIUSHI123/article/details/90670126" target="_blank">安装 gcc 指定版本</a>

```
# 查看ubuntu系统上g++的版本：
ls /usr/bin/g++*

# 安装新版gcc和g++
sudo apt-get install gcc-4.8 g++-4.8

# 切换不同版本
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 20

# 同理修改g++的方式类似：
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 20
```

据说 gcc6 版本才支持 c++11

```
sudo apt-get update && \
sudo apt-get install build-essential software-properties-common -y && \
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
sudo apt-get update && \
sudo apt-get install gcc-snapshot -y && \
sudo apt-get update && \
sudo apt-get install gcc-6 g++-6 -y && \
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 60 --slave /usr/bin/g++ g++ /usr/bin/g++-6
```

</details>

<details>
<summary>qmake.conf</summary>

qt-everywhere-opensource-src-5.7.0

```
cd ~/
wget http://download.qt.io/new_archive/qt/5.7/5.7.0/single/qt-everywhere-opensource-src-5.7.0.tar.gz
tar xvf qt-everywhere-opensource-src-5.7.0.tar.gz;cd qt-everywhere-opensource-src-5.7.0

vim qtbase/mkspecs/linux-arm-gnueabi-g++/qmake.conf
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

# modifications to linux.conf
QMAKE_AR = arm-none-linux-gnueabi-ar cqs
QMAKE_OBJCOPY = arm-none-linux-gnueabi-objcopy
QMAKE_NM = arm-none-linux-gnueabi-nm -P
QMAKE_STRIP = arm-none-linux-gnueabi-strip

QMAKE_LFLAGS = -static
QMAKE_LFLAGS_DLL = -static

load(qt_config)
```

</details>

<details>
<summary>编译 QtE5.7 源码 之 autoconfigure.sh</summary>

```
source /etc/profile
```

```
sed -i 's/\r$//' configure
```

qt-everywhere-opensource-src-5.7.0/autoconfigure.sh

```
#!/bin/sh
./configure \
-v \
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

```sh
apt-get install -y python
chmod 777 autoconfigure.sh
sudo sh autoconfigure.sh
make -j 16
make install
ls /opt/qt5.7.0

cd /usr/local/arm/arm-2014.05/arm-none-linux-gnueabi/libc/usr/lib/

cp libstdc++.a /opt/qt5.7.0/lib/
cp libstdc++.so /opt/qt5.7.0/lib/
cp libstdc++.so.6.0.19 /opt/qt5.7.0/lib/
```

</details>

<details>
<summary>修改开发板环境:vi /etc/profile</summary>

```
export TSLIB_ROOT=/opt/tslib1.4
export QT_ROOT=/opt/qt5.7.0

export TSLIB_TSDEVICE=/dev/input/event2
export TSLIB_TSEVENTTYPE=input
export TSLIB_CONFFILE=/opt/tslib1.4/etc/ts.conf
export TSLIB_PLUGINDIR=/opt/tslib1.4/lib/ts
export TSLIB_CONSOLEDEVICE=none
export TSLIB_FBDEVICE=/dev/fb0

export QWS_MOUSE_PROTO=tslib:/dev/input/event2
export LD_LIBRARY_PATH=/lib:/usr/lib:/usr/local/lib:$QT_ROOT/lib:$TSLIB_ROOT/lib:$TSLIB_ROOT/lib/

export QT_QPA_PLATFORM_PLUGIN_PATH=$QT_ROOT/plugins
export QT_QPA_PLATFORM=linuxfb:tty=/dev/fb0
export QT_QPA_FONTDIR=$QT_ROOT/lib/fonts
export QT_QPA_GENERIC_PLUGINS=tslib
```

</details>

开发工具<a href="http://download.qt.io/new_archive/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run" target="_blank">qt-opensource-linux-x64-5.7.0.run</a>

<details>
<summary>qtcreator配置arm</summary>

<a href="https://blog.csdn.net/u012902367/article/details/87876079" target="_blank">~会飞的蜗牛~ Linux 下的 Qt 环境搭建(arm 交叉编译）</a>
<a href="https://www.cnblogs.com/emouse/archive/2013/01/29/2881721.html" target="_blank">qtcreator 配置 arm</a>

</details>

<details>
<summary>linux下编译qt静态库</summary>

<a href="https://www.cnblogs.com/oloroso/p/5407779.html" target="_blank">linux 下编译 qt5.6.0 静态库 — configure 配置</a>
<a href="https://blog.csdn.net/earbao/article/details/53860574" target="_blank">Linux 下快速静态编译 Qt 以及 Qt 动态/静态版本共存</a>

```
./configure \
-y \
-static \
-release \
-confirm-license \
-opensource \
-no-xcb  \
-qt-zlib  \
-qt-libpng \
-qt-libjpeg \
-nomake examples \
-qt-sql-sqlite \
-prefix /opt/qt5.7.0

./configure \
-v \
–static \
–release–c++11 \
–qt-zlib  \
–qt-libpng \
–qt-libjpeg \
–nomakeexamples \
–qt-sql-sqlite \
–no-icu \
–prefix /opt/qt5.7.0
```

<a href="https://blog.csdn.net/wubw0585/article/details/87828159" target="_blank">Linux Qt-5.12.1 静态编译</a>

```
./configure \
-v \
-static \
-release \
-prefix /opt/qt5.7.0 \
-qt-zlib \
-qt-libpng \
-qt-libjpeg \
-nomake examples \
-nomake tests \
-no-opengl \
-skip qttools \
-skip qtserialport \
-skip qtsensors \
-skip qtconnectivity \
-skip qtmultimedia

# (1)指定静态库的路径
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/Qt-5.12.1_static/lib
后面的Qt静态库的路径根据实际情况改变

# (2)指定默认qmake
#将静态库的qmake拷贝到/usr/bin，然后检测该qmake是否是静态库的qmake路径
qmake -v
```

<a href="https://www.cnblogs.com/longxi/p/9913903.html" target="_blank">Ubuntu14.04 + Qt5.7.1（静态编译）</a>

```
./configure \
-v \
-static \
-debug-and-release \
-prefix /opt/qt5.7.0 \
-fontconfig \
-qt-sql-sqlite \
-qt-zlib \
-qt-libpng \
-qt-libjpeg \
-qt-freetype \
-qt-xcb \
-skip qtwebkit \
-nomake tests \
-nomake examples
```

</details>
