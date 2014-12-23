/**
 * @file    voltage.cpp
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "engine_configuration.h"
#include "adc_inputs.h"
#include "voltage.h"

extern engine_configuration_s *engineConfiguration;

float getVRef(engine_configuration_s *engineConfiguration) {
	return getVoltageDivided(engineConfiguration->vRefAdcChannel);
}

float getVBatt(engine_configuration_s *engineConfiguration) {
	return getVoltage(engineConfiguration->vbattAdcChannel) * engineConfiguration->vbattDividerCoeff;
}
