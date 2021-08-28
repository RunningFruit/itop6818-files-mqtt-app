确保通过 RS232 已经进入 fastboot 模式，接上 OTG 线（即 usb 线）

```
拨码开关
1、3 on
2 off
```

<details>
<summary>烧写android</summary>

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

</details>

<details>
<summary>烧写qt</summary>

```sh
fastboot.exe flash uboot u-boot-iTOP-6818.bin
fastboot.exe flash boot boot.img
fastboot.exe flash system system.img
fastboot reboot

setenv bootsystem qt
setenv lcdtype 4.3
saveenv
reset
```

</details>

【注意】烧写 Uboot 到 TF 卡，TF 卡文件格式一定要是 FAT32

```sh
#查看盘符
df -l
```

<details>
<summary>TF 卡烧写 Android</summary>

一、烧写 Uboot 到 TF 卡

```sh
#查看盘符
df -l

#如果没有 uboot 镜像则拷贝“光盘目录\03 镜像_Android5.1文件系统\u-boot-iTOP-6818.bin”到这个目录下
ls lollipop-5.1.1_r6/result/
mkuboot u-boot-iTOP-6818.bin

./mkuboot /dev/sdx
```

二、拨码开关设置为 TF 卡启动

```
将 TF 卡接入开发板，将拨码开关设置为 TF 卡启动，进入 uboot 模式
```

三、

```sh
TF 卡中建立目录“sdupdate”，sdupdate中放置boot.img、cache.img、system.img、u-boot-iTOP-6818.bin、userdata.img
#一次性烧写
sdfuse flashall

#分别烧写
#单独烧写 uboot 镜像
sdfuse flash uboot u-boot-iTOP-6818.bin
#单独烧写内核镜像
sdfuse flash boot boot.img
#单独烧写 Android 文件系统镜像
sdfuse flash cache cache.img
sdfuse flash system system.img
sdfuse flash userdata userdata.img

setenv bootsystem android
setenv lcdtype 4.3
#设置参数和命令
saveenv

```

四、设置拨码开关为 emmc 启动，取下 tf 卡

```sh
reset
```

</details>

<details>
<summary>TF 卡烧写 Qt</summary>

```sh
TF 卡中建立目录“sdupdate”，sdupdate中放置boot.img、system.img、u-boot-iTOP-6818.bin
#一次性烧写
sdfuse flashall

#分别烧写----
#单独烧写 uboot 镜像
sdfuse flash uboot u-boot-iTOP-6818.bin
#单独烧写内核镜像
sdfuse flash boot boot.img
#单独烧写 Android 文件系统镜像
sdfuse flash system system.img

setenv bootsystem qt
setenv lcdtype 4.3
#设置参数和命令
saveenv
#设置拨码开关为 emmc 启动，取下 tf 卡
reset
```

</details>

<details>
<summary>查看设备与执行程序</summary>

```sh
ls /dev/u*
ls /dev/mm*

mkdir /mnt/udisk
mount /dev/udisk /mnt/udisk/
ls /mnt/udisk/
chmod 777 helloworld
helloworld -qws
```

</details>
