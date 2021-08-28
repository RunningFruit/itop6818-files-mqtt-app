编译好的最小 linux 镜像
<a href="/itop6818-fastboot-minilinux-custom" target="_blank">itop6818-fastboot-minilinux-custom</a>

<a href="/itop6818-files-minilinux" target="_blank">itop6818-files-minilinux</a>

文件清单

```
arm-2009q3.tar.bz2
busybox-1.21.1.tar.bz2
lollipop-5.1.1_r6_20170512.tar.gz
system_6818_minilinux.tar.gz
```

<a href="https://busybox.net/downloads/busybox-1.21.1.tar.bz2" target="_blank">busybox-1.21.1.tar.bz2</a>

```
cd ~/work
```

<details>
<summary>arm-2009q3.tar.bz2</summary>

```sh
sudo su
wget -c https://sourcery.mentor.com/public/gnu_toolchain/arm-none-linux-gnueabi/arm-2009q3-67-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2
mkdir /usr/local/arm/
tar xvf arm-2009q3.tar.bz2 -C /usr/local/arm/

vim /etc/profile
export PATH=$PATH:/usr/local/arm/arm-2009q3/bin
source /etc/profile

#Ubuntu 命令行中输入命令
arm
```

</details>

<details>
<summary>busybox-1.21.1.tar.bz2</summary>

```sh
sudo apt-get install libncurses5-dev
wget -c https://busybox.net/downloads/busybox-1.21.1.tar.bz2
tar -xvf busybox-1.21.1.tar.bz2;cd busybox-1.21.1

#=====================  配置busybox start  =========================
make menuconfig

（1）选中“Busybox Settings”，
然后按“回车”进入到“Busybox Settings”

（2）第一个是“Build Optiions-> Cross Compiler prefix”,
输入 “arm-none-linux-gnueabi-”，
ok回车返回

（3）使用键盘的“左右”方向按键，
选中“Exit”，
“回车”回到“Busybox Settings”设置界面

（4）选中“Installation Options”，
然后选择“Select”，
按“回车”进入“Installation Options”配置界面

（5）选中“BusyBox installation prefix (NEW)”，
然后按“回车”进入“BusyBox installation prefix”配置界面，
删除“./_install”，
输入“../system”，
按“回车”回到“Installation Options”

（6）键盘“向右”的方向键移动光标到“Exit”，
按“回车”返回到“Busybox Settings”

（7）移动光标到“Exit”，
按“回车”返回到“Busybox Configuration”

（8）移动光标到“Exit”，
按“回车”，弹出保存配置界面，
选择“Yes”，按“回车”保存配置，退出配置界面

make -j 8
make install

cd ~/work/system
ls
mkdir dev etc lib mnt proc sys tmp var

#=====================  配置busybox end  =========================

chmod 755 ~/work/itop6818-files-minilinux/etc/eth0-setting
chmod 755 ~/work/itop6818-files-minilinux/etc/init.d/ifconfig-eth0
chmod 755 ~/work/itop6818-files-minilinux/etc/init.d/rcS
chmod 755 ~/work/itop6818-files-minilinux/etc/passwd
chmod 755 ~/work/itop6818-files-minilinux/etc/profile
chmod 755 ~/work/itop6818-files-minilinux/etc/rc.d/init.d/netd
cp -r ~/work/itop6818-files-minilinux/etc/* ~/work/system/etc

#=====================  lib start  =========================
cp -r /usr/local/arm/arm-2009q3/arm-none-linux-gnueabi/libc/lib/* ~/work/system/lib/
#=====================  lib end  =========================

cd var
mkdir lib lock log run tmp
cd ~/work
```

</details>

<details>
<summary>生成最小linux镜像</summary>

```sh
#rm -r ~/work/lollipop-5.1.1_r6/qt_system/system
#或
mv ~/work/lollipop-5.1.1_r6/qt_system/system ~/work/lollipop-5.1.1_r6/qt_system/system.origin

#tar xvf ~/work/itop6818-files-minilinux/01_*/system_6818_minilinux.tar.gz -C ~/work/lollipop-5.1.1_r6/qt_system/
#或
mv ~/work/system ~/work/lollipop-5.1.1_r6/qt_system/
```

