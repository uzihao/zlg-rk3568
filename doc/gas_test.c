/*---------------------------------------
*功能描述:  按键测试程序 
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
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>

#define  ARRY_SIZE(x) (sizeof(x)/sizeof(x[0]))     //函数的大小

/*************************************************
*主函数
*************************************************/
int main(int argc , char **argv)
{
	int gas_fd ,ret;
	char current_gas_value[]={'0'}; 
	
	gas_fd = open("/dev/gec_gas_drv",O_RDONLY);    //打开设备下的驱动成功为0
	if(gas_fd <0){
		perror("open device :");
		exit(1);		
	}

	//读取烟雾的现象
	while(1){
		
		ret = read(gas_fd, current_gas_value, sizeof(current_gas_value));
		if (ret != sizeof(current_gas_value)) { 
			perror("read gas:");
			exit(1);
		}
		printf("---------------%d",   ret);
		printf("---------------%x \n",ret);
		if(current_gas_value[0] == '0'){
			printf("gas up warm!\n");
		}
		else{
			printf("gas down warm!\n");
			
		}
		current_gas_value[0] = '0';
		sleep(2);
	}

	return 0;
}
