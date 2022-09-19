#include "pch.h"

#include "protected_gpio.h"

void setBoardConfigOverrides() {
	// No divider, just direct connected analog inputs (they're all internal, so why divide?)
	engineConfiguration->analogInputDividerCoefficient = 1;

	// 10k/100k divider
	engineConfiguration->vbattDividerCoeff = 11;
	engineConfiguration->vbattAdcChannel = EFI_ADC_4;

	engineConfiguration->canTxPin = Gpio::A12;
	engineConfiguration->canRxPin = Gpio::A11;
}

// TODO: these are made up values
static constexpr float AmpsPerVolt = 10;

// instantaneous limit is 100A
// long-term limit is 20A
static constexpr float MaxCurrent = 100;

static const ProtectedGpioConfig cfgs[] = {
	{ Gpio::A0,  EFI_ADC_10, AmpsPerVolt, MaxCurrent },
	{ Gpio::A1,  EFI_ADC_11, AmpsPerVolt, MaxCurrent },
	{ Gpio::A2,  EFI_ADC_12, AmpsPerVolt, MaxCurrent },
	{ Gpio::A3,  EFI_ADC_13, AmpsPerVolt, MaxCurrent },
	{ Gpio::C9,  EFI_ADC_14, AmpsPerVolt, MaxCurrent },
	{ Gpio::A8,  EFI_ADC_15, AmpsPerVolt, MaxCurrent },
	{ Gpio::A9,  EFI_ADC_8,  AmpsPerVolt, MaxCurrent },
	{ Gpio::A10, EFI_ADC_9,  AmpsPerVolt, MaxCurrent },
};

void boardInitHardware() {
	protectedGpio_add(Gpio::PROTECTED_PIN_0, cfgs);
}
