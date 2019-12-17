/**
 * @file    voltage.cpp
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
#include "engine.h"
#include "adc_inputs.h"
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
#ifdef USE_ADC3_VBATT_HACK
	extern adcsample_t vbattSampleProteus;
	adcsample_t sample = vbattSampleProteus;
#else
	adcsample_t sample = getAdcValue("vbatt", engineConfiguration->vbattAdcChannel PASS_ENGINE_PARAMETER_SUFFIX);
#endif

	return adcToVolts(sample) * engineConfiguration->vbattDividerCoeff;
}
