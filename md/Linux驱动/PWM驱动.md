## SYSFS PWM

对于 PWM 的 kernel 和 user space 使用说明在 Documentation/pwm.txt 有说明，下面重点提下 user space 部
分。就像 pwm.txt 文档里面说的，PWM 提供了用户层的接口，在 /sys/class/pwm/ 节点下面，PWM 驱动
加载成功后，会在 /sys/class/pwm/ 目录下产生 pwmchip0 目录；向 export 文件写入 0，就是打开 pwm 定
时器 0，会产生一个 pwm0 目录，相反的往 unexport 写入 0 就会关闭 pwm 定时器了，同时 pwm0 目录会
被删除,该目录下有以下几个文件：

enable：写入 1 使能 pwm，写入 0 关闭 pwm；
polarity：有 normal 或 inversed 两个参数选择，表示输出引脚电平翻转；
duty_cycle：在 normal 模式下，表示一个周期内高电平持续的时间（单位：纳秒），在 reversed 模
式下，表示一个周期中低电平持续的时间（单位：纳秒)；
period：表示 pwm 波的周期(单位：纳秒)；
以下是 pwmchip0 的例子，设置 pwm0 输出频率 100K，占空比 50%, 极性为正极性：

PWM Backlight
cd /sys/class/pwm/pwmchip0/
echo 0 > export
cd pwm0
echo 10000 > period
echo 5000 > duty_cycle
echo normal > polarity
echo 1 > enable

Q：pwmchip对应的是什么关系？

A：与驱动配置有关。如果驱动中配置了pwm0、pwm4、pwm8，则pwmchip0/1/2分别对应pwm0/4/8。

如下，

参数1，pwm4，对应某个pwmchipX

参数2，表示 index (per-chip index of the PWM to request)，一般是 0，因为我们 Rockchip PWM 每个
chip 只有一个

参数3，表示 PWM 输出波形的时间周期，单位是 ns；例如下面配置的 25000 ns就是表示想要得到的
PWM 输出周期是 40K 赫兹

参数 4，表示极性，为可选参数；下面例子中的配置为正极性。

```bash
pwms = <&pwm4 0 1000000 0>;
```

