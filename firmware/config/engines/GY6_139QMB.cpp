/**
 * @file	GY6_139QMB.cpp
 * @brief	139qmb default engine configuration
 *
 * 4-stroke one cylinder
 *
 * set engine_type 8
 *
 * @date Feb 13, 2014
 * @author rus084, (c) 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "GY6_139QMB.h"

void setGy6139qmbDefaultEngineConfiguration() {
	engineConfiguration->rpmHardLimit = 9000;
	engineConfiguration->cranking.rpm = 1100;
	setTargetRpmCurve(2000);

	engineConfiguration->globalTriggerAngleOffset = Gy6139_globalTriggerAngleOffset;
	engineConfiguration->displacement = 0.072; // 72cc
	engineConfiguration->cylindersCount = 1;
	setCrankOperationMode();

	engineConfiguration->firingOrder = FO_1;

	/**
	 * We treat the trigger as 8-1 toothed wheel
	 */
	engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 8;
	engineConfiguration->trigger.customSkippedToothCount = 1;

	// set injection_mode 1
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	// set ignition_mode 0
	engineConfiguration->ignitionMode = IM_ONE_COIL;
}
