/**
 * @file board.c
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "boards.h"
#include "engine.h"

// todo: migrate to engine->engineState.mockAdcState
float testMafValue = 0;

// see setMockVoltage
float getVoltageDivided(const char *msg, adc_channel_e hwChannel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	switch(hwChannel) {
	case TEST_MAF_CHANNEL:
		return testMafValue;
	default:
		return adcToVolts(engine->engineState.mockAdcState.getMockAdcValue(hwChannel));;
	}
}

float getVoltage(const char *msg, adc_channel_e hwChannel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (engine->engineState.mockAdcState.hasMockAdc[hwChannel])
		return adcToVolts(engine->engineState.mockAdcState.getMockAdcValue(hwChannel) * engineConfiguration->analogInputDividerCoefficient);
	return 0;
}

int getAdcValue(const char *msg, adc_channel_e hwChannel) {
	return 0;
}


