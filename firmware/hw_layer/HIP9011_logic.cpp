/*
 * @file HIP9011_logic.cpp
 *
 *  Created on: Jan 3, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "global.h"
#include "HIP9011_logic.h"

EXTERN_ENGINE;

HIP9011::HIP9011(Hip9011HardwareInterface *hardware) : rpmLookup() {
	this->hardware = hardware;
}

void HIP9011::setStateAndCommand(unsigned char cmd) {
	this->state = IS_SENDING_SPI_COMMAND;
	hardware->sendCommand(cmd);
}

#define BAND(bore) (900 / (PIF * (bore) / 2))

/**
 * @return frequency band we are interested in
 */
float getHIP9011Band(DEFINE_HIP_PARAMS) {
	return GET_CONFIG_VALUE(knockBandCustom) == 0 ?
			BAND(GET_CONFIG_VALUE(cylinderBore)) : GET_CONFIG_VALUE(knockBandCustom);
}

int getBandIndex(DEFINE_HIP_PARAMS) {
	float freq = getHIP9011Band(FORWARD_HIP_PARAMS);
	return getHip9011BandIndex(freq);
}

int getHip9011GainIndex(DEFINE_HIP_PARAMS) {
	int i = GAIN_INDEX(GET_CONFIG_VALUE(hip9011Gain));
	// GAIN_LOOKUP_SIZE is returned for index which is too low
	return i == GAIN_LOOKUP_SIZE ? GAIN_LOOKUP_SIZE - 1 : i;
}

int getHip9011GainIndex(float gain) {
	int i = GAIN_INDEX(gain);
	// GAIN_LOOKUP_SIZE is returned for index which is too low
	return i == GAIN_LOOKUP_SIZE ? GAIN_LOOKUP_SIZE - 1 : i;
}

/**
 *
 * We know the set of possible integration times, we know the knock detection window width
 */
void HIP9011::prepareHip9011RpmLookup(float angleWindowWidth) {
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
	float angleWindowWidth = GET_CONFIG_VALUE(knockDetectionWindowEnd) - GET_CONFIG_VALUE(knockDetectionWindowStart);
	if (angleWindowWidth < 0) {
#if EFI_PROD_CODE
		warning(CUSTOM_ERR_6697, "invalid knock window");
#endif
		angleWindowWidth = 0;
	}
	// float '==' is totally appropriate here
	if (this->angleWindowWidth == angleWindowWidth)
		return; // exit if value has not change
	this->angleWindowWidth = angleWindowWidth;
	prepareHip9011RpmLookup(angleWindowWidth);
}

void HIP9011::handleValue(int rpm DEFINE_PARAM_SUFFIX(DEFINE_HIP_PARAMS)) {
	setAngleWindowWidth(FORWARD_HIP_PARAMS);

	int prescalerIndex = GET_CONFIG_VALUE(hip9011PrescalerAndSDO);
	int integratorIndex = getIntegrationIndexByRpm(rpm);
	int gainIndex = getHip9011GainIndex(FORWARD_HIP_PARAMS);
	int bandIndex = getBandIndex(FORWARD_HIP_PARAMS);


	if (currentGainIndex != gainIndex) {
		currentGainIndex = gainIndex;
		setStateAndCommand(SET_GAIN_CMD + gainIndex);

	} else if (currentIntergratorIndex != integratorIndex) {
		currentIntergratorIndex = integratorIndex;
		setStateAndCommand(SET_INTEGRATOR_CMD + integratorIndex);
	} else if (currentBandIndex != bandIndex) {
		currentBandIndex = bandIndex;
		setStateAndCommand(SET_BAND_PASS_CMD + bandIndex);
	} else if (currentPrescaler != prescalerIndex) {
		currentPrescaler = prescalerIndex;
		setStateAndCommand(SET_PRESCALER_CMD + prescalerIndex);

	} else {
		state = READY_TO_INTEGRATE;
	}

}
