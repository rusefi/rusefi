#include "stm32f4xx.h"
#include <stdio.h>

//#include "ch.h"

#define LED_PIN 9

void DelayLED(__IO uint32_t nCount) {
	while(nCount--) {
  	}
}

void mainLED(int num) {
	int i;
	
	DelayLED(0XFFFFFF);
	DelayLED(0XFFFFFF);

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER |= (1 << (LED_PIN*2));
	GPIOB->OTYPER &= ~(1 << LED_PIN);

	for (i = 0; i < num; i++) {
		GPIOB->ODR &= ~(1 << LED_PIN);
		DelayLED(0XFFFFFF);
		GPIOB->ODR |= (1 << LED_PIN);
		DelayLED(0XFFFFFF);
	}
}

