/**
 * @file    allsensors.cpp
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "allsensors.h"

void initSensors(Engine *engine) {
	initThermistors(engine);
}

// todo: move this somewhere else? maybe.
bool getAcToggle(Engine *engine) {
	engine_configuration_s *engineConfiguration = engine->engineConfiguration;
	/**
	 * todo: make this flexible
	 *
	 * for now we are looking for a pull-up. High level means input switch is floating (which is OFF position)
	 * low value means input is ground - which means ON.
	 */
	return getVoltageDivided(engine->engineConfiguration->acSwitchAdc) < 2.5;
}
