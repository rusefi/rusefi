#include "pch.h"
#include "hellen_meta.h"

#include "hellen_leds_64.cpp"

void setBoardConfigOverrides() {
	setHellen64Can();
	setHellen64MegaEnPin();
	setHellen64SdCardSpi();

  // not stm32 CAN DFU configuration :(
	engineConfiguration->can2RxPin = Gpio::B5;
 	engineConfiguration->can2TxPin = Gpio::B6;

	// 4.7k high side/4.7k low side = 2.0 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 2.0f;

	// 33k / 6.8k = 5.835
	engineConfiguration->vbattDividerCoeff = (33 + 6.8) / 6.8;
	engineConfiguration->vbattAdcChannel = EFI_ADC_11;
}

void setBoardDefaultConfiguration() {
	engineConfiguration->luaOutputPins[0] = Gpio::MM64_OUT_PWM1;
	engineConfiguration->luaOutputPins[1] = Gpio::MM64_OUT_PWM2;
	engineConfiguration->luaOutputPins[2] = Gpio::MM64_OUT_PWM3;
	engineConfiguration->luaOutputPins[3] = Gpio::MM64_OUT_PWM4;

	engineConfiguration->auxAnalogInputs[0] = MM64_IN_ADC2_ANALOG;
	engineConfiguration->auxAnalogInputs[1] = MM64_IN_ADC3_ANALOG;
	engineConfiguration->auxAnalogInputs[2] = MM64_IN_ADC4_ANALOG;
	engineConfiguration->auxAnalogInputs[3] = MM64_IN_ADC5_ANALOG;
	engineConfiguration->auxAnalogInputs[4] = MM64_IN_ADC6_ANALOG;
}
