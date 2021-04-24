/**
 * @file	joystick.h
 *
 * @date Jan 2, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"

typedef enum {
	JB_CENTER = 0,
	JB_BUTTON_A = 1,
// not used so far	JB_BUTTON_B = 2,
// not used so far	JB_BUTTON_C = 3,
	JB_BUTTON_D = 4,
} joystick_button_e;

void onJoystick(joystick_button_e button);
void initJoystick();
void startJoystickPins();
void stopJoystickPins();
