/*
* Copyright(c) 2016 www.tarchcloud.com. All rights reserved 
* Created by Jason.Chen
*/

#include "../include/led.h"
#include <bcm2835.h>
#define POWER_PIN RPI_GPIO_P1_11

/************************************************
	函数:	
		PowerLenBlink
	参数：
		nCount:	LED闪烁多少次
	说明：
		创建上传数据字符串
*************************************************/
int PowerLenBlink(int nCount)
{	
	int bReturn = 1 ;
	int i = 0 ;
	if(bcm2835_init())
	{
		bcm2835_gpio_fsel(POWER_PIN,BCM2835_GPIO_FSEL_OUTP) ;

		for(i = 0 ;i < nCount ;i++)
		{
			printf("BLINK COUNT:%d\n",i );
			bcm2835_gpio_write(POWER_PIN, HIGH); 
			bcm2835_delay(500);
			bcm2835_gpio_write(POWER_PIN, LOW); 
			bcm2835_delay(500);
		}

		bcm2835_gpio_write(POWER_PIN, LOW); 

		bcm2835_close();

	}else 
	{
		bReturn = 0 ;
	}

	return bReturn ;
}
