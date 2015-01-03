/**
 * @file	joystick.h
 *
 * @date Jan 2, 2015
 * @author Andrey Belomutskiy, (c) 2012-2014
 */
#ifndef CONTROLLERS_JOYSTICK_H_
#define CONTROLLERS_JOYSTICK_H_

#include "main.h"

typedef enum {
	CENTER = 0,
	BUTTON_A = 1,
	BUTTON_B = 2,
	BUTTON_C = 3,
	BUTTON_D = 4,
} joystick_button_e;

void onJoystick(joystick_button_e button);
void initJoystick(Logging *shared);

#endif /* CONTROLLERS_JOYSTICK_H_ */
