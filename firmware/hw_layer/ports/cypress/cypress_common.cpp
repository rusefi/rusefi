/**
 * @file	cypress_common.cpp
 * @brief	Low level common Cypress code
 *
 * @date Jan 28, 2020
 * @author  andreika <prometheus.pcb@gmail.com>
 */

#include "pch.h"

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
		return Gpio::B0;
	case EFI_ADC_1:
		return Gpio::B1;
	case EFI_ADC_2:
		return Gpio::B2;
	case EFI_ADC_3:
		return Gpio::B3;
	case EFI_ADC_4:
		return Gpio::B4;
	case EFI_ADC_5:
		return Gpio::B5;
	case EFI_ADC_6:
		return Gpio::Invalid;
	case EFI_ADC_7:
		return Gpio::Invalid;
	case EFI_ADC_8:
		return Gpio::B8;
	case EFI_ADC_9:
		return Gpio::B9;
	case EFI_ADC_10:
		return Gpio::Invalid;
	case EFI_ADC_11:
		return Gpio::Invalid;
	case EFI_ADC_12:
		return Gpio::B12;
	case EFI_ADC_13:
		return Gpio::B13;
	case EFI_ADC_14:
		return Gpio::B14;
	case EFI_ADC_15:
		return Gpio::B15;
	case EFI_ADC_16:
		return Gpio::Invalid;
	case EFI_ADC_17:
		return Gpio::Invalid;
	case EFI_ADC_18:
		return Gpio::Invalid;
	case EFI_ADC_19:
		return Gpio::Invalid;
	case EFI_ADC_20:
		return Gpio::Invalid;
	case EFI_ADC_21:
		return Gpio::Invalid;
	case EFI_ADC_22:
		return Gpio::Invalid;
	case EFI_ADC_23:
		return Gpio::Invalid;
	case EFI_ADC_24:
		return Gpio::C10;
	case EFI_ADC_25:
		return Gpio::C9;
	case EFI_ADC_26:
		return Gpio::C8;
	case EFI_ADC_27:
		return Gpio::C7;
	case EFI_ADC_28:
		return Gpio::C5;
	case EFI_ADC_29:
		return Gpio::C4;
	case EFI_ADC_30:
		return Gpio::C3;
	case EFI_ADC_31:
		return Gpio::C2;
	default:
		firmwareError(ObdCode::CUSTOM_ERR_ADC_UNKNOWN_CHANNEL, "Unknown hw channel %d [%s]", hwChannel, msg);
		return Gpio::Invalid;
	}
}

adc_channel_e getAdcChannel(brain_pin_e pin) {
	switch (pin) {
	case Gpio::B0:
		return EFI_ADC_0;
	case Gpio::B1:
		return EFI_ADC_1;
	case Gpio::B2:
		return EFI_ADC_2;
	case Gpio::B3:
		return EFI_ADC_3;
	case Gpio::B4:
		return EFI_ADC_4;
	case Gpio::B5:
		return EFI_ADC_5;
	case Gpio::B8:
		return EFI_ADC_8;
	case Gpio::B9:
		return EFI_ADC_9;
	case Gpio::B12:
		return EFI_ADC_12;
	case Gpio::B13:
		return EFI_ADC_13;
	case Gpio::B14:
		return EFI_ADC_14;
	case Gpio::B15:
		return EFI_ADC_15;
	case Gpio::C10:
		return EFI_ADC_24;
	case Gpio::C9:
		return EFI_ADC_25;
	case Gpio::C8:
		return EFI_ADC_26;
	case Gpio::C7:
		return EFI_ADC_27;
	case Gpio::C5:
		return EFI_ADC_28;
	case Gpio::C4:
		return EFI_ADC_29;
	case Gpio::C3:
		return EFI_ADC_30;
	case Gpio::C2:
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
