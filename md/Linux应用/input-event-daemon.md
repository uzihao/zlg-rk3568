Linux系统提供一种进程检测特定按键并执行用户指令

[(115条消息) input-event-daemon_大樹bigtree的博客-CSDN博客](https://blog.csdn.net/lewanhah01/article/details/84838747)

首先在buildroot中添加input-event-daemon这个Hardware handing

```bash
BR2_PACKAGE_INPUT_EVENT_DAEMON
 input-event-daemon is a daemon which executes                                                  │  
  │ a user-defined command on input events, such as                                                │  
  │ keys, mouse buttons and switches.                                                              │  
  │                                                                                                │  
  │ http://github.com/gandro/input-event-daemon
```

通常需要一个配置conf位于/etc/input-event-daemon.conf

```bash
#
# /etc/input-event-daemon.conf
# RK809相关的按键配置，对应静音、休眠等等
#

[Keys]
*POWER:1      = /etc/power-key.sh press #监测到键值POWER输出值为1（对应PWRON被拉低）时，按键触发按下
*POWER:0      = /etc/power-key.sh release #监测到键值POWER输出值为0（对应PWRON不再被拉低）时，按键触发松开
MUTE         = amixer -q set Master mute #MUTE静音按键
FN+VOLUMEUP  = factory_reset_cfg
CTRL+ALT+ESC = beep
```

POWER实际上对应的键值是 116，这个可以在/usr/include/linux/input-event-codes.h中查到

这里启动的是/etc/power-key.sh

```bash
#!/bin/sh

EVENT=${1:-short-press}

TIMEOUT=3 # s
PIDFILE="/tmp/$(basename $0).pid"
LOCKFILE=/tmp/.power_key

short_press()
{
        logger -t $(basename $0) "[$$]: Power key short press..."

        if type pm-suspend &>/dev/null; then
                SUSPEND_CMD="pm-suspend"
        else
                SUSPEND_CMD="echo -n mem > /sys/power/state"
        fi

        if [ ! -f $LOCKFILE ]; then
                logger -t $(basename $0) "[$$]: Prepare to suspend..."

                touch $LOCKFILE
                sh -c "$SUSPEND_CMD"
                { sleep 2 && rm $LOCKFILE; }&
        fi
}

long_press()
{
        logger -t $(basename $0) "[$$]: Power key long press (${TIMEOUT}s)..."

        logger -t $(basename $0) "[$$]: Prepare to power off..."

        poweroff
}

logger -t $(basename $0) "[$$]: Received power key event: $@..."              
                                                                 
case "$EVENT" in                                                      
        press)                                                  
                # Lock it                                                     
                exec 3<$0               
                flock -x 3                                      
                                                                      
                start-stop-daemon -K -q -p $PIDFILE || true                   
                start-stop-daemon -S -q -b -m -p $PIDFILE -x /bin/sh -- \
                        -c "sleep $TIMEOUT; $0 long-press"            
                                                                      
                # Unlock                                                      
                flock -u 3                                
                ;;                                              
        release)                                                
                # Avoid race with press event                            
                sleep .5                                                      
                
                start-stop-daemon -K -q -p $PIDFILE && short_press
                ;;                                                            
        short-press)                                                          
                short_press                                       
                ;;                                              
        long-press)                                                      
                long_press                                
                ;;                                                
esac
```

大概的意思是，如果短按（即不超过$TIMEOUT设置的秒数（这里是3s）），则进入休眠（pm-suspend），否则直接关机（poweroff），采用了start-stop-daemon进行线程相关安全处理

配置完成以后，可以设置一个input-event-daemon的启动脚本SXXinput-event-daemon，上电默认开启

```bash
#! /bin/sh

[ -f /usr/bin/input-event-daemon ] || exit 0

start() {
        printf "Starting input-event-daemon: "
        input-event-daemon -v /dev/input/event*
        echo "done"
}


stop() {
        printf "Stopping input-event-daemon: "
        killall input-event-daemon
        echo "done"
}

restart() {
        stop
        start
}

# See how we were called.
case "$1" in
  start)
        start
        ;;
  stop)
        stop
        ;;
  restart|reload)
        restart
        ;;
  *)
        echo "Usage: $0 {start|stop|reload|restart}"
        exit 1
esac

exit $? 
```

大概意思是如果存在input-event-daemon服务，则对输入子事件进行监测