/**
 * @file	citroenBerlingoTU3JP.cpp
 *
 * CITROEN_TU3JP 15
 *
 * @date Apr 15, 2014
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#include "citroenBerlingoTU3JP.h"

void setCitroenBerlingoTU3JPConfiguration(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration) {
	engineConfiguration->engineType = CITROEN_TU3JP;

	engineConfiguration->triggerConfig.triggerType = TT_TOOTHED_WHEEL;
	engineConfiguration->triggerConfig.totalToothCount = 60;
	engineConfiguration->triggerConfig.skippedToothCount = 2;


	// set_cranking_injection_mode 0
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	// set_injection_mode 2
	engineConfiguration->injectionMode = IM_BATCH;
	// set_ignition_mode 2
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	// set_firing_order 2
	engineConfiguration->firingOrder = FO_1_THEN_3_THEN_4_THEN2;


}

