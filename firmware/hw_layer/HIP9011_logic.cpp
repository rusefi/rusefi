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
