/**
 * @file	trigger_gm.cpp
 *
 * @date Mar 28, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "trigger_gm.h"

void configureGmTriggerShape(trigger_shape_s *s) {
	triggerShapeInit(s);

	// all angles are x2 here - so, 5 degree width is 10
	float w = 10;

	s->shaftPositionEventCount = 14;

	triggerAddEvent(s, 120 - w, T_PRIMARY, TV_HIGH);
	triggerAddEvent(s, 120, T_PRIMARY, TV_LOW);

	triggerAddEvent(s, 240 - w, T_PRIMARY, TV_HIGH);
	triggerAddEvent(s, 240, T_PRIMARY, TV_LOW);

	triggerAddEvent(s, 360 - w, T_PRIMARY, TV_HIGH);
	triggerAddEvent(s, 360, T_PRIMARY, TV_LOW);

	triggerAddEvent(s, 480 - w, T_PRIMARY, TV_HIGH);
	triggerAddEvent(s, 480, T_PRIMARY, TV_LOW);

	triggerAddEvent(s, 600 - w, T_PRIMARY, TV_HIGH);
	triggerAddEvent(s, 600, T_PRIMARY, TV_LOW);

	triggerAddEvent(s, 700 - w, T_PRIMARY, TV_HIGH);
	triggerAddEvent(s, 700, T_PRIMARY, TV_LOW);

	triggerAddEvent(s, 720 - w, T_PRIMARY, TV_HIGH);
	triggerAddEvent(s, 720, T_PRIMARY, TV_LOW);

}

