/*
 * @file HIP9011_logic.cpp
 *
 *  Created on: Jan 3, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "HIP9011_logic.h"

EXTERN_ENGINE;

HIP9011::HIP9011() {
	needToInit = true;
	state = NOT_READY;
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

