- 发现是Qt加载插件出现问题，通过设置`QT_DEBUG_PLUGINS=1`，可以查看Qt的插件加载信息。
- 编译移植libiconv，[参考博文](https://blog.csdn.net/deyili/article/details/118224954)
- ![image-20220802181918181](C:\Users\H0114\AppData\Roaming\Typora\typora-user-images\image-20220802181918181.png)

```This application failed to start because it could not find or load the Qt platform plugin "wayland"in "/opt/qt-5.9.9-linux-aarch64-gcc/plugins/platforms".```

说明没有wayland这个gui组件

```Available platform plugins are: linuxfb (from /opt/qt-5.9.9-linux-aarch64-gcc/plugins/platforms), minimal (from /opt/qt-nux-aarch64-gcc/plugins/platforms), vnc (from /opt/qt-5.9.9-linux-aarch64-gcc/plugins/platforms), linuxfb, minimal, offs```

说明了可用的组件，如`vnc`, `linuxfb`...

解决办法，设置QT_QPA_PLATFORM环境变量，以offsrceen为例

```bash
export QT_QPA_PLATFORM=offscreen
```



