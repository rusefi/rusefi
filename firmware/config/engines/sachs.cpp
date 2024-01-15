/**
 * @file	sachs.cpp
 *
 * set engine_type 29
 * http://rusefi.com/forum/viewtopic.php?f=3&t=396
 *
 * @date Jan 26, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "sachs.h"

// used by HW CI
void setSachs() {
	engineConfiguration->displacement = 0.1; // 100cc
	engineConfiguration->cylindersCount = 1;

	setTwoStrokeOperationMode();
	engineConfiguration->firingOrder = FO_1;
	engineConfiguration->engineChartSize = 400;

	/**
	 * 50/2 trigger
	 */
	engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 50;
	engineConfiguration->trigger.customSkippedToothCount = 2;
}
