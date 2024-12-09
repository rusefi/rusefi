/**
 * @file board.cpp
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "boards.h"
#include "engine_sniffer.h"
#include "adc_math.h"

int adcGetRawValue(const char * /*msg*/, int /*hwChannel*/) {
	return 0;
}

// voltage in MCU universe, from zero to VDD
float getVoltage(const char *msg, adc_channel_e hwChannel) {
	return adcToVolts(adcGetRawValue(msg, hwChannel));
}

// Board voltage, with divider coefficient accounted for
float getVoltageDivided(const char *msg, adc_channel_e hwChannel) {
	return getVoltage(msg, hwChannel) * engineConfiguration->analogInputDividerCoefficient;
}
