/**
 * @file board.c
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "boards.h"
#include "engine.h"

// todo: migrate to engine->engineState.mockAdcState
float testMafValue = 0;
float testCltValue = 0;
float testIatValue = 0;

float getVoltageDivided(const char *msg, adc_channel_e hwChannel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	switch(hwChannel) {
	case TEST_MAF_CHANNEL:
		return testMafValue;
	case TEST_CLT_CHANNEL:
		return testCltValue;
		//return adcToVolts(engine->engineState.mockAdcState.getMockAdcValue(hwChannel));
	case TEST_IAT_CHANNEL:
		return testIatValue;
		//return adcToVolts(engine->engineState.mockAdcState.getMockAdcValue(hwChannel));
	}
	return 0;
}

float getVoltage(const char *msg, adc_channel_e hwChannel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (engine->engineState.mockAdcState.hasMockAdc[hwChannel])
		return adcToVolts(engine->engineState.mockAdcState.getMockAdcValue(hwChannel) * engineConfiguration->analogInputDividerCoefficient);
	return 0;
}

int getAdcValue(const char *msg, adc_channel_e hwChannel) {
	return 0;
}


