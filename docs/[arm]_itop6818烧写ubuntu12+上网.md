文件清单
<a href="http://www.diskgenius.cn/" target="_blank">DiskGenius</a>将 TF 卡转成 FAT32 格式
<a href="http://www.pc0359.cn/downinfo/52244.html" target="_blank">ATTO Disk Benchmark 32 位/64 位(硬盘速度测试软件)</a>

```
iTOP6818 开发板资料汇总（不含光盘内容）\05_iTOP-6818开发板Ubuntu系统\01_Ubuntu 镜像\iTOP4412_ubuntu_12.04_for_LCD_20141230.tar.gz

iTOP6818 开发板资料汇总（不含光盘内容）\05_iTOP-4418 开发板Ubuntu系统\02_Ubuntu12.04系统升级目录\iTOP_6818_ubuntu_20180320.tgz

iTOP-6818 光盘资料\05 镜像_QT文件系统\u-boot-iTOP-6818.bin
iTOP-6818 光盘资料\05 镜像_QT文件系统\boot.img
```

<a href="https://pan.baidu.com/s/12V6yl21PeEdhKNiYFHKRnw" target="_blank">文件资料</a> 提取码：8n1e

将 TF 卡接入 PC 机，打开硬盘速度测试软件软件，设置一下参数：

```
“驱动器”选择接入 PC 的 TF 卡”
“数据包大小”选择
“1024->1024”
“总长度”选择“32mb”
单击按钮“开始”测试，结束后，会显示 TF 卡的测试结果
若显示写入速度不够 5MB/秒，不能用于烧写 Ubuntu
```

TF 卡分区

```
#先在 TF 卡接入虚拟机Ubuntu 之前查看盘符
df -l


TF 卡分三个区
1、指定分区前面的自由空间大小为 32M
2、新区大小为 2048M，“Linux Ext3”格式
3、新区设置为 FAT32 格式
```

查看盘符

```sh
df -l
```

解压到 TF 卡 2G 的盘符

```sh
cd /media/NEW\ VOLUME_/
rm -rf *
ls -a
#ubuntu 解压到 2G 的盘符
sudo tar zxvf iTOP_6818_ubuntu_20180320.tgz -C /media/NEW\ VOLUME_/

#ubuntu压缩包放到 fat32 的盘符
iTOP_6818_ubuntu_20180320.tgz
```

fastboot 目录下放置镜像

```sh
# 烧写 uboot 和 kernel 镜像
iTOP-6818 光盘资料\05 镜像_QT 文件系统\2G\u-boot-iTOP-6818.bin

iTOP-6818 光盘资料\05 镜像_QT 文件系统\2G\boot.img
```

（在终端助手执行）将 TF 卡接到开发板上，开发板烧写 QT 镜像文件

```sh
fastboot
```

接上 OTG

```sh
fastboot flash uboot u-boot-iTOP-6818.bin
fastboot flash boot boot.img
fastboot reboot
```

设置 TF 启动

```sh
setenv bootsystem ubuntu_tf
saveenv
reset
```

（在终端助手执行）查看盘符

```sh
df -l
#ls /dev/sd*
ls /dev/mmcblk1p*
```

将 Ubuntu 压缩包拷贝到 emmc 中

```sh
#emmc位置
cd /media/57f8f4bc-abf4-655f-bf67-946fc0f9f25b_/
rm -rf *
ls
tar vxf /media/NEW\ VOLUME/iTOP_6818_ubuntu_20180320.tgz -C ./

#解压完成之后，开发板断电，拔掉 TF 卡（注意一定要拔掉 TF 卡），然后启动开发板，进入 uboot 模式
```

（在终端助手执行）设置 emmc 启动

```sh
setenv bootsystem ubuntu_emmc
saveenv
reset
```

<a href="https://blog.csdn.net/wf19930209/article/details/73195055" target="_blank">virtualbox 的 USB 识别</a>

```
vboxmanage -v
6.0.10r132072
```

<a href="https://bbs.21ic.com/icview-1630956-1-3.html" target="_blank">学习 4412 开发板烧写 Ubuntu 系统</a>
<a href="https://blog.csdn.net/liboxiu/article/details/81772391" target="_blank">迅为 itop4412 烧写 ubuntu，脱离 tf 卡启动</a>

关键文件

```sh
/usr/bin/6620_launcher
insmod /lib/modules/mt6620/mtk_hif_sdio.ko
insmod /lib/modules/mt6620/mtk_stp_wmt.ko
insmod /lib/modules/mt6620/mtk_stp_uart.ko
insmod /lib/modules/mt6620/mtk_stp_gps.ko
#insmod /lib/modules/mt6620/hci_stp.ko
#insmod /lib/modules/mt6620/mt6620_fm_drv.ko
#insmod /lib/modules/mt6620/mtk_fm_priv.ko
insmod /lib/modules/mt6620/mtk_wmt_wifi.ko WIFI_major=194
insmod /lib/modules/mt6620/wlan_mt6620.ko
wpa_supplicant
```

<details>
<summary>/etc/init.d/mt6620</summary>

```sh
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
chmod 0660 /dev/ttySAC2
chmod 0666 /dev/gps

/usr/bin/6620_launcher -m 1 -b 921600 -n /etc/firmware/mt6620_patch_hdr.bin -d /dev/ttySAC2 &

sleep 4

echo 1 > /dev/wmtWifi

ifconfig eth0 down

sleep 2

wpa_supplicant -iwlan0 -Dnl80211 -c/etc/wpa_supplicant.conf -dd >/var/wifi_log &

sleep 15
udhcpc -i wlan0 >/var/udhcpc_log &
```

</details>

