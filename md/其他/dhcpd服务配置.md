修改/etc/init.d/S80dhcp-server。例如把INTERFACES的值改为usb网卡(如usb0)

```bash
# /etc/init.d/S80dhcp-server
# DHCP服务器运行脚本
# 依赖: /etc/dhcp/dhcpd.conf dhcpd

#!/bin/sh
#
# $Id: dhcp3-server.init.d,v 1.4 2003/07/13 19:12:41 mdz Exp $
#

# On what interfaces should the DHCP server (dhcpd) serve DHCP requests?
#       Separate multiple interfaces with spaces, e.g. "eth0 eth1".
INTERFACES="usb0"

# Additional options that are passed to the DHCP server daemon?
OPTIONS=""

NAME="dhcpd"
DAEMON="/usr/sbin/${NAME}"
CFG_FILE="/etc/default/${NAME}"

# Read configuration variable file if it is present
[ -r "${CFG_FILE}" ] && . "${CFG_FILE}"

# Sanity checks
test -f /usr/sbin/dhcpd || exit 0
test -f /etc/dhcp/dhcpd.conf || exit 0

case "$1" in
	start)
		printf "Starting DHCP server: "
		test -d /var/lib/dhcp/ || mkdir -p /var/lib/dhcp/
		test -f /var/lib/dhcp/dhcpd.leases || touch /var/lib/dhcp/dhcpd.leases
		start-stop-daemon -S -q -x ${DAEMON} -- -q $OPTIONS $INTERFACES
		[ $? = 0 ] && echo "OK" || echo "FAIL"
		;;
	stop)
		printf "Stopping DHCP server: "
		start-stop-daemon -K -q -x ${DAEMON}
		[ $? = 0 ] && echo "OK" || echo "FAIL"
		;;
	restart | force-reload)
		$0 stop
		$0 start
		if [ "$?" != "0" ]; then
			exit 1
		fi
		;;
	*)
		echo "Usage: $0 {start|stop|restart|force-reload}"
		exit 1
esac

exit 0
```

修改配置脚本/etc/dhcp/dhcpd.conf，里面的内容很多，只需要将默认的配置，改为以下内容即可

```bash
# /etc/dhcp/dhcpd.conf
# DHCP服务器运行脚本
# 依赖: dhcpd

subnet 10.11.25.0 netmask 255.255.255.0 { # 子网掩码配置
range 10.11.25.100 10.11.26.200; # IP分配池范围
option domain-name-servers 114.114.114.114; # dns配置，可省略
option routers 10.11.25.1; # 网关IP
option broadcast-address 10.11.25.255; # 广播地址
default-lease-time 600; # 默认租约时间
max-lease-time 7200; # 最大租约时间
}
```

如上设置完以后，dhcp服务器（运行dhcpd的linux系统）的ip将变为10.11.25.1，dhcp客户端（如通过USB连接的Windows系统）将获得10.11.25.100的IP
