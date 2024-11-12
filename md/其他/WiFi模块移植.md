```bash
# /etc/udev/rules.d/start_wifi_ap.sh
# 配置支持DHCP的WiFi-AP模式
# 依赖：hostapd udev /etc/hostapd.conf F80dhcp-server /etc/dhcp/dhcpd.conf

#!/bin/bash

echo "ready to killall hostapd"
killall hostapd

echo "sleep start"
sleep 5

echo "ip config start"
ifconfig wlan0 10.11.26.1 up
#route add default gw 10.11.26.1 dev wlan0

sleep 2

echo "dhcp start"
/etc/init.d/F80dhcp-server restart

echo "hostapd start"
hostapd /etc/hostapd.conf -B

exit 0

```

```bash
# /etc/hostapd.conf
# WPA2-PSK authentication with AES encryption
# 配置WiFi模式
# 依赖：hostapd
interface=wlan0
driver=nl80211
ctrl_interface=/var/run/hostapd
ssid=atm12skyd # atm13skyd # Please customize the hotspot name
channel=6 # default
ieee80211n=1
hw_mode=g
ignore_broadcast_ssid=1 # Whether to hide hotspot names
wpa=2 
wpa_passphrase=skydmmx1234 # Please customize the passwd
wpa_key_mgmt=WPA-PSK
rsn_pairwise=CCMP
```

```bash
# /etc/udev/rules.d/98-usb-wifi-88x2cu.rules
# 88x2cu默认udisk设备，需要驱动切换为WiFi网卡，切换后将重新热插拔检测
# 依赖: udev usb_modeswitch usb_modeswitch.d

ACTION=="add", ATTR{idVendor}=="0bda", ATTR{idProduct}=="1a2b", RUN+="/etc/udev/rules.d/usb_modeswitch -W -c /etc/usb_modeswitch.d/0bda:1a2b"
```

```bash
# /etc/udev/rules.d/99-usb-wifi-88x2bu.rules
# 88x2bu及88x2cu(转换后)热插拔自动配置热点
# 依赖: udev start_wifi_ap.sh
ACTION=="add", SUBSYSTEM=="net", ATTRS{idVendor}=="0bda", ATTRS{idProduct}=="b812|c812|b82c", RUN+="/bin/sh /etc/udev/rules.d/start_wifi_ap.sh"
```



```bash
# /etc/usb_modeswitch.d/0bda\:1a2b
# usb_modeswitch表，标注源pidvid与目标pidvid
# RTL8812CU Wifi Dongle
DefaultVendor=0x0bda
DefaultProduct=0x1a2b
TargetVendor=0x0bda
TargetProduct=0xc812
StandardEject=1
```



```bash
# /etc/init.d/F80dhcp-server
# DHCP服务器运行脚本
# 依赖: /etc/dhcp/dhcpd.conf dhcpd

#!/bin/sh
#
# $Id: dhcp3-server.init.d,v 1.4 2003/07/13 19:12:41 mdz Exp $
#

# On what interfaces should the DHCP server (dhcpd) serve DHCP requests?
#       Separate multiple interfaces with spaces, e.g. "eth0 eth1".
INTERFACES="wlan0"

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



```bash
# /etc/dhcp/dhcpd.conf
# DHCP服务器运行脚本
# 依赖: dhcpd

ddns-update-style none;
option domain-name-servers 192.168.0.1, 114.114.114.114;
default-lease-time 600;
max-lease-time 7200;
log-facility local7;
subnet 10.11.26.0 netmask 255.255.255.0 {
range 10.11.26.100 10.11.26.200;
option domain-name-servers 114.114.114.114;
option routers 10.11.26.1;
option broadcast-address 10.11.26.255;
default-lease-time 600;
max-lease-time 7200;
}
```

