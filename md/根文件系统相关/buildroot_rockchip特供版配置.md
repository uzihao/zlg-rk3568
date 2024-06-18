20230328：

buildroot可以使用不同的配置工具进行配置，以满足不同的需求。下面是几种常见的配置工具及其解释：

1. `make config`

`make config` 是最基础、最原始的一种配置工具。它提供了一种基于字符界面的交互式配置方式，用户需要一个一个地回答询问，以配置内核。该工具的缺点是配置选项较多，而且需要手动回答询问，比较繁琐。

1. `make menuconfig`

`make menuconfig` 是一种基于 ncurses 库的字符界面配置工具，相比于 `make config`，它提供了更友好的用户界面，可以更方便地进行配置。用户可以通过菜单选择不同的配置选项，而不需要手动输入。

1. `make xconfig`

`make xconfig` 是一种基于 Qt 库的图形界面配置工具，相比于 `make menuconfig`，它提供了更美观、更直观的用户界面。用户可以通过鼠标点击不同的配置选项，而不需要手动输入或选择菜单。

1. `make gconfig`

`make gconfig` 是一种基于 GTK+ 库的图形界面配置工具，与 `make xconfig` 类似，它也提供了美观、直观的用户界面，用户可以通过鼠标点击不同的配置选项来进行配置。rockchip_rk3568

1. `make savedefconfig`

`make savedefconfig` 用于生成一个新的 `.config` 文件，该文件包含当前配置的默认值。用户可以将其重命名为 `xxx_defconfig` 并将其保存到 `./configs/` 目录中，以便下次使用 `make xxx_defconfig` 命令快速加载该配置。

1. `make oldconfig`

`make oldconfig` 用于将旧版本的配置文件与新版本的内核源码进行比较，并在新版本中生成一个新的配置文件。这个命令通常用于升级内核版本时，以便在新版本中保留旧版本的配置选项。

1. `make defconfig`

`make defconfig` 用于生成一个默认的配置文件，该文件包含所有必需的配置选项和默认值。这个命令通常用于生成一个最小配置的内核，以便在嵌入式系统中使用。

总之，这些配置工具都可以用来配置 Linux 内核，每个工具都有自己的优缺点，用户可以根据自己的需要选择适合自己的配置工具。





基本操作步骤：

```bash
#定义buildroot_dir为buildroot的根目录

# 1
make rockchip_rk3568_defconfig #把${buildroot_dir}/configs/rockchip_rk3568_defconfig加载到${buildroot_dir}/.config
# 2 
make menuconfig #图形化配置。配置完成后，直接点击保存，默认覆盖${buildroot_dir}/.config
# 3
make savedefconfig #载入${buildroot_dir}/.config到${buildroot_dir}/configs/rockchip_rk3568_defconfig
#上面这一步保存的路径实际上是由BR2_DEFCONFIG决定的，.config已经将这个配置项定义为${buildroot_dir}/configs/rockchip_rk3568_defconfig
```

增加的项：



要单独清除 Buildroot 中的某个软件包，可以使用以下命令：

make <package-name>-dirclean

make python3-rebuild



BR2_ROOTFS_OVERLAY可以加入路径，路径中包含的内容将会被编译进文件系统中

BR2_ROOTFS_POST_BUILD_SCRIPT可以加入路径，路径中包含的脚本将会执行
