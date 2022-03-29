#include "pch.h"

void setSdCardConfigurationOverrides() {
}

void setBoardConfigOverrides() {
	// No divider, just direct connected analog inputs (they're all internal, so why divide?)
	engineConfiguration->analogInputDividerCoefficient = 1;

	// 10k/100k divider
	engineConfiguration->vbattDividerCoeff = 11;
	engineConfiguration->vbattAdcChannel = EFI_ADC_4;
}
