#include "stm32f4xx.h"
#include "led.h"
#include <stdio.h>
void DelayLED(__IO uint32_t nCount);  //函数声明
uint8_t I2C_Test(void);
int main(void)
{
  LED_GPIO_Config();                  //GPIO管脚初始化
  while (1)
  {
    LED1_ON;
    LED2_ON;
    DelayLED(0XFFFFFF);
    LED1_OFF;
    LED2_OFF;
    DelayLED(0XFFFFFF);
  }
}
/********************************************************************************************
*函数名称：void Delay(__IO uint32_t nCount)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：延时函数
*******************************************************************************************/
void DelayLED(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}



void _init(void){} 