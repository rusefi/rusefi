#include "pch.h"

#include "protected_gpio.h"

void setSdCardConfigurationOverrides() {
}

void setBoardConfigOverrides() {
	// No divider, just direct connected analog inputs (they're all internal, so why divide?)
	engineConfiguration->analogInputDividerCoefficient = 1;

	// 10k/100k divider
	engineConfiguration->vbattDividerCoeff = 11;
	engineConfiguration->vbattAdcChannel = EFI_ADC_4;

	engineConfiguration->canTxPin = GPIOA_12;
	engineConfiguration->canRxPin = GPIOA_11;
}

// TODO: these are made up values
static constexpr float AmpsPerVolt = 10;

// instantaneous limit is 100A
// long-term limit is 20A
static constexpr float MaxCurrent = 100;

static const ProtectedGpioConfig cfgs[] = {
	{ GPIOA_0,  EFI_ADC_10, AmpsPerVolt, MaxCurrent },
	{ GPIOA_1,  EFI_ADC_11, AmpsPerVolt, MaxCurrent },
	{ GPIOA_2,  EFI_ADC_12, AmpsPerVolt, MaxCurrent },
	{ GPIOA_3,  EFI_ADC_13, AmpsPerVolt, MaxCurrent },
	{ GPIOC_9,  EFI_ADC_14, AmpsPerVolt, MaxCurrent },
	{ GPIOA_8,  EFI_ADC_15, AmpsPerVolt, MaxCurrent },
	{ GPIOA_9,  EFI_ADC_8,  AmpsPerVolt, MaxCurrent },
	{ GPIOA_10, EFI_ADC_9,  AmpsPerVolt, MaxCurrent },
};

void boardInitHardware() {
	// We recycle GPIOI_0..7 for protected outputs since this is a 64-pin STM32 which has no port I
	protectedGpio_add(GPIOI_0, cfgs);
}
