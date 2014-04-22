/**
 * @file	honda_accord.c
 *
 * @date Jan 12, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "engine_configuration.h"
#include "trigger_decoder.h"

void setHondaAccordConfiguration(engine_configuration_s *engineConfiguration) {
	engineConfiguration->triggerConfig.totalToothCount = 24;
	engineConfiguration->triggerConfig.skippedToothCount = 2;
}

void setHondaAccordConfiguration2(engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2) {


	EventHandlerConfiguration *config = &engineConfiguration2->engineEventConfiguration;

	float x = 10;

	resetEventList(&config->ignitionEvents);
	registerActuatorEvent(&config->ignitionEvents, 0, addOutputSignal(SPARKOUT_1_OUTPUT), x);
	registerActuatorEvent(&config->ignitionEvents, 0, addOutputSignal(SPARKOUT_1_OUTPUT), x + 180);
	registerActuatorEvent(&config->ignitionEvents, 0, addOutputSignal(SPARKOUT_1_OUTPUT), x + 360);
	registerActuatorEvent(&config->ignitionEvents, 0, addOutputSignal(SPARKOUT_4_OUTPUT), x + 540);
}

