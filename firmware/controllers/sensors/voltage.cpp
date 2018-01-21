/**
 * @file    voltage.cpp
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
#include "engine.h"
#include "analog_input.h"
#include "voltage.h"

EXTERN_ENGINE;

float getVRef(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
// not currently used	return getVoltageDivided("vref", engineConfiguration->vRefAdcChannel);
	return NAN;
}

bool hasVBatt(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return engineConfiguration->vbattAdcChannel != EFI_ADC_NONE;
}

float getVBatt(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return getVoltage("vbatt", engineConfiguration->vbattAdcChannel) * engineConfiguration->vbattDividerCoeff;
}
