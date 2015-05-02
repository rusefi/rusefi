/**
 * @file    voltage.cpp
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"
#include "engine_configuration.h"
#include "adc_inputs.h"
#include "voltage.h"

float getVRef(engine_configuration_s *engineConfiguration) {
	return getVoltageDivided("vref", engineConfiguration->vRefAdcChannel);
}

float getVBatt(engine_configuration_s *engineConfiguration) {
	return getVoltage("vbatt", engineConfiguration->vbattAdcChannel) * engineConfiguration->vbattDividerCoeff;
}
