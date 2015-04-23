/**
 * @file	GY6_139QMB.cpp
 * @brief	139qmb default engine configuration
 *
 * @date Feb 13, 2014
 * @author rus084
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"
#include "GY6_139QMB.h"
#include "engine_math.h"

void setGy6139qmbDefaultEngineConfiguration(
		engine_configuration_s *engineConfiguration) {
	board_configuration_s *boardConfiguration = &engineConfiguration->bc;

	engineConfiguration->rpmHardLimit = 9000;
	engineConfiguration->cranking.rpm = 1100;
	engineConfiguration->analogInputDividerCoefficient = 1.52;
	engineConfiguration->algorithm = LM_MAP;
	engineConfiguration->globalTriggerAngleOffset = 45;
	engineConfiguration->bc.sensorChartMode = SC_MAP;
	engineConfiguration->specs.displacement = 0.072; // 72cc
	engineConfiguration->specs.cylindersCount = 1;
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);

	engineConfiguration->specs.firingOrder = FO_ONE_CYLINDER;

	/**
	 * We treat the trigger as 8-1 toothed wheel
	 */
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 8;
	engineConfiguration->trigger.customSkippedToothCount = 1;

	boardConfiguration->injectionPins[0] = GPIOC_9;
	boardConfiguration->ignitionPins[0] = GPIOC_8;
}
