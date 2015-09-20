/**
 * @file	trigger_nissan.cpp
 *
 * @date Sep 19, 2015
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "trigger_nissan.h"

void initializeNissan(TriggerShape *s) {
	s->reset(FOUR_STROKE_CAM_SENSOR, false);
	s->isSynchronizationNeeded = false;

	float w = 5;

	float base = 180;

	base += 180;
	// todo
	base += 180;
	// todo
	base += 180;

	s->addEvent(base - w, T_PRIMARY, TV_HIGH);
	s->addEvent(base, T_PRIMARY, TV_LOW);

}
