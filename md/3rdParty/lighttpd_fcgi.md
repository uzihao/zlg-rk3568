[(29条消息) lighttpd+fastcgi嵌入式web交叉编译到arm_苏提春晓_的博客-CSDN博客_lighttpd arm](https://blog.csdn.net/main_h_/article/details/116260243)

[(29条消息) 使用lighttpd搭建轻量级web服务器详解_dvlinker的博客-CSDN博客_嵌入式轻量级web服务器](https://blog.csdn.net/chenlycly/article/details/126072917?ops_request_misc=%7B%22request%5Fid%22%3A%22167583742716800182741593%22%2C%22scm%22%3A%2220140713.130102334..%22%7D&request_id=167583742716800182741593&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~baidu_landing_v2~default-4-126072917-null-null.142^v73^insert_down2,201^v4^add_ask,239^v1^insert_chatgpt&utm_term=lighttpd服务器&spm=1018.2226.3001.4187)

```shell
.
├── cache
├── cgi-bin
│   └── myfastcgi.fcgi
├── config
│   ├── conf.d
│   │   ├── access_log.conf
│   │   ├── auth.conf
│   │   ├── cgi.conf
│   │   ├── debug.conf
│   │   ├── deflate.conf
│   │   ├── dirlisting.conf
│   │   ├── evhost.conf
│   │   ├── expire.conf
│   │   ├── fastcgi.conf
│   │   ├── magnet.conf
│   │   ├── Makefile
│   │   ├── Makefile.am
│   │   ├── Makefile.in
│   │   ├── mime.conf
│   │   ├── mod.template
│   │   ├── proxy.conf
│   │   ├── rrdtool.conf
│   │   ├── scgi.conf
│   │   ├── secdownload.conf
│   │   ├── simple_vhost.conf
│   │   ├── ssi.conf
│   │   ├── status.conf
│   │   ├── userdir.conf
│   │   └── webdav.conf
│   ├── lighttpd.conf
│   └── modules.conf
├── fcgi-prefix // 编译出来的fcgi
│   ├── bin
│   │   └── cgi-fcgi
│   ├── include
│   │   ├── fastcgi.h
│   │   ├── fcgiapp.h
│   │   ├── fcgi_config.h
│   │   ├── fcgimisc.h
│   │   ├── fcgio.h
│   │   ├── fcgios.h
│   │   └── fcgi_stdio.h
│   └── lib
│       ├── libfcgi++.a
│       ├── libfcgi.a
│       ├── libfcgi++.la
│       ├── libfcgi.la
│       ├── libfcgi++.so -> libfcgi++.so.0.0.0
│       ├── libfcgi.so -> libfcgi.so.0.0.0
│       ├── libfcgi++.so.0 -> libfcgi++.so.0.0.0
│       ├── libfcgi.so.0 -> libfcgi.so.0.0.0
│       ├── libfcgi++.so.0.0.0
│       ├── libfcgi.so.0.0.0
│       └── pkgconfig
│           ├── fcgi++.pc
│           └── fcgi.pc
├── lib 
│   ├── mod_access.la
... (省略)
│   └── mod_wstunnel.so
├── log
│   └── error.log // 保存日志
├── sbin
│   ├── lighttpd // bin
│   └── lighttpd-angel
├── share
│   └── man
│       └── man8
│           ├── lighttpd.8
│           └── lighttpd-angel.8
├── sockets
├── test
│   ├── myfastcgi.cpp // 自己编写的fcgi源文件
│   ├── myfastcgi.fcgi
│   ├── myfastcgi.o
│   └── test.html // 测试网页
├── upload
├── vhosts
└── webpages
    └── test.html

```

编译fcgi

```bash
aarch64-linux-gnu-g++ -c myfastcgi.cpp -o  myfastcgi.o -I ../fcgi-prefix/include 
aarch64-linux-gnu-g++ myfastcgi.o -o  myfastcgi.fcgi -I ../fcgi-prefix/include -lfcgi -L ../fcgi-prefix/lib
```



附录：

[u-blox.txt](../doc/lighttpd_fcgi_prefix.tar)
