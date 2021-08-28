<a href="https://pan.baidu.com/s/12V6yl21PeEdhKNiYFHKRnw" target="_blank">文件资料</a> 提取码：8n1e

文件清单

```
#安装 JDK 和库文件等
iTOP6818 开发板资料汇总（不含光盘内容）\02_iTOP-6818 开发板编译系统所需要的工具包等\03-tools\linux_tools.tgz
iTOP6818 开发板资料汇总（不含光盘内容）\02_iTOP-6818 开发板编译系统所需要的工具包等\04-JDK以及库文件安装脚本\android_env.tar.gz
iTOP-6818光盘资料\04 源码_Android5.1文件系统\lollipop-5.1.1_r6_20170512.tar
```

放置文件

```sh
linux_tools.tgz、android_env.tar.gz放到ubuntu系统“/”目录下
lollipop-5.1.1_r6_20160801.tar.gz 放 “~/” 目录下
```

编译步骤

```sh
cd /
tar -vxf linux_tools.tgz
tar -vxf android_env.tar.gz

apt-get install -y default-jre default-jdk realpath
cd /android_env/ubuntu/
./install-devel-packages.sh


tar -vxf lollipop-5.1.1_r6_20160801.tar.gz -C ~/;cd ~/lollipop-5.1.1_r6/u-boot
cp nsih-2G16b-6818.txt nsih.txt
cp -r config_for_iTOP6818_android .config
export ARCH=arm
cd ..


#保证给 Ubuntu 系统提供 6G 以上内存
./build_android.sh

#在 result 目录下生成镜像
ls result
```

确保通过 RS232 已经进入 fastboot 模式，接上 OTG 线（即 usb 线）

```
拨码开关
1、3 on
2 off
```

文件清单

```
iTOP-6818光盘资料\03 镜像_Android5.1文件系统
iTOP-6818光盘资料\03 镜像_Android5.1文件系统\2G
```

烧写 android

```sh
fastboot.exe flash uboot u-boot-iTOP-6818.bin
fastboot.exe flash boot boot.img
fastboot.exe flash cache cache.img
fastboot.exe flash system system.img
fastboot.exe flash userdata userdata.img
fastboot.exe flash recovery recovery.img
fastboot reboot

setenv bootsystem android
setenv lcdtype 4.3
saveenv
reset

adb devices
adb push <deviceId> helloworld /data
```
