安装 gcc-arm-linux-gnueabihf

```sh
apt-get install gcc-arm-linux-gnueabihf
```

<details>
<summary>boa的移植和配置</summary>

```sh
wget http://www.boa.org/boa-0.94.13.tar.gz
mkdir ~/work; tar xvf boa-0.94.13.tar.gz -C ~/work;cd ~/work/boa-0.94.13;

#根据configure.in文件进行一系列的配置，生成config.status,configure和Makefile文件
chmod 755 configure
./configure
apt-get install -y byacc flex
```

vim src/compat.h

```C
//#define TIMEZONE_OFFSET(foo) foo##->tm_gmtoff
#define TIMEZONE_OFFSET(foo) (foo)->tm_gmtoff
```

```sh
cat boa.conf|grep -v "#"|grep -v "^$"
```

```sh
mkdir /usr/lib/boa/ /etc/boa/ /var/log/boa /www

cat > /etc/boa/boa.conf<<EOF
Port 80
User root
Group root
ErrorLog /var/log/boa/error_log
AccessLog /var/log/boa/access_log
DocumentRoot /www
UserDir public_html
DirectoryIndex index.html
DirectoryMaker /usr/lib/boa/boa_indexer
KeepAliveMax 1000
KeepAliveTimeout 10
MimeTypes /etc/mime.types
DefaultType text/plain
CGIPath /bin:/usr/bin:/usr/local/bin
Alias /doc /usr/doc
ScriptAlias /cgi-bin/ /www/cgi-bin/
ServerName localhost
EOF
```

```sh
cd src;
make
arm-linux-gnueabihf-strip boa

cp boa_indexer /usr/lib/boa/
cp boa /etc/boa/
chmod 755 -R /www

vim log.c
/*
 if (dup2(error_log, STDERR_FILENO) == -1) {
  DIE("unable to dup2 the error log");
 }
*/


vim boa.c
/*
 if (setuid(0) != -1) {
  DIE("icky Linux kernel bug!");
 }
*/

#启动web服务
./boa
```

</details>

<details>
<summary>cgic库移植和配置</summary>

```sh
git clone https://github.com/boutell/cgic
cd cgic

vim Makefile
CC=gcc 改成 CC=arm-linux-gnueabihf-gcc
#CPP=gcc -E 改成 CCP=arm-linux-gnueabihf-gcc -E
AR=ar 改成 AR=arm-linux-gnueabihf-ar
RANLIB=ranlib 改成 RANLIB=arm-linux-gnueabihf-ranlib
gcc cgictest.o -o cgictest.cgi ${LIBS} 改成 $(CC) $(CFLAGS) cgictest.o -o cgictest.cgi ${LIBS}
gcc capture.o -o capture ${LIBS} 改成 $(CC) $(CFLAGS) capture.o -o capture ${LIBS}

make

#来验证生成CGIC库的正确性
CGIC库中的常用文件：
cgic.h：头文件
cgic.c：CGIC的源代码文件
cgictest.c：CGIC库的作者提供的一个CGI程序例子
capture.c：用于调试CGI程序的工具
Makefile：安装CGIC的脚本文件


mkdir -p ~/work/cgictest;cd ~/work/cgictest
ls ~/work/cgic/
cp ~/work/cgic/cgic.h .
cp ~/work/cgic/cgic.c .
cp ~/work/cgic/cgictest.c .
cp ~/work/cgic/capture .
cp ~/work/cgic/cgictest.cgi .
#LIBS
cp ~/work/cgic/libcgic.a .

vim Makefile
CFLAGS=-g -Wall
CC=arm-linux-gnueabihf-gcc
AR=arm-linux-gnueabihf-ar
RANLIB=arm-linux-gnueabihf-ranlib
LIBS=-L./ -lcgic

test.cgi:cgictest.c cgic.h cgic.c
（这里必有一个tab符）$(CC) $(CFLAGS) cgictest.c cgic.c -o test.cgi ${LIBS}

mkdir -p /www/cgi-bin
cp capture /www/cgi-bin/
cp test.cgi /www/cgi-bin/

apt-get install curl
curl http://127.0.0.1/cgi-bin/cgictest.cgi
```

</details>

<details>
<summary>cgictest.c</summary>

```C
#include <stdio.h>
#include "cgic.h"
#include <string.h>
#include <stdlib.h>
int cgiMain() {
	cgiHeaderContentType("text/html");
	fprintf(cgiOut, "<HTML><HEAD>\n");
	fprintf(cgiOut, "<TITLE>My First CGI</TITLE></HEAD>\n");
	fprintf(cgiOut, "<BODY><H1>Hello CGIC</H1></BODY>\n");
	fprintf(cgiOut, "</HTML>\n");
	return 0;
}
```

</details>

<details>
<summary>myled.c</summary>

