/**
 * @file	stm32_common.cpp
 * @brief	Low level common STM32 code
 *
 * @date Mar 28, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "global.h"
#include "efi_gpio.h"

#ifndef EFI_PIN_ADC9
#define EFI_PIN_ADC9 GPIOB_1
#endif /* EFI_PIN_ADC9 */

#if EFI_PROD_CODE
#include "usbconsole.h"
extern ioportid_t PORTS[];
#if defined(STM32F4XX) || defined(STM32F7XX)
ioportid_t PORTS[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH };
#else
ioportid_t PORTS[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOF};
#endif /* defined(STM32F4XX) || defined(STM32F7XX) */
#endif /* EFI_PROD_CODE */

#if HAL_USE_ADC

// ADC_CHANNEL_IN0 // PA0
// ADC_CHANNEL_IN1 // PA1
// ADC_CHANNEL_IN2 // PA2
// ADC_CHANNEL_IN3 // PA3
// ADC_CHANNEL_IN4 // PA4
// ADC_CHANNEL_IN5 // PA5 - this is also TIM2_CH1
// ADC_CHANNEL_IN6 // PA6
// ADC_CHANNEL_IN7 // PA7
// ADC_CHANNEL_IN8 // PB0
// ADC_CHANNEL_IN9 // PB1
// ADC_CHANNEL_IN10 // PC0
// ADC_CHANNEL_IN11 // PC1
// ADC_CHANNEL_IN12 // PC2
// ADC_CHANNEL_IN13 // PC3
// ADC_CHANNEL_IN14 // PC4
// ADC_CHANNEL_IN15 // PC5

brain_pin_e getAdcChannelBrainPin(const char *msg, adc_channel_e hwChannel) {
	// todo: replace this with an array :)
	switch (hwChannel) {
	case EFI_ADC_0:
		return GPIOA_0;
	case EFI_ADC_1:
		return GPIOA_1;
	case EFI_ADC_2:
		return GPIOA_2;
	case EFI_ADC_3:
		return GPIOA_3;
	case EFI_ADC_4:
		return GPIOA_4;
	case EFI_ADC_5:
		return GPIOA_5;
	case EFI_ADC_6:
		return GPIOA_6;
	case EFI_ADC_7:
		return GPIOA_7;
	case EFI_ADC_8:
		return GPIOB_0;
	case EFI_ADC_9:
		return EFI_PIN_ADC9;
	case EFI_ADC_10:
		return GPIOC_0;
	case EFI_ADC_11:
		return GPIOC_1;
	case EFI_ADC_12:
		return GPIOC_2;
	case EFI_ADC_13:
		return GPIOC_3;
	case EFI_ADC_14:
		return GPIOC_4;
	case EFI_ADC_15:
		return GPIOC_5;
	default:
		firmwareError(CUSTOM_ERR_ADC_UNKNOWN_CHANNEL, "Unknown hw channel %d [%s]", hwChannel, msg);
		return GPIO_INVALID;
	}
}

adc_channel_e getAdcChannel(brain_pin_e pin) {
	switch (pin) {
	case GPIOA_0:
		return EFI_ADC_0;
	case GPIOA_1:
		return EFI_ADC_1;
	case GPIOA_2:
		return EFI_ADC_2;
	case GPIOA_3:
		return EFI_ADC_3;
	case GPIOA_4:
		return EFI_ADC_4;
	case GPIOA_5:
		return EFI_ADC_5;
	case GPIOA_6:
		return EFI_ADC_6;
	case GPIOA_7:
		return EFI_ADC_7;
	case GPIOB_0:
		return EFI_ADC_8;
	case EFI_PIN_ADC9:
		return EFI_ADC_9;
	case GPIOC_0:
		return EFI_ADC_10;
	case GPIOC_1:
		return EFI_ADC_11;
	case GPIOC_2:
		return EFI_ADC_12;
	case GPIOC_3:
		return EFI_ADC_13;
	case GPIOC_4:
		return EFI_ADC_14;
	case GPIOC_5:
		return EFI_ADC_15;
	default:
		return EFI_ADC_ERROR;
	}
}

// deprecated - inline?
ioportid_t getAdcChannelPort(const char *msg, adc_channel_e hwChannel) {
	brain_pin_e brainPin = getAdcChannelBrainPin(msg, hwChannel);
	return getHwPort(msg, brainPin);
}

// deprecated - inline?
int getAdcChannelPin(adc_channel_e hwChannel) {
	brain_pin_e brainPin = getAdcChannelBrainPin("get_pin", hwChannel);
	return getHwPin("get_pin", brainPin);
}

extern SerialUSBDriver SDU1;

void jump_to_bootloader() {
	// AN2606 Application note
	// STM32 microcontroller system memory boot mode

	// todo: this does not work yet

	// Switch to the HSI clock source - no PLL
	RCC->CR &= RCC_CR_HSITRIM | RCC_CR_HSION; /* CR Reset value.              */
	RCC->CFGR = 0; /* CFGR reset value.            */

	usbDisconnectBus(&USBD1);
	chThdSleepMilliseconds(1500);

	sdStop (&USB_SERIAL_DRIVER);
	sduStop (&SDU1);
	usbStop (&USBD1);
	__disable_irq();
	chSysDisable();
	// reset the Systick Timer
	SysTick->CTRL = SysTick->LOAD = SysTick->VAL = 0;
	SYSCFG->MEMRMP = 0x01;
	SCB->VTOR = 0;
	SCB->ICSR = (0x1U << 27); //ICSR_PENDSVCLR;


	// set the main stack pointer to its default value
	__set_MSP(0x20001000);

	void (*SysMemBootJump)(void);
	SysMemBootJump = (void (*)(void)) (*((uint32_t *) 0x1FFF0004));

	SysMemBootJump();
}

#endif /* HAL_USE_ADC */
