#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>

#define I2C_SLAVE 0x78

#define OLED_CMD 0x00
#define OLED_DATA 0x40

#define I2C_SLAVE_CMD 0x0703
#define I2C_TENBIT 0x0704

static int fd;
static u_int8_t OLED_GRAM[144][8];

void SendByte(u_int8_t dat)
{
	int ret = write(fd, &dat, sizeof(dat));
	if(ret == -1)
	{
    	perror("failed to send byte");
	}
}

void OLED_wrByte(u_int8_t data, u_int8_t mode)
{
	u_int8_t buf[2];
	buf[0] = mode;
	buf[1] = data;

	int ret = write(fd, buf, sizeof(buf));
	if(ret == -1)
	{
    	perror("failed to send byte");
	}
}

//开启OLED显示 
void OLED_disPlayOn(void)
{
	OLED_wrByte(0x8D, OLED_CMD);//电荷泵使能
	OLED_wrByte(0x14, OLED_CMD);//开启电荷泵
	OLED_wrByte(0xAF, OLED_CMD);//点亮屏幕
}

//关闭OLED显示 
void OLED_disPlayOff(void)
{
	OLED_wrByte(0x8D, OLED_CMD);//电荷泵使能
	OLED_wrByte(0x10, OLED_CMD);//关闭电荷泵
	OLED_wrByte(0xAF, OLED_CMD);//关闭屏幕
}

void OLED_refresh()
{
	u_int8_t i,n;
	for(i=0;i<8;i++)
	{
	   	OLED_wrByte(0xb0 + i, OLED_CMD); //设置行起始地址
	   	OLED_wrByte(0x00, OLED_CMD);   //设置低列起始地址
	   	OLED_wrByte(0x10, OLED_CMD);   //设置高列起始地址
	   	for(n=0; n < 128; n++)
		{
			OLED_wrByte(OLED_GRAM[n][i], OLED_DATA);
		}
  }
}

//清屏函数
void OLED_clear(void)
{
	u_int8_t i,n;
	for(i=0;i<8;i++)
	{
	   	for(n=0;n<128;n++)
		{
			OLED_GRAM[n][i]=0;//清除所有数据
		}
  	}
	OLED_refresh();//更新显示
}

//画点 
//x:0~127
//y:0~63
void OLED_drawPoint(u_int8_t x,u_int8_t y)
{
	u_int8_t i, m, n;
	i = y / 8;
	m = y % 8;
	n = 1 << m;
	OLED_GRAM[x][i] |= n;
}

//清除一个点
//x:0~127
//y:0~63
void OLED_clearPoint(u_int8_t x,u_int8_t y)
{
	u_int8_t i,m,n;
	i = y / 8;
	m = y % 8;
	n = 1 << m;
	OLED_GRAM[x][i] =~ OLED_GRAM[x][i];
	OLED_GRAM[x][i] |= n;
	OLED_GRAM[x][i] =~ OLED_GRAM[x][i];
}

//画线
//x:0 ~ 128
//y:0 ~ 64
void OLED_drawLine(u_int8_t x1, u_int8_t y1, u_int8_t x2, u_int8_t y2)
{
	u_int8_t i,k,k1,k2,y0;
	if((x1 < 0) || (x2 > 128) || (y1 < 0) 
		|| (y2 > 64) || (x1 > x2) || (y1 > y2))
	{
		return;
	}

	if(x1 == x2)    //画竖线
	{
		for(i = 0; i < (y2-y1); i++)
		{
			OLED_drawPoint(x1, y1 + i);
		}
  	}
	else if(y1 == y2)   //画横线
	{
		for(i=0; i < (x2 - x1); i++)
		{
			OLED_drawPoint(x1+i, y1);
		}
  	}
	else      //画斜线
	{
		k1 = y2 - y1;
		k2 = x2 - x1;
		k = k1 * 10 / k2;
		for(i = 0; i < (x2 - x1); i++)
		{
			OLED_drawPoint(x1+i,y1+i*k/10);
		}
	}
}

//m^n
u_int32_t OLED_pow(u_int8_t m, u_int8_t n)
{
	u_int32_t result = 1;
	while(n--)
	{
	   result *= m;
	}
	return result;
}

//配置写入数据的起始位置
void OLED_wrBP(u_int8_t x,u_int8_t y)
{
	OLED_wrByte(0xb0+y, OLED_CMD);//设置行起始地址
	OLED_wrByte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_wrByte((x & 0x0f) | 0x01, OLED_CMD);
}

void init()
{
    SendByte(0xAE);		 //关闭显示
	
	SendByte(0xD5); //设置显示时钟分频比/振荡器频率
	SendByte(0x80);

	SendByte(0xA8); //设置多路复用率
	SendByte(0x3F);

	SendByte(0xD3); //设置显示偏移
	SendByte(0x00);

	SendByte(0x40); //设置显示开始行

	SendByte(0xA1); //设置左右方向，0xA1正常 0xA0左右反置

	SendByte(0xC8); //设置上下方向，0xC8正常 0xC0上下反置

	SendByte(0xDA); //设置COM引脚硬件配置
	SendByte(0x12);

	SendByte(0x81); //设置对比度控制
	SendByte(0xCF);

	SendByte(0xD9); //设置预充电周期
	SendByte(0xF1);

	SendByte(0xDB); //设置VCOMH取消选择级别
	SendByte(0x30);

	SendByte(0xA4); //设置整个显示打开/关闭

	SendByte(0xA6); //设置正常/倒转显示

	SendByte(0x8D); //设置充电泵
	SendByte(0x14);

	SendByte(0xAF); //开启显示

	OLED_clear();
}

void main()
{
    fd = open("/dev/i2c-4", O_RDWR);
    if(fd < 0)
    {
        perror("failed to open i2c :");
    }
    else
    {
        printf("success ot open i2c-4\n");
    }

	if (ioctl(fd, I2C_SLAVE_CMD, I2C_SLAVE >> 1)< 0) {
    	perror("set slave address failed\n");
	}
	else
	{
		printf("success to se i2c slave addr \n");
	}

	if (ioctl(fd, I2C_TENBIT, 0) < 0) {
		perror("failed to tenbit\n");
	}
	else
	{
		printf("success to tanbit\n");
	}

    init();

	OLED_disPlayOn();
	OLED_drawLine(4, 6, 50, 30);
	OLED_refresh();

    close(fd);
}