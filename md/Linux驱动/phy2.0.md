现象：使用yt8531SH 1000M网络，但未知何种原因自协商为100M网。

问题：100M也能满足需求，但自协商时间过长，通常需要LED闪烁三次才能结束。

解决办法：1.修改驱动

查找资料发现，已知MII/GMII/RGMII协议对于phy寄存器是有标准规范的，drivers/net/mii.h规定0x00寄存器(MII_BMCR)的定义如下，因此可以在驱动往struct phy_driver结构体的config_init回调函数，关闭网卡自协商，开启百兆，关闭千兆

```bash
/* Basic mode control register. */
#define BMCR_RESV		0x003f	/* Unused...                   */
#define BMCR_SPEED1000		0x0040	/* MSB of Speed (1000)         */
#define BMCR_CTST		0x0080	/* Collision test              */
#define BMCR_FULLDPLX		0x0100	/* Full duplex                 */
#define BMCR_ANRESTART		0x0200	/* Auto negotiation restart    */
#define BMCR_ISOLATE		0x0400	/* Isolate data paths from MII */
#define BMCR_PDOWN		0x0800	/* Enable low power state      */
#define BMCR_ANENABLE		0x1000	/* Enable auto negotiation     */
#define BMCR_SPEED100		0x2000	/* Select 100Mbps              */
#define BMCR_LOOPBACK		0x4000	/* TXD loopback bits           */
#define BMCR_RESET		0x8000	/* Reset to default state      */
#define BMCR_SPEED10		0x0000	/* Select 10Mbps               */
```

```c
static void yt8531S_config_init_speed(struct phy_device *phydev)
{
    int ret;
    int val = 0;
    const char *speed;
    struct device *dev = &phydev->mdio.dev;
    struct device_node *of_node = dev->of_node;

    ret = of_property_read_string(of_node, "speed", &speed);
    if (ret >= 0) {
        val = phy_read(phydev, MII_BMCR);
        if (val < 0) {
            netdev_info(phydev->attached_dev, "%s , phy addr: %d, failed to read MII_BMCR\n",
                __func__, phydev->mdio.addr);
        }
        ret = strcmp(speed, "100M");
        if (ret == 0) {
            val &= ~BMCR_SPEED1000;
            val &= ~BMCR_ANENABLE;
            val |= BMCR_SPEED100;
            ytphy_write_ext(phydev, 0xa000, 0);
            phy_write(phydev, MII_BMCR, val);
            netdev_info(phydev->attached_dev, "%s , phy addr: %d, set to 100M, val is 0x%x\n",
                __func__, phydev->mdio.addr, val);
            return;
        }
        ret = strcmp(speed, "1000M");
        if (ret == 0) {
            val |= BMCR_SPEED1000;
            val &= ~BMCR_ANENABLE;
            val &= ~BMCR_SPEED100;
            ytphy_write_ext(phydev, 0xa000, 0);
            phy_write(phydev, MII_BMCR, val);
            netdev_info(phydev->attached_dev, "%s , phy addr: %d, set to 1000M\n",
                __func__, phydev->mdio.addr);
            return;
        }
        netdev_info(phydev->attached_dev, "%s , phy addr: %d, Invalid speed: %s\n",
            __func__, phydev->mdio.addr, speed);
    } else {
        netdev_info(phydev->attached_dev, "%s , phy addr: %d, speed not set\n",
            __func__, phydev->mdio.addr);
    }
}
```

对于dts:

```bash
&mdio0 {
	rgmii_phy0: phy@0 {
		compatible = "ethernet-phy-ieee802.3-c22";
		reg = <0x0>;

		/* motorcomm, YT8531SH phy */
		phy-connection-type = "rgmii-id";
		led0_state = "link";
		led1_state = "active";
		led2_state = "active";

		speed = "100M";

        clocks = <&cru CLK_MAC0_OUT>;
	};
};
```

