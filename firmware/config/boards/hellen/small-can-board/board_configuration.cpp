#include "pch.h"
#include "hellen_meta.h"

#include "hellen_leds_64.cpp"

void setBoardConfigOverrides() {
	setHellen64Can();
	setHellen64MegaEnPin();
	setHellen64SdCardSpi1();

	// 4.7k high side/4.7k low side = 2.0 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 2.0f;

	// 33k / 6.8k = 5.835
	engineConfiguration->vbattDividerCoeff = (33 + 6.8) / 6.8;
	engineConfiguration->vbattAdcChannel = EFI_ADC_11;

	// Force I/O for Lua - so no TS is required
	engineConfiguration->luaOutputPins[0] = H64_OUT_PWM1;
	engineConfiguration->luaOutputPins[1] = H64_OUT_PWM2;
	engineConfiguration->luaOutputPins[2] = H64_OUT_PWM3;
	engineConfiguration->luaOutputPins[3] = H64_OUT_PWM4;

	engineConfiguration->auxAnalogInputs[0] = EFI_ADC_2;
	engineConfiguration->auxAnalogInputs[1] = EFI_ADC_3;
	engineConfiguration->auxAnalogInputs[2] = EFI_ADC_4;
	engineConfiguration->auxAnalogInputs[3] = EFI_ADC_5;
	engineConfiguration->auxAnalogInputs[4] = EFI_ADC_6;
	engineConfiguration->auxAnalogInputs[5] = EFI_ADC_7;
	engineConfiguration->auxAnalogInputs[6] = EFI_ADC_14;
	engineConfiguration->auxAnalogInputs[7] = EFI_ADC_15;
}
