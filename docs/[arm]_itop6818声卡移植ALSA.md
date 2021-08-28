<a href="/itop6818-programs-linux/tree/master/%E5%A3%B0%E5%8D%A1%E7%A7%BB%E6%A4%8DALSA" target="_blank">声卡移植 ALSA</a>

源码编译

```sh
/root/ALSA
tar -xvf alsa-lib-1.1.5.tar.bz2;cd alsa-lib-1.1.5
CC=arm-linux-gnueabihf-gcc ./configure --host=arm-linux --prefix=/root/ALSA/install/
make;make install

ls /root/ALSA/install/

tar -xvf alsa-utils-1.1.5.tar.bz2;cd alsa-utils-1.1.5
CC=arm-linux-gnueabihf-gcc ./configure --prefix=/root/ALSA/install/ --host=arm-linux --with-alsa-inc-prefix=/root/ALSA/install/include --with-alsa-prefix=/root/ALSA/install/lib --disable-alsamixer
--disable-xmlto --disable-nls
make;cd aplay;ls;make
#用 ls 命令可以看到生成的aplay可执行文件
```

```sh
cp binary/* /bin/
```

在开发板上使用 ALSA，将 U 盘插到运行 qt 系统的开发板上

```sh
mount /dev/sda1 /mnt/disk
cp /mnt/disk/ALSA/* /bin
```

插上耳机，使用以下命令检测耳机

```sh
speaker-test -c 2 &

killall speaker-test
```

此时的 ALSA 并不完全，如果通过 apaly 直接播放音频的话会发现不能控制音量；
创建了一个名为 'pcm' 的从设备，隶属于音量控制插件 primary，通过!default 选项将该插件设置成默认 PCM 输出设备

```sh
vi /etc/asound.conf

pcm.!default "plug:primary"
pcm.primary {
	type softvol
	slave.pcm "plughw:0"
	control.name "master"
	control.card 0
}
```

重启开发板

```sh
reboot
```

用 aplay 进行音频播放，音频文件放在/root 目录中

```sh
aplay /root/VenusMars1.wav &

#默认255，将音量设置为 100，音量值可取 0-255
amixer set 'master' 100
```

录音测试

```sh
#filename 用户自己指定，&不可省略
arecord filename &
```

aplay 播放测试