linux_tools.tgz 安装打包软件和编译

```sh
#安装一下make_ext4fs工具
cd /
sudo tar xvf ~/work/itop6818-files-minilinux/linux_tools.tgz

#生成system.img文件系统镜像
cd ~/work/lollipop-5.1.1_r6/qt_system
make_ext4fs -s -l 314572800 -a root -L linux system.img system

ls system.img
zip system.zip system.img
mv system.zip ~/work/itop6818-fastboot-minilinux-custom;cd ~/work/itop6818-fastboot-minilinux-custom
git add *
```

<details>
<summary>手动定制Linux内核</summary>

```sh
export ARCH=arm
cd ~/work/lollipop-5.1.1_r6/kernel
cp -r config_for_iTOP6818_linux .config
make menuconfig

Enabled loadable module support
输入“/”，以进入搜索界面，
输入“leds”，然后按“回车”，查找一下“leds”的驱动，
发现很多配置都是“=n”，通过方向按键，控制向下翻页，然后观察哪个选项配置成了“=y”。


6818 设置默认主机名
General setup --->
	Default hostname


设置编译器
General setup --->
	Cross-compiler tool prefix  # 默认是arm-eabi-
	改为 arm-none-linux-gnueabi-


串口虚拟控制台
Device Drivers --->
	Character devices --->
		Serial drivers --->
			<*> ARM AMBA PL010 serial port support
			[*] Support for console on AMBA serial port

vim .config，搜索关键词“android”，找到CONFIG_CMDLINE；
删除掉“console=ttyAMA0,115200n8”、“androidboot.hardware=drone2”和“androidboot.console=ttyAMA0”，然后保存，内核配置完成


led灯
驱动源码： drivers/char/itop4418_led.c
Device Drivers --->
	Character devices --->
		Enable LEDS config

蜂鸣器 Buzzer
驱动源码： drivers/char/itop4418_buzzer.c
Device Drivers --->
	Character devices --->
		Enable BUZZER config

ADC 数模转换
驱动源码： drivers/staging/iio/adc/nxp_adc.c
Device Drivers --->
	[*] Staging drivers --->
		<*> Industrial I/O support --->
			Analog to digital converters --->
				<*> Analog Devices SLsiAP driver

RTC 实时时钟
驱动源码： drivers/rtc/rtc-nxp.c
Device Drivers --->
	Character devices --->
		Real Time Clock --->
			SLsiAP RTC

RFID/RC522
驱动源码： drivers/spi/rc522.c
General setup --->
	SPI support --->
		RC522 Module driver support

CAN 总线
驱动源码： drivers/net/can/mcp251x.c
Networking support --->
	CAN bus subsystem support--->
		CAN Device Drivers --->
			Microchip MCP251x SPI CAN controllers #选项去掉

USB 转串口 PL2303
驱动源码： drivers/usb/serial/pl2303.c
Device Drivers --->
	USB support --->
		USB Serial Converter support --->
			USB Prolific 2303 Single Port Serial Driver #选上这个选项

USB 转串口 CP210X
驱动源码： drivers/usb/serial/cp210x.c
Device Drivers --->
	USB support --->
		USB Serial Converter support --->
			USB CP210x family of UART Bridge Controller #选上这个选项

RS485
驱动源码： drivers/char/max485_ctl.c
Device Drivers --->
	Character devices --->
		Enable MAX485 config

4.3 寸触摸屏-ts2007
6818 的 4.4 寸电阻触摸屏
驱动源码： drivers/input/touchscreen/tsc2007.c
Device Drivers --->
	Input device support --->
		Touchscreens --->
			TSC2007 based touchscreens

摄像头 ov5640
驱动源码： drivers/media/video/ov5640.c
Device Drivers --->
	Multimedia support --->
		Video capture adapters --->
			Encoders, decoders, sensors and other helper chips --->
				ov5640 DVP camera support

摄像头 AVIN
驱动源码： drivers/media/video/tvp5150.c
Device Drivers --->
	Multimedia support --->
		Video capture adapters --->
			Encoders, decoders, sensors and other helper chips --->
				Texas Instruments TVP5150 video decoder

摄像头 usb-uvc
驱动源码： drivers/media/video/uvc/uvc*
Device Drivers --->
	Multimedia support --->
		Video capture adapters --->
			V4L USB devices --->
				<*> USB Video Class (UVC)
				[*] UVC input events device support

功能按键 keys
驱动源码： drivers/input/keyboard/nxp_io_key.c
Device Drivers --->
	Input device support --->
		Keyboards --->
			<*> SLsiAP push Keypad support

WIFI/BT
驱动源码： drivers/misc/mediatek/combo_mt66xx/* 和 drivers/net/wireless/combo_mt66xx/*

drivers/misc/mediatek/combo_mt66xx/* 部分配置如下
Device Drivers --->
	Misc devices --->
		MTK wireless chip configuration --->
			[*] MediaTek combo_mt66xx Config --->

drivers/net/wireless/combo_mt66xx/* 部分配置如下
Device Drivers --->
	[*] Network device support --->
		[*] Wireless LAN --->
			MTK wireless chip configuration

声卡 sound
驱动源码： sound/soc/nexell/nxp-rt5623.c
Device Drivers --->
	<*> Sound card support --->
		<*> Advanced Linux Sound Architecture --->
			ALSA for SoC audio support --->
				<*> ALC562(123) I2S audio codec.

看门狗 watchdog
驱动源码： drivers/watchdog/nxp_wdt.c
Device Drivers --->
	[*] Watchdog Timer Support --->
		<*> NXP Watchdog
```