配置完成后，发现初始化时BMCR值确实写入进去了，但网卡起来时自协商却又重新打开了，问题还是存在

2.应用层配置

已知ethtool可以配置网络，实现锁定100M、关闭自协商

```ba
ethtool -s eth0 speed 100 duplex full autoneg off
```

可以在/etc/init.d/启动脚本中加入配置

后面发现可以直接修改/etc/network/interfaces修改，但这种方法还是有问题，没插入网线就不会配置

```bash
auto lo
iface lo inet loopback

auto eth1
iface eth1 inet static
address 169.254.1.99
gateway 169.254.1.1
netmask 255.255.255.0
up ethtool -s eth1 speed 100 duplex full autoneg off

auto eth0
iface eth0 inet static
address 192.168.4.210
gateway 192.168.4.1
netmask 255.255.255.0
up ethtool -s eth0 speed 100 duplex full autoneg off
```

up ... 代表ifup启动网卡后做的事情，同理down ... 代表网卡ifdown关闭网卡后做的事情，pre-up是启动网卡前，pre-down是关闭网卡前

现象：能通过MDIO总线与phy寄存器通信，但插入网线phy没有把RJ45 LED灯点亮，网络也不通

分析：发现不是用晶振，而是MAC提供25M时钟给phy，但内核没开这个时钟

解决办法：

修改前:

```bash
&gmac0 {
...

	assigned-clocks = <&cru SCLK_GMAC0_RX_TX>, <&cru SCLK_GMAC0>; 
	assigned-clock-parents = <&cru SCLK_GMAC0_RGMII_SPEED>, <&cru CLK_MAC0_2TOP>;

	assigned-clock-rates = <0>, <125000000>;
	
	pinctrl-names = "default";
	pinctrl-0 = <&gmac0_miim
		     &gmac0_tx_bus2
		     &gmac0_rx_bus2
		     &gmac0_rgmii_clk
		     &gmac0_rgmii_bus>;
...
};

&mdio0 {
	rgmii_phy0: phy@0 {
		compatible = "ethernet-phy-ieee802.3-c22";
		reg = <0x0>;

		/* motorcomm, YT8531SH phy */
		phy-connection-type = "rgmii-id";
		led0_state = "link";
		led1_state = "active";
		led2_state = "active";
	};
};
```

修改后：

```bash
&gmac0 {
	phy-mode = "rgmii";
	clock_in_out = "output";

	snps,reset-gpio = <&gpio1 RK_PB1 GPIO_ACTIVE_LOW>; /* change */
	snps,reset-active-low;
	/* Reset time is 20ms, 100ms for rtlr8211f */
	snps,reset-delays-us = <0 20000 140000>;

	assigned-clocks = <&cru SCLK_GMAC0_RX_TX>, <&cru SCLK_GMAC0>, <&cru CLK_MAC0_OUT>;
	assigned-clock-parents = <&cru SCLK_GMAC0_RGMII_SPEED>, <&cru CLK_MAC0_2TOP>;

	assigned-clock-rates = <0>, <125000000>, <25000000>;
	
	pinctrl-names = "default";
	pinctrl-0 = <&gmac0_miim
		     &gmac0_tx_bus2
		     &gmac0_rx_bus2
		     &gmac0_rgmii_clk
		     &gmac0_rgmii_bus
             &eth0_pins>;

	tx_delay = <0x26>;
	rx_delay = <0x2c>;

	phy-handle = <&rgmii_phy0>;
	status = "okay";
};
&mdio0 {
	rgmii_phy0: phy@0 {
		compatible = "ethernet-phy-ieee802.3-c22";
		reg = <0x0>;

		/* motorcomm, YT8531SH phy */
		phy-connection-type = "rgmii-id";
		led0_state = "link";
		led1_state = "active";
		led2_state = "active";

        clocks = <&cru CLK_MAC0_OUT>;
	};
};
```





