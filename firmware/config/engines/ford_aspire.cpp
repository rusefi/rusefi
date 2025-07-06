/**
 * @file    ford_aspire.cpp
 * @brief	1996 Ford Aspire default engine configuration
 *
 * FORD_ASPIRE_1996
 * todo: revisit "unit" tests and remove this configuration?
 *
 * @date Sep 9, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "ford_aspire.h"

#include "advance_map.h"

void setFordAspireEngineConfiguration() {
	engineConfiguration->firingOrder = FO_1_3_4_2;
	engineConfiguration->globalTriggerAngleOffset = 175;
	engineConfiguration->crankingTimingAngle = -37;

	setSingleCoilDwell();
	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engineConfiguration->trigger.type = trigger_type_e::TT_FORD_ASPIRE;
	engineConfiguration->engineSnifferRpmThreshold = 13000;
}
