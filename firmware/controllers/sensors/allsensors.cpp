/**
 * @file    allsensors.cpp
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "engine.h"
#include "allsensors.h"

void initSensors(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_S) {
	initThermistors(sharedLogger PASS_ENGINE_PARAMETER);
	initMapDecoder(PASS_ENGINE_PARAMETER_F);
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
