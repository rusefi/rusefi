/**
 * @file	test_engine.cpp
 *
 * set_engine_type 26
 *
 * @date Nov 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"
#include "test_engine.h"

void setTestEngineConfiguration(engine_configuration_s *engineConfiguration) {
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;

	trigger_config_s *triggerConfig = &engineConfiguration->trigger;
	triggerConfig->customIsSynchronizationNeeded = false;
	triggerConfig->customTotalToothCount = 60;
	triggerConfig->customSkippedToothCount = 0;

	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);

	engineConfiguration->ignitionMode = IM_ONE_COIL;
	setConstantDwell(engineConfiguration, 3); // 50% duty cycle @ 5000 rpm

	board_configuration_s *bc = &engineConfiguration->bc;
	bc->malfunctionIndicatorPin = GPIO_UNASSIGNED;

	bc->ignitionPins[0] = GPIOC_7; // #1
	bc->ignitionPins[1] = GPIO_UNASSIGNED; // #2
	bc->ignitionPins[2] = GPIO_UNASSIGNED; // #3
	bc->ignitionPins[3] = GPIO_UNASSIGNED; // #4
	bc->ignitionPins[4] = GPIO_UNASSIGNED; // #5
	bc->ignitionPins[5] = GPIO_UNASSIGNED; // #6

	bc->logicAnalyzerPins[0] = GPIO_UNASSIGNED;
	bc->logicAnalyzerPins[1] = GPIO_UNASSIGNED;
	bc->logicAnalyzerPins[2] = GPIO_UNASSIGNED;
	bc->logicAnalyzerPins[3] = GPIO_UNASSIGNED;

	engineConfiguration->hasCltSensor = false;
	engineConfiguration->hasIatSensor = false;

}
