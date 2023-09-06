#include "pch.h"
#include "hellen_meta.h"

#include "hellen_leds_64.cpp"

void setBoardConfigOverrides() {
	setHellen64Can();
	setHellen64MegaEnPin();
	setHellen64SdCardSpi1();

	// 33k / 6.8k = 5.835
	engineConfiguration->vbattDividerCoeff = (33 + 6.8) / 6.8;
	engineConfiguration->vbattAdcChannel = EFI_ADC_5;

	// Force I/O for Lua - so no TS is required
	engineConfiguration->luaOutputPins[0] = Gpio::B7;
	engineConfiguration->luaOutputPins[1] = Gpio::C8;
	engineConfiguration->luaOutputPins[2] = Gpio::C9;
	engineConfiguration->luaOutputPins[3] = Gpio::A8;

	// TODO: are analog correct?
	engineConfiguration->auxAnalogInputs[0] = EFI_ADC_0;
	engineConfiguration->auxAnalogInputs[1] = EFI_ADC_1;
	engineConfiguration->auxAnalogInputs[2] = EFI_ADC_2;
	engineConfiguration->auxAnalogInputs[3] = EFI_ADC_3;

	// TODO: are digital input correct?
	engineConfiguration->luaDigitalInputPins[0] = Gpio::C2;
	engineConfiguration->luaDigitalInputPins[1] = Gpio::C3;
	engineConfiguration->luaDigitalInputPins[2] = Gpio::C4;
}
