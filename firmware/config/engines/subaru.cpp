/**
 * @file	subaru.cpp
 *
 * SUBARU_2003_WRX
 * set_engine_type 22
 *
 * @date Sep 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "subaru.h"
#include "honda_accord.h"

void setSubaru2003Wrx(engine_configuration_s *engineConfiguration) {

	board_configuration_s * boardConfiguration = &engineConfiguration->bc;

	setFrankenso_01_LCD(boardConfiguration);
	setFrankenso0_1_joystick(engineConfiguration);

}
