/**
 * @file	cypress_common.cpp
 * @brief	Low level common Cypress code
 *
 * @date Jan 28, 2020
 * @author  andreika <prometheus.pcb@gmail.com>
 */

#include "pin_repository.h"

#if HAL_USE_ADC || defined(__DOXYGEN__)

// ADC_CHANNEL_IN0 // PA2   (def=VIGN)
// ADC_CHANNEL_IN1 // PA3   (def=MAP4)
// ADC_CHANNEL_IN2 // x
// ADC_CHANNEL_IN3 // PD3   (def=MAP3)
// ADC_CHANNEL_IN4 // x
// ADC_CHANNEL_IN5 // x
// ADC_CHANNEL_IN6 // x
// ADC_CHANNEL_IN7 // PB12  (def=MAP2)
// ADC_CHANNEL_IN8 // PB13  (def=MAP1)
// ADC_CHANNEL_IN9 // x
// ADC_CHANNEL_IN10 // PE2  (def=O2S2)
// ADC_CHANNEL_IN11 // x
// ADC_CHANNEL_IN12 // PC14 (def=O2S)
// ADC_CHANNEL_IN13 // PC15 (def=TPS)
// ADC_CHANNEL_IN14 // PC16 (def=CLT)
// ADC_CHANNEL_IN15 // PC17 (def=IAT)

brain_pin_e getAdcChannelBrainPin(const char *msg, adc_channel_e hwChannel) {
	// todo: replace this with an array :)
	switch (hwChannel) {
	case EFI_ADC_0:
		return GPIOB_0;
	case EFI_ADC_1:
		return GPIOB_1;
	case EFI_ADC_2:
		return GPIOB_2;
	case EFI_ADC_3:
		return GPIOB_3;
	case EFI_ADC_4:
		return GPIOB_4;
	case EFI_ADC_5:
		return GPIOB_5;
	case EFI_ADC_6:
		return GPIO_INVALID;
	case EFI_ADC_7:
		return GPIO_INVALID;
	case EFI_ADC_8:
		return GPIOB_8;
	case EFI_ADC_9:
		return GPIOB_9;
	case EFI_ADC_10:
		return GPIO_INVALID;
	case EFI_ADC_11:
		return GPIO_INVALID;
	case EFI_ADC_12:
		return GPIOB_12;
	case EFI_ADC_13:
		return GPIOB_13;
	case EFI_ADC_14:
		return GPIOB_14;
	case EFI_ADC_15:
		return GPIOB_15;
	case EFI_ADC_16:
		return GPIO_INVALID;
	case EFI_ADC_17:
		return GPIO_INVALID;
	case EFI_ADC_18:
		return GPIO_INVALID;
	case EFI_ADC_19:
		return GPIO_INVALID;
	case EFI_ADC_20:
		return GPIO_INVALID;
	case EFI_ADC_21:
		return GPIO_INVALID;
	case EFI_ADC_22:
		return GPIO_INVALID;
	case EFI_ADC_23:
		return GPIO_INVALID;
	case EFI_ADC_24:
		return GPIOC_10;
	case EFI_ADC_25:
		return GPIOC_9;
	case EFI_ADC_26:
		return GPIOC_8;
	case EFI_ADC_27:
		return GPIOC_7;
	case EFI_ADC_28:
		return GPIOC_5;
	case EFI_ADC_29:
		return GPIOC_4;
	case EFI_ADC_30:
		return GPIOC_3;
	case EFI_ADC_31:
		return GPIOC_2;
	default:
		firmwareError(CUSTOM_ERR_ADC_UNKNOWN_CHANNEL, "Unknown hw channel %d [%s]", hwChannel, msg);
		return GPIO_INVALID;
	}
}

adc_channel_e getAdcChannel(brain_pin_e pin) {
	switch (pin) {
	case GPIOB_0:
		return EFI_ADC_0;
	case GPIOB_1:
		return EFI_ADC_1;
	case GPIOB_2:
		return EFI_ADC_2;
	case GPIOB_3:
		return EFI_ADC_3;
	case GPIOB_4:
		return EFI_ADC_4;
	case GPIOB_5:
		return EFI_ADC_5;
	case GPIOB_8:
		return EFI_ADC_8;
	case GPIOB_9:
		return EFI_ADC_9;
	case GPIOB_12:
		return EFI_ADC_12;
	case GPIOB_13:
		return EFI_ADC_13;
	case GPIOB_14:
		return EFI_ADC_14;
	case GPIOB_15:
		return EFI_ADC_15;
	case GPIOC_10:
		return EFI_ADC_24;
	case GPIOC_9:
		return EFI_ADC_25;
	case GPIOC_8:
		return EFI_ADC_26;
	case GPIOC_7:
		return EFI_ADC_27;
	case GPIOC_5:
		return EFI_ADC_28;
	case GPIOC_4:
		return EFI_ADC_29;
	case GPIOC_3:
		return EFI_ADC_30;
	case GPIOC_2:
		return EFI_ADC_31;
	default:
		return EFI_ADC_ERROR;
	}
}

// deprecated - migrate to 'getAdcChannelBrainPin'
ioportid_t getAdcChannelPort(const char *msg, adc_channel_e hwChannel) {
	return getHwPort(msg, getAdcChannelBrainPin(msg, hwChannel));
}

// deprecated - migrate to 'getAdcChannelBrainPin'
int getAdcChannelPin(adc_channel_e hwChannel) {
	return getHwPin("get_pin", getAdcChannelBrainPin("get_pin", hwChannel));
}

#endif /* HAL_USE_ADC */

#if EFI_PROD_CODE
void jump_to_bootloader() {
	// todo:
	// Will not return from here
	NVIC_SystemReset();
}
#endif /* EFI_PROD_CODE */
