/**
 * @file	mazda_626.cpp
 * MAZDA_626
 * set_engine_type 28
 *
 * @date Jan 16, 2015
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "mazda_626.h"
#include "engine_math.h"
#include "honda_accord.h"
#include "custom_engine.h"

void setMazda626EngineConfiguration(engine_configuration_s *engineConfiguration) {
	setCustomEngineConfiguration(engineConfiguration);

	board_configuration_s *boardConfiguration = &engineConfiguration->bc;
	engineConfiguration->trigger.type = TT_MAZDA_DOHC_1_4;

	boardConfiguration->analogChartMode = AC_TRIGGER;
	engineConfiguration->analogChartFrequency = 2;
}
