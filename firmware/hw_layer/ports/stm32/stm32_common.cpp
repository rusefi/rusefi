/**
 * @file	stm32_common.cpp
 * @brief	Low level common STM32 code
 *
 * @date Mar 28, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "global.h"

#if EFI_PROD_CODE
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
		return GPIOB_1;
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
	case GPIOB_1:
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

// deprecated - migrate to 'getAdcChannelBrainPin'
ioportid_t getAdcChannelPort(const char *msg, adc_channel_e hwChannel) {
	// todo: replace this with an array :)
	switch (hwChannel) {
	case ADC_CHANNEL_IN0:
		return GPIOA;
	case ADC_CHANNEL_IN1:
		return GPIOA;
	case ADC_CHANNEL_IN2:
		return GPIOA;
	case ADC_CHANNEL_IN3:
		return GPIOA;
	case ADC_CHANNEL_IN4:
		return GPIOA;
	case ADC_CHANNEL_IN5:
		return GPIOA;
	case ADC_CHANNEL_IN6:
		return GPIOA;
	case ADC_CHANNEL_IN7:
		return GPIOA;
	case ADC_CHANNEL_IN8:
		return GPIOB;
	case ADC_CHANNEL_IN9:
		return GPIOB;
	case ADC_CHANNEL_IN10:
		return GPIOC;
	case ADC_CHANNEL_IN11:
		return GPIOC;
	case ADC_CHANNEL_IN12:
		return GPIOC;
	case ADC_CHANNEL_IN13:
		return GPIOC;
	case ADC_CHANNEL_IN14:
		return GPIOC;
	case ADC_CHANNEL_IN15:
		return GPIOC;
	default:
		firmwareError(CUSTOM_ERR_ADC_UNKNOWN_CHANNEL, "Unknown hw channel %d [%s]", hwChannel, msg);
		return NULL;
	}
}

// deprecated - migrate to 'getAdcChannelBrainPin'
int getAdcChannelPin(adc_channel_e hwChannel) {
	// todo: replace this with an array :)
	switch (hwChannel) {
	case ADC_CHANNEL_IN0:
		return 0;
	case ADC_CHANNEL_IN1:
		return 1;
	case ADC_CHANNEL_IN2:
		return 2;
	case ADC_CHANNEL_IN3:
		return 3;
	case ADC_CHANNEL_IN4:
		return 4;
	case ADC_CHANNEL_IN5:
		return 5;
	case ADC_CHANNEL_IN6:
		return 6;
	case ADC_CHANNEL_IN7:
		return 7;
	case ADC_CHANNEL_IN8:
		return 0;
	case ADC_CHANNEL_IN9:
		return 1;
	case ADC_CHANNEL_IN10:
		return 0;
	case ADC_CHANNEL_IN11:
		return 1;
	case ADC_CHANNEL_IN12:
		return 2;
	case ADC_CHANNEL_IN13:
		return 3;
	case ADC_CHANNEL_IN14:
		return 4;
	case ADC_CHANNEL_IN15:
		return 5;
	default:
		// todo: better error handling, that's input parameter validation
		firmwareError(CUSTOM_ERR_ADC_CHANNEL, "Unknown hw channel %d", hwChannel);
		return -1;
	}
}

#endif /* HAL_USE_ADC */
