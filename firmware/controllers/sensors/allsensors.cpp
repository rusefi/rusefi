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

void initSensors() {
	initMapDecoder();
	acDebounce.init(15, engineConfiguration->acSwitch, engineConfiguration->acSwitchMode);
}

bool getAcToggle() {
	return acDebounce.readPinState();
}

bool hasAcToggle() {
	return (isBrainPinValid(engineConfiguration->acSwitch));
}
