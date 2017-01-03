/**
 * @file    voltage.cpp
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "main.h"
#include "engine.h"
#include "adc_inputs.h"
#include "voltage.h"

EXTERN_ENGINE;

float getVRef(DECLARE_ENGINE_PARAMETER_F) {
	return getVoltageDivided("vref", engineConfiguration->vRefAdcChannel);
}

bool hasVBatt(DECLARE_ENGINE_PARAMETER_F) {
	return engineConfiguration->vbattAdcChannel != EFI_ADC_NONE;
}

float getVBatt(DECLARE_ENGINE_PARAMETER_F) {
	return getVoltage("vbatt", engineConfiguration->vbattAdcChannel) * engineConfiguration->vbattDividerCoeff;
}
