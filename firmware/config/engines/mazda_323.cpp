/**
 * @file	mazda_323.cpp
 *
 * @date Mar 8, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "mazda_323.h"

EXTERN_CONFIG;

void setMazda323EngineConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.displacement = 1.6;

	engineConfiguration->ignitionMode = IM_ONE_COIL;

	/**
	 * We treat the trigger as 4/0 toothed wheel
	 */
//	setToothedWheelConfiguration(engineConfiguration, 4, 0);
}
