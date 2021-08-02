/**
 * @file board.cpp
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"
#include "boards.h"
#include "engine.h"
#include "engine_sniffer.h"
#include "adc_math.h"

int getAdcValue(const char *msg, int hwChannel) {
	return engine->engineState.mockAdcState.getMockAdcValue(hwChannel);
}

// voltage in MCU universe, from zero to VDD
float getVoltage(const char *msg, adc_channel_e hwChannel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	return adcToVolts(getAdcValue(msg, hwChannel));
}

// Board voltage, with divider coefficient accounted for
float getVoltageDivided(const char *msg, adc_channel_e hwChannel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	return getVoltage(msg, hwChannel) * engineConfiguration->analogInputDividerCoefficient;
}
