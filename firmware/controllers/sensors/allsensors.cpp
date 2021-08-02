/**
 * @file    allsensors.cpp
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

ButtonDebounce acDebounce("ac_switch");

void initSensors(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	initMapDecoder(PASS_ENGINE_PARAMETER_SIGNATURE);
	acDebounce.init(15, CONFIG(acSwitch), CONFIG(acSwitchMode));
}

bool getAcToggle(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return acDebounce.readPinState();
}

bool hasAcToggle(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return (isBrainPinValid(CONFIG(acSwitch)));
}
