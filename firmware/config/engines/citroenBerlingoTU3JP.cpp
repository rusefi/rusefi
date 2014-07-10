/**
 * @file	citroenBerlingoTU3JP.cpp
 *
 * CITROEN_TU3JP: engine_type 15
 *
 * This config overrides some values of the default configuration which is set by setDefaultConfiguration() method
 *
 *
 * @date Apr 15, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "citroenBerlingoTU3JP.h"

void setCitroenBerlingoTU3JPConfiguration(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration) {
	engineConfiguration->engineType = CITROEN_TU3JP;

	//engineConfiguration->triggerConfig.triggerType = todo 60_2

	setToothedWheelConfiguration(engineConfiguration, 60, 2);

	setTriggerSynchronizationGap(&engineConfiguration->triggerConfig, 2.5);


	// set_cranking_injection_mode 0
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	// set_injection_mode 2
	engineConfiguration->injectionMode = IM_BATCH;
	// set_ignition_mode 2
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	// set_firing_order 2
	engineConfiguration->firingOrder = FO_1_THEN_3_THEN_4_THEN2;


}

