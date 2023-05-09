

下面这个脚本使用了rsync，用于同步系统中存放在不同位置的文件夹

```bash
#!/bin/bash

# 同步脚本
RSYNC_CMD="rsync -avr --delete"
RSYNC_DEST="${PWD}/."

RSYNC_SRCS=( \
"/home/uzihao/AT603-rootfs/target" \
"/home/uzihao/SDK/rockchip_rk3568/half_open/kernel/arch/arm64/boot/dts/rockchip/*at603-vc.dts" \
"/home/uzihao/SDK/rockchip_rk3568/half_open/kernel/arch/arm64/boot/dts/rockchip/*at603-vc-gpio.dtsi" \
"/home/uzihao/SDK/rockchip_rk3568/half_open/kernel/arch/arm64/boot/dts/rockchip/*at603-vc-lvds.dtsi" \
"/home/uzihao/SDK/rockchip_rk3568/half_open/kernel/arch/arm64/configs/at60x_config" \
"/home/uzihao/SDK/rockchip_rk3568/half_open/buildroot/configs/rockchip_rk3568_defconfig" \
)

MOUNT_SRC=/home/uzihao/AT603-rootfs/rootfs.img
MOUNT_DES=/home/uzihao/AT603-rootfs/target

sudo mount ${MOUNT_SRC} ${MOUNT_DES}

sleep 3

for src in "${RSYNC_SRCS[@]}";
do
    ${RSYNC_CMD} $src ${RSYNC_DEST}
done

sudo umount ${MOUNT_SRC} ${MOUNT_DES}
```

```bash
rsync -av --delete /path/to/source/folder/ /home/user/myfolder/
其中：

-a参数表示使用归档模式，保留所有备份相关的元数据，同时递归同步子目录。

-v参数表示使用详细模式，输出更多的信息。

--delete参数表示删除目标目录中不再源目录中存在的文件
```



