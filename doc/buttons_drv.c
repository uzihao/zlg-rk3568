/*---------------------------------------
*功能描述:  蜂鸣器驱动 
*创建者：   粤嵌技术部
*创建时间： 2015,01,01
---------------------------------------
*修改日志：
*修改内容：
*修改人：
*修改时间：
----------------------------------------*/

/*************************************************
*头文件
*************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>

#include <asm/mach-types.h>
#include <asm/gpio.h>
#include <asm/delay.h>
#include <mach/gpio.h>
#include <mach/soc.h>
#include <mach/platform.h>


#define DEVICE_NAME		"gecBt"                //设备名字

//按键结构体
struct button_desc {
	int gpio;
	int number;
	char *name;	
	struct timer_list timer;
};

//按键的管脚，编号，名字
static struct button_desc buttons[] = {
    	{ (PAD_GPIO_B + 9 ), 0, "KEY0" },//B9
	{ (PAD_GPIO_A + 28), 1, "KEY1" },//A28
    	{ (PAD_GPIO_B + 30), 2, "KEY2" },//volup//B30
	{ (PAD_GPIO_B + 31), 3, "KEY3" },//voidn//B31
    	//{ (PAD_GPIO_B + 9 ), 3, "KEY3" },//B9
};

//赋按键的初始值
static volatile char key_values[] = {
	0, 0, 0, 0
};

//等待队列申请
static DECLARE_WAIT_QUEUE_HEAD(button_waitq);

static volatile int ev_press = 0;

/*************************************************
*定时器
*************************************************/
static void x6818_buttons_timer(unsigned long _data)
{
	struct button_desc *bdata = (struct button_desc *)_data;

	mod_timer(&bdata->timer, jiffies + msecs_to_jiffies(40));
}

/*************************************************
*按键中断
*************************************************/
static irqreturn_t button_interrupt(int irq, void *dev_id)
{
	struct button_desc *bdata = (struct button_desc *)dev_id;
	int down;
	int number;
	unsigned tmp;

	tmp = gpio_get_value(bdata->gpio);

	/* active low */
	down = tmp;
	pr_err("KEY %d: %08x\n", bdata->number, down);

	number = bdata->number;
	if (down == (key_values[number])) {
		key_values[number] = '0' + down;
		ev_press = 1;
		wake_up_interruptible(&button_waitq);
	}

	return IRQ_HANDLED;
}

/*************************************************
*按键初始化
*************************************************/
static int x6818_buttons_init(void)
{
	int irq;
	int i;
	int err = 0;

	//每个按键 都设置 为双边向沿 触发模式
	for (i = 0; i < ARRAY_SIZE(buttons); i++) {
		if (!buttons[i].gpio)
			continue;
		gpio_free(buttons[i].gpio);
		setup_timer(&buttons[i].timer, x6818_buttons_timer,
				(unsigned long)&buttons[i]);

		irq = gpio_to_irq(buttons[i].gpio);
		
		pr_err("irq = %.2d\n",irq);
		err = request_irq(irq, button_interrupt, IRQ_TYPE_EDGE_BOTH, 
				buttons[i].name, (void *)&buttons[i]);
		if (err)
		{
			pr_err("irq = %.2d is failed\n",i);
			break;
		}

	}
	//中断申请失败的时候，释放中断
	if (err) {
		i--;
		for (; i >= 0; i--) {
			if (!buttons[i].gpio)
				continue;

			irq = gpio_to_irq(buttons[i].gpio);
			disable_irq(irq);
			free_irq(irq, (void *)&buttons[i]);

			del_timer_sync(&buttons[i].timer);
		}

		return -EBUSY;
	}

	ev_press = 1;
	return 0;
}

static int x6818_buttons_open(struct inode *inode, struct file *file) 
{
	return 0;
}



/*************************************************
*按键释放
*************************************************/
static int x6818_buttons_close(struct inode *inode, struct file *file)
{
	int irq, i;

	for (i = 0; i < ARRAY_SIZE(buttons); i++) {
		if (!buttons[i].gpio)
			continue;
		irq = gpio_to_irq(buttons[i].gpio);
		free_irq(irq, (void *)&buttons[i]);

		del_timer_sync(&buttons[i].timer);
	}

	return 0;
}

/*************************************************
*按键读取值
*************************************************/
static int x6818_buttons_read(struct file *filp, char __user *buff,
		size_t count, loff_t *offp)
{
	unsigned long err;
	
	if (!ev_press) {
		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;
		else{
			pr_err("wai_enven*********************\n");
			wait_event_interruptible(button_waitq, ev_press);
			pr_err("OK*********************************\n");
		}
	}

	ev_press = 0;

	err = copy_to_user((void *)buff, (const void *)(&key_values),
			min(sizeof(key_values), count));

	return err ? -EFAULT : min(sizeof(key_values), count);
}

/*************************************************
*按键的头
*************************************************/
static unsigned int x6818_buttons_poll( struct file *file,
		struct poll_table_struct *wait)
{
	unsigned int mask = 0;

	poll_wait(file, &button_waitq, wait);
	if (ev_press)
		mask |= POLLIN | POLLRDNORM;

	return mask;
}

/*************************************************
*文件操作集
*************************************************/
static struct file_operations dev_fops = {
	.owner		= THIS_MODULE,
	.open           = x6818_buttons_open,
	.release	= x6818_buttons_close, 
	.read		= x6818_buttons_read,
	.poll		= x6818_buttons_poll,
};

/*************************************************
*杂项设备
*************************************************/
static struct miscdevice misc = {
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= DEVICE_NAME,
	.fops		= &dev_fops,
};

/********************************************************************
*驱动的初始化函数--->从内核中申请资源（内核、中断、设备号、锁....）
********************************************************************/
static int __init button_dev_init(void)
{
	int ret;
	ret = misc_register(&misc);
	x6818_buttons_init();
	pr_err(DEVICE_NAME"\tinitialized\n");

	return ret;
}

/*****************************************************************
*驱动退出函数 --->将申请的资源还给内核
*****************************************************************/
static void __exit button_dev_exit(void)
{
	misc_deregister(&misc);
}

module_init(button_dev_init);                       //驱动的入口函数会调用一个用户的初始化函数
module_exit(button_dev_exit);                       //驱动的出口函数会调用一个用户的退出函数

//驱动的描述信息： #modinfo  *.ko , 驱动的描述信息并不是必需的。
MODULE_AUTHOR("ZOROE@GEC");                         //驱动的作者
MODULE_DESCRIPTION("Butoons of driver");            //驱动的描述
MODULE_LICENSE("GPL");                              //遵循的协议


