/**
 * @file    allsensors.cpp
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "engine.h"
#include "allsensors.h"

EXTERN_ENGINE;

void initSensors(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_S) {
	initThermistors(sharedLogger PASS_ENGINE_PARAMETER);
	initMapDecoder(sharedLogger PASS_ENGINE_PARAMETER);
}

// todo: move this somewhere else? maybe.
bool getAcToggle(DECLARE_ENGINE_PARAMETER_F) {
	/**
	 * todo: make this flexible
	 *
	 * for now we are looking for a pull-up. High level means input switch is floating (which is OFF position)
	 * low value means input is ground - which means ON.
	 */
	return getVoltageDivided("A/C", engine->engineConfiguration->acSwitchAdc) < 2.5;
}
