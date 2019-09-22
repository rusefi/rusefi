/**
 * @file board.cpp
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
#include "boards.h"
#include "engine.h"
#include "engine_sniffer.h"
#include "adc_math.h"

static LoggingWithStorage logger("simulator board");

EXTERN_ENGINE;

int getAdcValue(const char *msg, int hwChannel) {
	return engine->engineState.mockAdcState.getMockAdcValue(hwChannel);
}

// Board voltage, with divider coefficient accounted for
float getVoltageDivided(const char *msg, adc_channel_e hwChannel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	return getVoltage(msg, hwChannel) * engineConfiguration->analogInputDividerCoefficient;
}
