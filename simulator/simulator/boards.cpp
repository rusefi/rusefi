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
expected<float> adcGetRawVoltage(const char *msg, adc_channel_e hwChannel) {
	return expected(adcRawValueToRawVoltage(adcGetRawValue(msg, hwChannel)));
}

// voltage in ECU universe, with all input dividers and OpAmps gains taken into account, voltage at ECU connector pin
expected<float> adcGetScaledVoltage(const char *msg, adc_channel_e hwChannel) {
	auto rawVoltage = adcGetRawVoltage(msg, hwChannel);

	if (rawVoltage) {
		return expected(rawVoltage.value_or(0) * engineConfiguration->analogInputDividerCoefficient);
	}

	return expected(rawVoltage);
}
