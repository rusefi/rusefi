/**
 * @file	kinetis_common.cpp
 * @brief	Low level common Kinetis code
 *
 * @date Mar 28, 2019
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
	static_assert(EFI_ADC_NONE == ADC_CHANNEL_NONE);

	// todo: replace this with an array :)
	switch (hwChannel) {
	case EFI_ADC_0:
		return Gpio::A2;
	case EFI_ADC_1:
		return Gpio::A3;
	case EFI_ADC_2:
		return Gpio::Invalid;
	case EFI_ADC_3:
		return Gpio::D3;
	case EFI_ADC_4:
		return Gpio::Invalid;
	case EFI_ADC_5:
		return Gpio::Invalid;
	case EFI_ADC_6:
		return Gpio::Invalid;
	case EFI_ADC_7:
		return Gpio::B12;
	case EFI_ADC_8:
		return Gpio::B13;
	case EFI_ADC_9:
		return Gpio::Invalid;
	case EFI_ADC_10:
		return Gpio::E2;
	case EFI_ADC_11:
		return Gpio::Invalid;
	case EFI_ADC_12:
		return Gpio::C14;
	case EFI_ADC_13:
		return Gpio::C15;
	case EFI_ADC_14:
		return Gpio::C16;
	case EFI_ADC_15:
		return Gpio::C17;
	default:
		firmwareError(ObdCode::CUSTOM_ERR_ADC_UNKNOWN_CHANNEL, "Unknown hw channel %d [%s]", hwChannel, msg);
		return Gpio::Invalid;
	}
}

adc_channel_e getAdcChannel(brain_pin_e pin) {
	switch (pin) {
	case Gpio::A2:
		return EFI_ADC_0;
	case Gpio::A3:
		return EFI_ADC_1;
	//case Gpio::A2:
	//	return EFI_ADC_2;
	case Gpio::D3:
		return EFI_ADC_3;
	//case Gpio::A4:
	//	return EFI_ADC_4;
	//case Gpio::A5:
	//	return EFI_ADC_5;
	//case Gpio::A6:
	//	return EFI_ADC_6;
	case Gpio::B12:
		return EFI_ADC_7;
	case Gpio::B13:
		return EFI_ADC_8;
	//case Gpio::B1:
	//	return EFI_ADC_9;
	case Gpio::E2:
		return EFI_ADC_10;
	//case Gpio::C1:
	//	return EFI_ADC_11;
	case Gpio::C14:
		return EFI_ADC_12;
	case Gpio::C15:
		return EFI_ADC_13;
	case Gpio::C16:
		return EFI_ADC_14;
	case Gpio::C17:
		return EFI_ADC_15;
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
#define BOOTLOADER_LOCATION 0x1C00001CUL
void jump_to_bootloader() {
	typedef void (*bootloader_start_t)(void * arg);
	// Read the function address from the ROM API tree and turn it into a function pointer
	bootloader_start_t bootloaderStart = (bootloader_start_t)(**(uint32_t **)BOOTLOADER_LOCATION);
	// Call the function!
	bootloaderStart(NULL);
	// Will not return from here
	NVIC_SystemReset();
}
#endif /* EFI_PROD_CODE */
