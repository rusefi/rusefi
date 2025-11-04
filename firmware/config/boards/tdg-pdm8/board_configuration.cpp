#include "pch.h"

#include "protected_gpio.h"
#include "board_overrides.h"

Gpio getCommsLedPin() {
	// this board has no comms led
	return Gpio::Unassigned;
}

Gpio getWarningLedPin() {
	// this board has no warning led
	return Gpio::Unassigned;
}

Gpio getRunningLedPin() {
	// this board has no running led
	return Gpio::Unassigned;
}

static void tdg_pdm8_boardConfigOverrides() {
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

static void tdg_pdm8_boardInitHardware() {
	protectedGpio_add(Gpio::PROTECTED_PIN_0, cfgs);
}

void setup_custom_board_overrides() {
	custom_board_InitHardware = tdg_pdm8_boardInitHardware;
	custom_board_ConfigOverrides = tdg_pdm8_boardConfigOverrides;
}

