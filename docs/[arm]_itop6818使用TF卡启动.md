烧写 Uboot 到 TF 卡

```sh
#将 TF 卡从 windows 接入 Ubuntu

#查看盘符
df -l

cd /home/6818/lollipop-5.1.1_r6/result
ls u-boot-Top-6818.bin

./mkuboot /dev/sdx
```

TF 卡引导 uboot

```
拨码开关编号 1 2 3
EMMC 启动 1 0 1
TF 卡启动 1 0 0
```

TF 卡烧写 Android

```sh
#在 TF 卡中建立目录“sdupdate”，然后将 Android 的镜像拷贝到“sdupdate”目录下

boot.img
cache.img
system.img
u-boot-iTOP-6818.bin
userdata.img

#recovery.img


#将 TF 卡接到开发板，将开发板设置为 TF 卡启动，上电启动开发板，进入 uboot 模式
sdfuse flashall

setenv bootsystem android
saveenv
```

TF 卡烧写 Qt

```sh
#在 TF 卡中建立目录“sdupdate”，然后将 QT 的镜像拷贝到“sdupdate”目录下

boot.img
system.img
u-boot-iTOP-6818.bin

#将 TF 卡接到开发板，将开发板设置为 TF 卡启动，上电启动开发板，进入 uboot 模式
sdfuse flashall

setenv bootsystem qt
saveenv
```
