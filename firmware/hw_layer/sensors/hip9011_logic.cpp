/*
 * @file HIP9011_logic.cpp
 *
 *  Created on: Jan 3, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"
#include "hip9011_logic.h"

EXTERN_ENGINE;

HIP9011::HIP9011(Hip9011HardwareInterface *hardware) : rpmLookup() {
	this->hw = hardware;
}

int HIP9011::sendCommand(uint8_t cmd) {
	return hw->sendSyncCommand(cmd, NULL);
}

/**
 * @return frequency band we are interested in
 */
float HIP9011::getBand(DEFINE_HIP_PARAMS) {
	return GET_CONFIG_VALUE(knockBandCustom) == 0 ?
			HIP9011_BAND(GET_CONFIG_VALUE(cylinderBore)) :
			GET_CONFIG_VALUE(knockBandCustom);
}

int HIP9011::getBandIndex(DEFINE_HIP_PARAMS) {
	float freq = getBand(FORWARD_HIP_PARAMS);
	return getHip9011BandIndex(freq);
}

int HIP9011::getGainIndex(DEFINE_HIP_PARAMS) {
	int i = GAIN_INDEX(GET_CONFIG_VALUE(hip9011Gain));
	// GAIN_LOOKUP_SIZE is returned for index which is too low
	return i == GAIN_LOOKUP_SIZE ? GAIN_LOOKUP_SIZE - 1 : i;
}

/**
 *
 * We know the set of possible integration times, we know the knock detection window width
 */
void HIP9011::prepareRpmLookup(void) {
	/**
	 * out binary search method needs increasing order thus the reverse order here
	 */
	for (int i = 0; i < INT_LOOKUP_SIZE; i++) {
		rpmLookup[i] = getRpmByAngleWindowAndTimeUs(integratorValues[INT_LOOKUP_SIZE - i - 1], angleWindowWidth);
	}
}

int HIP9011::getIntegrationIndexByRpm(float rpm) {
	int i = findIndexMsg("getIbR", rpmLookup, INT_LOOKUP_SIZE, (rpm));
	return i == -1 ? INT_LOOKUP_SIZE - 1 : INT_LOOKUP_SIZE - i - 1;
}

void HIP9011::setAngleWindowWidth(DEFINE_HIP_PARAMS) {
	float new_angleWindowWidth =
		GET_CONFIG_VALUE(knockDetectionWindowEnd) -
		GET_CONFIG_VALUE(knockDetectionWindowStart);
	if (new_angleWindowWidth < 0) {
#if EFI_PROD_CODE
		warning(CUSTOM_KNOCK_WINDOW, "invalid knock window");
#endif
		new_angleWindowWidth = 0;
	}
	// float '==' is totally appropriate here
	if (angleWindowWidth == new_angleWindowWidth)
		return; // exit if value has not change
	angleWindowWidth = new_angleWindowWidth;
	prepareRpmLookup();
}

void HIP9011::handleSettings(int rpm DEFINE_PARAM_SUFFIX(DEFINE_HIP_PARAMS)) {
	int ret;

	setAngleWindowWidth(FORWARD_HIP_PARAMS);

	int new_prescaler = GET_CONFIG_VALUE(hip9011PrescalerAndSDO);
	int new_integratorIdx = getIntegrationIndexByRpm(rpm);
	int new_gainIdx = getGainIndex(FORWARD_HIP_PARAMS);
	int new_bandIdx = getBandIndex(FORWARD_HIP_PARAMS);

	if (gainIdx != new_gainIdx) {
		ret = sendCommand(SET_GAIN_CMD(new_gainIdx));
		if (ret == 0)
			gainIdx = new_gainIdx;
	}
	if (intergratorIdx != new_integratorIdx) {
		ret = sendCommand(SET_INTEGRATOR_CMD(new_integratorIdx));
		if (ret == 0)
			intergratorIdx = new_integratorIdx;
	}
	if (bandIdx != new_bandIdx) {
		ret = sendCommand(SET_BAND_PASS_CMD(new_bandIdx));
		if (ret == 0)
			bandIdx = new_bandIdx;
	}
	if (prescaler != new_prescaler) {
		ret = sendCommand(SET_PRESCALER_CMD(new_prescaler));
		if (ret == 0)
			prescaler = new_prescaler;
	}
}
