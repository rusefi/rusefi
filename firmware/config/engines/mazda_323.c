/**
 * @file	mazda_323.c
 *
 * @date Mar 8, 2014
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#include "mazda_323.h"

void setMazda323EngineConfiguration(engine_configuration_s *engineConfiguration) {
	engineConfiguration->cylindersCount = 6;

	engineConfiguration->ignitionMode = IM_ONE_COIL;

	/**
	 * We treat the trigger as 4/0 toothed wheel
	 */
	engineConfiguration->triggerConfig.totalToothCount = 4;
	engineConfiguration->triggerConfig.skippedToothCount = 0;
	engineConfiguration->triggerConfig.isSynchronizationNeeded = FALSE;

}

void setMazda323configuration2_s(engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2) {

}
