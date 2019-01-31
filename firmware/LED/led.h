#ifndef _LED_H_
#define _LED_H_
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

/*407VET6*/
#define  LED1_ON	  	GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define	 LED1_OFF	    GPIO_ResetBits(GPIOB,GPIO_Pin_9)

/*407ZGT6*/
#define  LED2_ON	  	GPIO_SetBits(GPIOG,GPIO_Pin_15)
#define	 LED2_OFF	    GPIO_ResetBits(GPIOG,GPIO_Pin_15)
	

void LED_GPIO_Config(void);

#endif
