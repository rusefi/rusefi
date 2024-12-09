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
float adcGetRawVoltage(const char *msg, adc_channel_e hwChannel) {
	return adcRawValueToRawVoltage(adcGetRawValue(msg, hwChannel));
}

// voltage in ECU universe, with all input dividers and OpAmps gains taken into account, voltage at ECU connector pin
float adcGetScaledVoltage(const char *msg, adc_channel_e hwChannel) {
	return adcGetRawVoltage(msg, hwChannel) * engineConfiguration->analogInputDividerCoefficient;
}