<details>
<summary>setwifi_6818.sh</summary>

```sh
#!/bin/bash
mknod /dev/stpwmt c 190 0
mknod /dev/stpgps c 191 0
mknod /dev/fm c 193 0
mknod /dev/wmtWifi c 194 0

insmod /system/lib/modules/mtk_hif_sdio.ko
insmod /system/lib/modules/mtk_stp_wmt.ko
insmod /system/lib/modules/mtk_stp_uart.ko
insmod /system/lib/modules/mtk_stp_gps.ko

insmod /system/lib/modules/mtk_wmt_wifi.ko WIFI_major=194
insmod /system/lib/modules/wlan_mt6620.ko

chmod 0666 /dev/stpwmt
chmod 0666 /dev/stpgps
chmod 0666 /dev/fm
chmod 0666 /dev/wmtWifi
chmod 0660 /dev/ttySAC2
chmod 0666 /dev/gps

/system/bin/6620_launcher -m 1 -b 921600 -n /system/etc/firmware/mt6620_patch_hdr.bin -d /dev/ttySAC2 &

sleep 3
oput=`echo 1 > /dev/wmtWifi`
wpa_supplicant -iwlan0 -Dnl80211 -c/etc/wpa_supplicant.conf  &
pid=$!
sleep 2

kill $pid
wpa_supplicant  -i wlan0 -Dwext -c /etc/wpa_supplicant.conf &
dhclient wlan0 &
```

</details>

<details>
<summary>pppd_conf.sh</summary>

```sh
#!/bin/sh
pwd
cd `dirname $0` || exit 0
pwd
pppd call wcdma  | tee  /usr/ppp.log &
sleep 4
vgw=`tail /usr/ppp.log |   grep  'remote IP address' | grep -m 1 -o '\([0-9]\{1,3\}\.\)\{3\}[0-9]\{1,3\}'`
vnamenserver=`tail /usr/ppp.log |  grep  primary | grep -m 1 -o '\([0-9]\{1,3\}\.\)\{3\}[0-9]\{1,3\}'`
echo $vgw
echo $vnamenserver
echo "nameserver $vnamenserver" > /etc/resolv.conf
route add default gw $vgw
```

</details>

<details>
<summary>wifi_instart.sh</summary>

```sh
#!/bin/bash
yellow() {
    echo  "\033[33m $1 \033[0m"
}

echo
yellow 'setting up wlan_mt6620...'
cat << ED > /etc/init.d/itop-set
#!/bin/bash


  #turn on 4.3 inch screen
  echo 60 > /sys/class/gpio/export
  echo out > /sys/class/gpio/gpio60/direction
  echo 1 > /sys/class/gpio/gpio60/value

  #Configure audio route as :-
  amixer cset numid=7 127
  amixer cset numid=8 1
  amixer cset numid=40 1
  amixer cset numid=45 1

  #chown root:tty /dev/ttySAC*
  #chmod 777 /dev/ttySAC*

  echo 0 > /proc/sys/kernel/hung_task_timeout_secs

#################################################################################################################
######################### support MT6620 WIFI Module ########################
#!/bin/sh
  mknod /dev/stpwmt c 190 0
  mknod /dev/stpgps c 191 0
  mknod /dev/fm c 193 0
  mknod /dev/wmtWifi c 194 0

  insmod /system/lib/modules/mtk_hif_sdio.ko
  insmod /system/lib/modules/mtk_stp_wmt.ko
  insmod /system/lib/modules/mtk_stp_uart.ko
  insmod /system/lib/modules/mtk_stp_gps.ko

  insmod /system/lib/modules/mtk_wmt_wifi.ko WIFI_major=194
  insmod /system/lib/modules/wlan_mt6620.ko

  chmod 0666 /dev/stpwmt
  chmod 0666 /dev/stpgps
  chmod 0666 /dev/fm
  chmod 0666 /dev/wmtWifi
  chmod 0660 /dev/ttySAC2
  chmod 0666 /dev/gps

  /system/bin/6620_launcher -m 1 -b 921600 -n /system/etc/firmware/mt6620_patch_hdr.bin -d /dev/ttySAC2 &

  sleep 3
  oput=\`echo 1 > /dev/wmtWifi\`
  wpa_supplicant -iwlan0 -Dnl80211 -c/etc/wpa_supplicant.conf  &
  pid=$!
  sleep 2

  kill $pid
  wpa_supplicant  -i wlan0 -Dwext -c /etc/wpa_supplicant.conf &
  dhclient wlan0 &


######################## end support ########################

#################################################################################################################
######################## support ath6kl wifi module ########################

#   insmod /lib/firmware/ath6k/AR6003/hw2.1.1/cfg80211.ko
#   insmod /lib/firmware/ath6k/AR6003/hw2.1.1/ath6kl_sdio.ko

#   sleep 1
############################ endf support ############################

ED
echo
yellow 'done'
echo
echo ' use "wpa_passphrase ssid [passphrase] /etc/wpa_supplicant.conf" to preset wifi'
echo
yellow  'then reboot or run "sh /etc/init.d/itop-set" to set up wifi'
```

</details>

MT6620 WiFi 测试

```sh
# 配置上网名称和密码
wpa_passphrase XXX "YYY" > /etc/wpa_supplicant.conf

# 设置时间大约 15 秒，设置完成后就可上网
sh setwifi_6818.sh

# 发现网卡节点
ifconfig

# 若上一步没有问题，设置 WiFi 开机启动
sh wifi_instart.sh
```

EC20 4G 测试

```sh
# 插上天线以及 SIM 卡
sh /root/pppd_conf.sh

# 发现 4G 的节点
ifconfig
```
