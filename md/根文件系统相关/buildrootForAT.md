必备组件：

/etc/profile /etc/resolve.conf

AT502:

| 组件名                       | 配置项                                                       | AT502      | AT603      |
| ---------------------------- | ------------------------------------------------------------ | ---------- | ---------- |
| python3                      | BR2_PACKAGE_NCURSES_WCHAR（提供对wchar宽字符数据类型的支持） BR2_SHARED_LIBS（需要有动态库才行）BR2_PACKAGE_PYTHON3 BR2_PACKAGE_PYTHON3_PY_ONLY   BR2_PACKAGE_PYTHON_NUMPY BR2_PACKAGE_PYTHON_PIP | (jamcode)⭐ | (ocusync)⭐ |
| OpenSSH ？                   | BR2_PACKAGE_OPENSSH（需要使能PermitRootLogin）               | ⭐          | ⭐          |
| 开机自动分配空间             | BR2_PACKAGE_E2FSPROGS_RESIZE2FS                              | ⭐          | ⭐          |
| usbmount自动挂载             |                                                              | ⭐          | ⭐          |
| Qt5.15.2                     | 自行搭建                                                     | ⭐          | ⭐          |
| AT502脚本                    | 主控启动、主控和背板服务、FX3串口记录与下载服务              | ⭐          |            |
| AT603脚本                    | 主控启动、ocusync脚本、FX3相关                               |            | ⭐          |
| USB以太网                    | (RNDIS)kernel中配置                                          | ⭐          | ⭐          |
| exFat4格式U盘支持            | BR2_PACKAGE_EXFAT                                            | ⭐          | ⭐          |
| USB枚举实用程序              | BR2_PACKAGE_USBUTILS                                         | ⭐          | ⭐          |
| 相关网络的ip与nameserver更改 |                                                              | ⭐          | ⭐          |
| 命令行首显示                 | [linux系统 命令行首显示 #而不是$ 码农家园 (codenong.com)](https://www.codenong.com/cs105695464/) | ⭐          | ⭐          |
| BASH                         | BR2_PACKAGE_BASH                                             | ⭐          | ⭐          |

在rockchip 3568的builroot添加python相关库的时候，老是报错，site.py", line 177  file=sys.stderr)，网上说可能和python的多版本有关，pythonpath设置的不对，但是这个是buildroot啊！难道是内部的环境变量不对？

首先看日志br.log，找到使用的python路径是host/bin/python，链接的是python3.X的库

于是我查看buildroot里build/bin/python，是一个软连接，居然是指向python2.7！！这可能是原来rockchip搭建好的python版本，怪不得会报错

于是重新链接，确实这个错误消失了，但是会显示缺少cython的模块

在make menuconfig找到host-cython，添加到defconfig并继续编译，发现不生效还是报错

于是用make O=output/rockchip_rk3568 host-python-cython-rebuild编译好host-cython

再继续上述编译操作就成功了



搭建出来的Python环境可以pip下载，以pandas为例：

```bash
pip3 install pandas -i http://pypi.tuna.tsinghua.edu.cn/simple --trusted-host pypi.tuna.tsinghua.edu.cn # http下载 需要加trusted-host
```





# buildroot for Qt5

| 组件名                             | 配置项                             | AT502 | AT603 |
| ---------------------------------- | ---------------------------------- | ----- | ----- |
| QT5.15 LTS版本                     | BR2_PACKAGE_QT5_VERSION_5_15       | ⭐     | ⭐     |
| Qt5Concurrent                      | BR2_PACKAGE_QT5BASE_CONCURRENT     | ⭐     | ⭐     |
| sqlite支持                         | BR2_PACKAGE_QT5BASE_SQLITE_SYSTEM  | ⭐     | ⭐     |
| LinuxFb                            | BR2_PACKAGE_QT5BASE_LINUXFB        | ⭐     | ⭐     |
| eglfs                              | BR2_PACKAGE_QT5BASE_EGLFS          | ⭐     | ⭐     |
| OpenGL相关库                       | BR2_PACKAGE_QT5BASE_OPENGL_LIB     | ⭐     | ⭐     |
| fontconfig and freetype2 libraries | BR2_PACKAGE_QT5BASE_FONTCONFIG     | ⭐     | ⭐     |
| RGB                                | BR2_PACKAGE_QT5BASE_LINUXFB_ARGB32 | ⭐     | ⭐     |
| 图表                               | BR2_PACKAGE_QT5CHARTS              | ⭐     |       |
| QtSerial                           | BR2_PACKAGE_QT5SERIALPORT          | ⭐     | ⭐     |
| QWebChannel                        | BR2_PACKAGE_QT5WEBCHANNEL          | ⭐     |       |
| QJson                              | BR2_PACKAGE_QJSON                  | ⭐     | ⭐     |
| 压缩工具                           | BR2_PACKAGE_QUAZIP                 | ⭐     | ⭐     |
| QTXCB需要XCB支持                   | BR2_PACKAGE_XORG7                  |       |       |
| eglfs需要QTXCB支持                 | BR2_PACKAGE_QT5BASE_XCB            |       |       |
| Qt quick                           | BR2_PACKAGE_QT5DECLARATIVE         |       |       |
| Qt quick                           | BR2_PACKAGE_QT5DECLARATIVE_QUICK   |       |       |
| gdb python                         | BR2_PACKAGE_HOST_GDB_PYTHON        |       |       |
| QQuick 常用组件                    | BR2_PACKAGE_QT5QUICKCONTROLS2      |       |       |

裁剪

| 组件名 | 配置项 | AT502 | AT603 |
| ------ | ------ | ----- | ----- |
|        |        |       |       |
|        |        |       |       |
|        |        |       |       |
|        |        |       |       |

