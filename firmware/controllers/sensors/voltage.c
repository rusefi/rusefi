/**
 * @file    voltage.c
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "boards.h"
#include "engine_configuration.h"
#include "adc_inputs.h"
#include "voltage.h"

extern engine_configuration_s *engineConfiguration;

float getVRef(void) {
//	return getAdcValue(ADC_CHANNEL_VREF);
	return getVoltageDivided(ADC_CHANNEL_VREF);
}

float getVBatt(engine_configuration_s *engineConfiguration) {
	return getVoltage(engineConfiguration->vbattAdcChannel) * engineConfiguration->vbattDividerCoeff;
}
