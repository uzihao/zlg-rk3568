/*---------------------------------------
*功能描述:  直流电机测试程序
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



#define PATH  	"/dev/dc_motor"                 //绝对路径

/*************************************************
*主函数
*************************************************/
int main(void)
{
	int fd;
	unsigned int val;

	fd = open(PATH, O_RDWR);                         //打开设备，成功返回0
	if (fd < 0) {
		perror("Failed to open!\n");
		exit(1);
	}

	do{ 
		printf(" Please select how to operate the DC motor.\n");
		printf("0 ->Stop\n");
     		printf("1 -> Rollback\n");
		printf("4 -> Foreward\n");
		printf("9 -> Exit!\n");
     		scanf("%d", &val);
		
		if(val == 9) 
			break;
		ioctl(fd, val, 0);
	}while(1);

	close(fd);
	return 0;
}

