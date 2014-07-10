/**
 * @file	GY6_139QMB.cpp
 * @brief	139qmb default engine configuration
 *
 * @date Feb 13, 2014
 * @author rus084
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "GY6_139QMB.h"

void setGy6139qmbDefaultEngineConfiguration(engine_configuration_s *engineConfiguration) {
	engineConfiguration->rpmHardLimit = 9000;
	engineConfiguration->crankingSettings.crankingRpm = 800;
	engineConfiguration->analogInputDividerCoefficient = 1.52;
	engineConfiguration->algorithm = LM_MAP;
	engineConfiguration->globalTriggerAngleOffset = 15;
	engineConfiguration->analogChartMode = AC_MAP;
	engineConfiguration->cylindersCount = 1;
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);

	engineConfiguration->firingOrder = FO_ONE_CYLINDER;

	/**
	 * We treat the trigger as 1/0 toothed wheel
	 */
	engineConfiguration->triggerConfig.totalToothCount = 1;
	engineConfiguration->triggerConfig.skippedToothCount = 0;
	engineConfiguration->triggerConfig.isSynchronizationNeeded = false;
	engineConfiguration->needSecondTriggerInput = false;
}
