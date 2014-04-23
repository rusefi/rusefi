/**
 * @file	trigger_chrysler.cpp
 *
 * @date Mar 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#include "trigger_chrysler.h"

void configureNeonTriggerShape(trigger_shape_s *s) {
	triggerShapeInit(s);

	// voodoo magic - we always need 720 at the end
	int base = 720 - 560;

	s->initialState[0] = 1;

	triggerAddEvent(s, base - 720 + 600, T_SECONDARY, TV_HIGH);
	triggerAddEvent(s, base - 720 + 604, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, base - 720 + 616, T_SECONDARY, TV_HIGH);
	triggerAddEvent(s, base - 720 + 620, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, base - 720 + 643, T_SECONDARY, TV_HIGH);
	triggerAddEvent(s, base - 720 + 648, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, base - 720 + 671, T_SECONDARY, TV_HIGH);
	triggerAddEvent(s, base - 720 + 676, T_SECONDARY, TV_LOW);

	triggerAddEvent(s, base + 0, T_PRIMARY, TV_LOW);

	triggerAddEvent(s, base + 20, T_SECONDARY, TV_HIGH);
	triggerAddEvent(s, base + 60, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, base + 75, T_SECONDARY, TV_HIGH);
	triggerAddEvent(s, base + 79, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, base + 101, T_SECONDARY, TV_HIGH);
	triggerAddEvent(s, base + 106, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, base + 130, T_SECONDARY, TV_HIGH);
	triggerAddEvent(s, base + 135, T_SECONDARY, TV_LOW);

	triggerAddEvent(s, base + 200, T_PRIMARY, TV_HIGH); // width = 150

	triggerAddEvent(s, base + 236, T_SECONDARY, TV_HIGH);
	triggerAddEvent(s, base + 239, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, base + 250, T_SECONDARY, TV_HIGH);
	triggerAddEvent(s, base + 255, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, base + 277, T_SECONDARY, TV_HIGH);
	triggerAddEvent(s, base + 282, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, base + 305, T_SECONDARY, TV_HIGH);
	triggerAddEvent(s, base + 310, T_SECONDARY, TV_LOW);

	triggerAddEvent(s, base + 374, T_SECONDARY, TV_HIGH);

	triggerAddEvent(s, base + 395, T_PRIMARY, TV_LOW); // width =

	triggerAddEvent(s, base + 418, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, base + 436, T_SECONDARY, TV_HIGH);
	triggerAddEvent(s, base + 441, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, base + 463, T_SECONDARY, TV_HIGH);
	triggerAddEvent(s, base + 468, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, base + 492, T_SECONDARY, TV_HIGH);
	triggerAddEvent(s, base + 497, T_SECONDARY, TV_LOW);


	triggerAddEvent(s, base + 560, T_PRIMARY, TV_HIGH); // width =

	s->shaftPositionEventCount = 4 + 8 + 8 + 8 + 8;
}

