/**
 * @file board.cpp
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "boards.h"

// see setMockVoltage
float getVoltageDivided(const char *msg, adc_channel_e hwChannel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	return adcToVolts(engine->engineState.mockAdcState.getMockAdcValue(hwChannel));;
}

float getVoltage(const char *msg, adc_channel_e hwChannel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (engine->engineState.mockAdcState.hasMockAdc[hwChannel])
		return adcToVolts(engine->engineState.mockAdcState.getMockAdcValue(hwChannel) * engineConfiguration->analogInputDividerCoefficient);
	return 0;
}

int getAdcValue(const char *msg, adc_channel_e hwChannel) {
	return 0;
}