</details>

注意用的是 arm-2009q3
最后使用编译 qt 的方法，编译生成的 system.img 就是最小 linux 系统镜像。还需要烧写其它文件,直接使用 qt 的即可，设置也和 qt 一样

```sh
cd ~/work/lollipop-5.1.1_r6/u-boot
cp nsih-2G16b-6818.txt nsih.txt

#cd ~/work/lollipop-5.1.1_r6/kernel
#cp -r config_for_iTOP6818_linux .config
#export ARCH=arm

#如果是 2G 核心板，则打开 kernel 目录下
vim ~/work/lollipop-5.1.1_r6/linux/kernel/kernel-3.4.39/arch/arm/plat-s5p6818/topeet/include/cfg_mem.h
#if 1   //对应 1G 内核
//改为
#if 0   //对应 2G 内核

#config_for_iTOP6818_android 支持 Android 的缺省文件
#config_for_iTOP6818_linux 支持 Qt 的缺省文件

#build_android.sh 默认编译android，选项还有ubuntu、qt、clean

cd ~/work/lollipop-5.1.1_r6
./build_android.sh qt
ls result
#u-boot-iTOP-6818.bin、boot.img、system.img

cd result;
zip u-boot-iTOP-6818.zip u-boot-iTOP-6818.bin;
zip boot.zip boot.img;
mv *.zip ~/work/itop6818-fastboot-minilinux-custom;cd ~/work/itop6818-fastboot-minilinux-custom;
git add *
git commit -m "+"
git push origin master
```

system_6818_minilinux.tar.gz 文件说明

```
“iTOP6818 开发板资料汇总（不含光盘内容）\06_iTOP-6818开发板最小 linux系统源码、镜像以及应用\02_最小linux 镜像\system_6818_minilinux.tar.gz”压缩包，

解压之后，和用户自己生成的 system 文件一模一样，用户如果制作过程中出了问题，可以用压缩包中的文件测试。
```

</details>

helloworld.c

```C
#include <stdio.h>

int main(void){
	printf("hello world");
}
```

编译 helloworld 程序

```sh
arm-none-linux-gnueabi-gcc -o helloworld helloworld.c -static
```

OTG 烧写镜像

```sh
fastboot.exe flash system system.img
fastboot.exe reboot
```

```sh
mkdir /data
#拷贝程序到开发板
chmod 777 /data


#开启允许 USB 调试，fastboot目录下
adb push helloworld /data

#超级终端中，输入命令
cd /data
./helloworld
```

<a href="https://blog.csdn.net/a568713197/article/details/88095014" target="_blank">最小 Linux 系统的搭建、tftp 服务器搭建与测试、搭建 NFS 文件系统</a>
<a href="https://www.oschina.net/question/2371345_2187748" target="_blank">嵌入式开发板学习从零建立 Linux 最小系统</a>
