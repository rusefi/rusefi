/**
 * @file    allsensors.cpp
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine.h"
#include "allsensors.h"

EXTERN_ENGINE;

void initSensors(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	initMapDecoder(sharedLogger PASS_ENGINE_PARAMETER_SUFFIX);
	acDebounce.init(15, &CONFIG(acSwitch), &CONFIG(acSwitchMode));
}

ButtonDebounce acDebounce;

void getAcToggle(DECLARE_ENGINE_PARAMETER_SUFFIX) {
	return acDebounce.readPinState();
}

void hasAcToggle(DECLARE_ENGINE_PARAMETER_SUFFIX) {
	return (CONFIG(acSwitchPin) != GPIO_UNASSIGNED);
}
