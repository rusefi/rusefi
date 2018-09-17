/**
 * @file	joystick.h
 *
 * @date Jan 2, 2015
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef CONTROLLERS_JOYSTICK_H_
#define CONTROLLERS_JOYSTICK_H_

#include "global.h"

typedef enum {
	JB_CENTER = 0,
	JB_BUTTON_A = 1,
	JB_BUTTON_B = 2,
	JB_BUTTON_C = 3,
	JB_BUTTON_D = 4,
} joystick_button_e;

void onJoystick(joystick_button_e button);
void initJoystick(Logging *shared);

#endif /* CONTROLLERS_JOYSTICK_H_ */
