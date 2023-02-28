| dpkg -l         | 获取已安装的程序 |
| --------------- | ---------------- |
| apt-get purge % | 删除软件         |
|                 |                  |
|                 |                  |

管理配置文件

| 命令                                          | 说明                                   |
| --------------------------------------------- | -------------------------------------- |
| /etc/systemd/resolved.conf                    | 网络配置相关，包括dns                  |
| systemctl restart systemd-resolved.service    | 重启上面这个服务                       |
| route add default gw 192.168.x.1              | 网关配置                               |
| sudo rsync -avx firefly@192.168.1.11:/ rootfs | 同步文件夹                             |
| stty -F /dev/ttyS1                            | 查看串口属性                           |
| stty -F /dev/ttyS1 speed 115200               | 设置串口波特率115200，一般会输出115200 |
| echo "123" > /dev/ttyS1                       | 写串口                                 |
| cat /dev/ttyS1输出                            | 输出串口                               |
| cat /dev/ttyS1 > output.txt                   | 输出串口内容到文本                     |
| cat /proc/bus/input/devices                   | 打印input设备                          |

| 删除的软件        |        |                        |
| ----------------- | ------ | ---------------------- |
| 2048-qt           | 游戏   |                        |
| fcitx*， libfcitx | 输入法 |                        |
| gnome*            |        |                        |
| gstreamer         |        |                        |
| lxqt              |        |                        |
| qml-module        |        |                        |
| qt5-              |        |                        |
|                   |        |                        |
| libkf5*           |        | 会自动下载软件，很奇怪 |
| libqt*            |        |                        |
| reoffice*         |        |                        |