```C
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#define MAX 10

int main(void)
{
	char *data;
	int leds[2] = {0, 0};
	long m, n;
	int exit = 0, i;
	printf("Content-Type:text/html;charset=gb2312\n\n");
	printf("<html>\n");
	printf("<body>\n");
	printf("<title>iTOP-4412</title> ");
	printf("<h3>iTOP-4412</h3> ");
	data = getenv("QUERY_STRING");
	printf("<p>receive data:%s</p>", data);
	while (*data != '\0')
	{
		if (*data == '=')
			switch (*(data + 1))
			{
			case '1': leds[0] = 1; break;
			case '2': leds[1] = 1; break;
			default: exit = 1; break;
			}
		if (exit == 1)
			break;
		data++;
	}


	int fd, LedOnOffTimes;
	char gpio[MAX], cmd[MAX];
	char *leds_ctl = "/dev/leds_ctl";
	LedOnOffTimes = MAX;

	printf("leds_ctl light on and off 5 times \r\n");


	if ((fd = open(leds_ctl, O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
		printf("open %s failed\n", leds_ctl);
	else
	{
		printf("open %s success\r\n", leds_ctl);
		while (LedOnOffTimes--)
		{
			printf("ioctl leds_ctl %d times\n", LedOnOffTimes);
			ioctl(fd, 0, 0);	//parameter 2 is cmd ,cmd = 1 leds on
			ioctl(fd, 0, 1);
			sleep(1);
			ioctl(fd, 1, 0);
			ioctl(fd, 1, 1);
			sleep(1);
		}
	}
	ioctl(fd, 0, 0);
	close(fd);

	return 0;
}
```

</details>

<details>
<summary>myled.html</summary>

```html
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>led 远程控制</title>
    <style type="text/css">
      body {
        background-color: #999900;
        text-align: center;
      }

      .ziti {
        font-size: 24px;
      }

      .juzhong {
        text-align: center;
      }

      .hsz {
        text-align: center;
      }

      .hsz td {
        color: #00f;
        font-size: 18px;
      }

      .hsz {
        background-color: #fcc;
      }

      .juzhong table {
        text-align: center;
      }

      .juzhong table tr {
      }

      #h1 {
        background-color: #0fc;
      }

      #h2 {
        background-color: #ff9;
      }

      .h3 {
        background-color: #0cf;
      }

      .ys1 {
        font-size: 24px;
      }

      .STYLE1 {
        font-size: 36px;
      }
    </style>
  </head>

  <body class="juzhong">
    <table
      width="900"
      border="0"
      align="center"
      cellpadding="0"
      cellspacing="0"
    >
      <tr>
        <td>
          <p class="STYLE1">&nbsp;</p>
          <p class="STYLE1">iTOP-6818 WEB SERVER</p>
        </td>
      </tr>
      <tr>
        <td height="30">&nbsp;</td>
      </tr>
      <tr>
        <td>
          <form
            action="/cgi-bin/myled.cgi"
            method="get"
            enctype="application/x-www-form-urlencoded"
            name="form1"
            target="_blank"
            id="form1"
          >
            <table
              width="300"
              border="1"
              align="center"
              cellpadding="1"
              cellspacing="1"
            >
              <tr>
                <td>Led1</td>
                <td>
                  <input name="led1" type="checkbox" id="led1" value="1" />
                  <label for="led1"></label>
                </td>
              </tr>
              <tr>
                <td>Led2</td>
                <td>
                  <input name="led2" type="checkbox" id="led2" value="2" />
                  <label for="led2"></label>
                </td>
              </tr>
              <tr>
                <td colspan="2">
                  <input
                    type="submit"
                    name="submit"
                    id="submit"
                    value="submit"
                  />
                </td>
              </tr>
            </table>
          </form>
        </td>
      </tr>
      <tr>
        <td>
          <p>&nbsp;</p>
        </td>
      </tr>
    </table>
    <p>&nbsp;</p>
  </body>
</html>
```

</details>

编译：

```sh
gcc-arm-linux-gnueabihf-gcc –o myled.cgi myled.c
mv myled.cgi /www/cgi-bin
```

在开发板上启动 boa，开启 web 服务

```sh
mv myled.html /www/
#在电脑上浏览器上输入，192.168.xxx.xxx/myled.html，开始测试
```

在虚拟机上安装 nfs

```sh
apt-get install -y portmap nfs-kernel-server
```

编辑 exports 文件

```sh
vi /etc/exports
/nfs/arm *(rw,sync,no_root_squash)

mkdir -p /nfs/arm
chmod 755 -R /nfs
```

启动 nfs

```sh
/etc/init.d/portmap start
/ect/init.d/nfs start
#检测nfs是否真的在运行
/etc/init.d/portmap status
```

确定开发板和虚拟机 linux 的 IP 地址

```
虚拟机采用桥接方式

修改开发板和虚拟机的IP地址，
使在同一网段下可以在开发板或者虚拟机终端里ping对方IP,
如果通则网络正常了
```

挂载 NFS 到开发板上

```sh
mount -t nfs 192.168.1.100:/www/cgi-bin /nfs/arm -o nolock
```

卸载

```sh
umount /nfs/arm
```

<a href="https://www.jianshu.com/p/dafcf652baed" target="_blank">嵌入式设备 web 开发笔记：boa 和 cgic</a>
<a href="https://www.cnblogs.com/xmnn1990/p/4704023.html" target="_blank">arm-linux 挂载 nfs 文件系统</a>
