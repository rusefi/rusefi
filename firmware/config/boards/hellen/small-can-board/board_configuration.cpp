#include "pch.h"
#include "hellen_meta.h"

#include "hellen_leds_64.cpp"
#include "board_overrides.h"

static void small_can_board_boardConfigOverrides() {
	setHellen64Can();
	setHellen64MegaEnPin();
	setHellen64SdCardSpi();

  // not stm32 CAN DFU configuration :(
	engineConfiguration->can2RxPin = Gpio::B5;
 	engineConfiguration->can2TxPin = Gpio::B6;

	setHellenAnalogDividers();
	engineConfiguration->vbattAdcChannel = EFI_ADC_11;
}

static void small_can_board_boardDefaultConfiguration() {
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

void setup_custom_board_overrides() {
	custom_board_DefaultConfiguration = small_can_board_boardDefaultConfiguration;
	custom_board_ConfigOverrides = small_can_board_boardConfigOverrides;
}
