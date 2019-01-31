//=============================================================================
//文件名称:led.h
//功能概要:STM32f401RBT6 循环点亮LED灯
//版权所有:源地工作室http://vcc-gnd.taobao.com/
//版本更新:2015-02-20 V1.0
//调试方式:J-LINK-OB
//=============================================================================

//头文件
#include "led.h"

//=============================================================================
//函数名称:LED_GPIO_Config(void)
//功能概要:LED灯引脚配置
//参数名称:无
//函数返回:无
//=============================================================================
void LED_GPIO_Config(void)
{	
	//定义一个GPIO_InitTypeDef 类型的结构体
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOG,ENABLE);	//使能GPIOC的外设时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//选择要用的GPIO引脚		 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置引脚为普通输出模式		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//设置引脚为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置引脚速度为100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置引脚为上拉 		 
	GPIO_Init(GPIOB, &GPIO_InitStructure);//调用库函数，初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 		 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}






