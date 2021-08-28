<a href="/itop6818-files-mt6620-4g" target="_blank">itop6818-files-mt6620-4g</a>

移植 OpenSSL

```sh
wget --no-check-certificate http://www.openssl.org/source/openssl-1.1.0g.tar.gz
tar xvf openssl-1.1.0g.tar.gz
cd openssl-1.1.0g
./config no-asm shared --prefix=$(pwd)/__install
```

vim Makefile

```sh
搜索 CFLAG
删除上图中红框中的“-m64”

SHARED_LIBS=libcrypto.so libssl.so
SHLIB_TARGET=linux-shared

找到 CFLAG 变量，在变量的最后加上：-DOPENSSL_NO_STATIC_ENGINE 保存退出
```

```sh
make CROSS_COMPILE=/usr/local/arm/4.3.2/bin/arm-none-linux-gnueabi-
make install
```

```sh
mkdir ~/work
cp -r __install/lib ~/work
```

移植 libnl

```sh
wget --no-check-certificate https://github.com/tgraf/libnl-1.1-stable/archive/libnl1_1_4.tar.gz
tar xvf libnl1_1_4.tar.gz;mv libnl-1.1-stable-libnl1_1_4 libnl-1.1.4;cd libnl-1.1.4
./configure --prefix=$(pwd)/__install --enable-shared --enable-static
make CC=/usr/local/arm/4.3.2/bin/arm-none-linux-gnueabi-gcc
make install
```

libnl 下的 lib 文件夹

```sh
cp -r __install/lib ~/work
```

移植 wpa_supplicant

```sh
wget http://w1.fi/releases/wpa_supplicant-2.9.tar.gz
tar xvf wpa_supplicant-2.9.tar.gz;cd wpa_supplicant-2.9/wpa_supplicant;
```

```sh
cp defconfig .config

vim Makefile

将ifndef CC CC=gcc endif 修改为：

CFLAGS += -I../../libnl-1.1.4/__install/include/
CFLAGS += -I../../openssl-1.1.0g/__install/include/
LIBS += -L../../libnl-1.1.4/__install/lib/
LIBS += -L../../openssl-1.1.0g/__install/lib/
#ifndef CC
CC=/usr/local/arm/4.3.2/bin/arm-none-linux-gnueabi-gcc
#endif

make
ls wpa_supplicant
#将编译好的 wpa_supplicant 工具拷贝到开发板上的“/usr/sbin”目录即可
```

```
复制 ~/lollipop-5.1.1_r6/qt_system/system/lib、~/lollipop-5.1.1_r6/qt_system/system/usr/lib、~/work/lib 文件夹中的库文件到开发板的 /lib 目录

复制 ~/lollipop-5.1.1_r6/qt_system/system/sbin、~/lollipop-5.1.1_r6/qt_system/system/usr/sbin 文件夹中的文件到开发板的 /usr/sbin 目录

复制 ls ~/lollipop-5.1.1_r6/qt_system/system/usr/bin/6620_launcher 到开发板的 /usr/bin 目录下

复制 ~/lollipop-5.1.1_r6/qt_system/system/lib/modules/mt6620/ 中的所有文件到 /lib/modules/mt6620/ 目录

复制 make_mt6620.sh 到开发板的任意目录
```

开发板连接 WiFi

```sh
#在开发板执行下面的指令，更新 WiFi开机启动脚本

cat << EOF > /etc/init.d/mt6620
#!/bin/sh
#support MT6620 WIFI Module
mknod /dev/stpwmt c 190 0
mknod /dev/stpgps c 191 0
mknod /dev/fm c 193 0
mknod /dev/wmtWifi c 194 0

insmod /lib/modules/mt6620/mtk_hif_sdio.ko
insmod /lib/modules/mt6620/mtk_stp_wmt.ko
insmod /lib/modules/mt6620/mtk_stp_uart.ko
insmod /lib/modules/mt6620/mtk_stp_gps.ko
#insmod /lib/modules/mt6620/hci_stp.ko
#insmod /lib/modules/mt6620/mt6620_fm_drv.ko
#insmod /lib/modules/mt6620/mtk_fm_priv.ko
insmod /lib/modules/mt6620/mtk_wmt_wifi.ko WIFI_major=194
insmod /lib/modules/mt6620/wlan_mt6620.ko

chmod 0666 /dev/stpwmt
chmod 0666 /dev/stpgps
chmod 0666 /dev/fm
chmod 0666 /dev/wmtWifi
chmod 0666 /dev/gps
chmod 0660 /dev/ttySAC2

/usr/bin/6620_launcher -m 1 -b 921600 -n /etc/firmware/mt6620_patch_hdr.bin -d /dev/ttySAC2 &

sleep 4
echo 1 > /dev/wmtWifi

wpa_supplicant -iwlan0 -Dnl80211 -c/etc/wpa_supplicant.conf &
sleep 3
udhcpc -i wlan0 >/var/udhcpc_log &
EOF
```

或者在开发板上执行

```sh
make_mt6620.sh
```

然后执行以下指令

```sh
wpa_passphrase XXX "YYY" > /etc/wpa_supplicant.conf

#执行命令“ ./etc/init.d/mt6620 ”即可连接到 WiFi 网络，配置时间大约 30 秒左右
./etc/init.d/mt6620
```
