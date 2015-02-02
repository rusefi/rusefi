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

void setMazda626EngineConfiguration(engine_configuration_s *engineConfiguration) {
	board_configuration_s *boardConfiguration = &engineConfiguration->bc;
	engineConfiguration->trigger.type = TT_FORD_ESCORT_GT_T;

	setFrankenso_01_LCD(boardConfiguration);
	setFrankenso0_1_joystick(engineConfiguration);

	setSingleCoilDwell(engineConfiguration);
	engineConfiguration->ignitionMode = IM_ONE_COIL;
}
