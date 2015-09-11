/**
 * @file	trigger_subaru.cpp
 *
 * @date Sep 10, 2015
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "trigger_subaru.h"

void initialize36_2_2_2(TriggerShape *s) {
	s->reset(FOUR_STROKE_CAM_SENSOR, true);

	float wide = 30 * 2;
	float narrow = 10 * 2;

	s->isSynchronizationNeeded = false;

	float base = 0;

	for (int i = 0; i < 12; i++) {
		s->addEvent(base + narrow / 2, T_PRIMARY, TV_HIGH);
		s->addEvent(base + narrow, T_PRIMARY, TV_LOW);
		base += narrow;
	}

	s->addEvent(base + wide / 2, T_PRIMARY, TV_HIGH);
	s->addEvent(base + wide, T_PRIMARY, TV_LOW);
	base += wide;

	for (int i = 0; i < 15; i++) {
		s->addEvent(base + narrow / 2, T_PRIMARY, TV_HIGH);
		s->addEvent(base + narrow, T_PRIMARY, TV_LOW);
		base += narrow;
	}

	s->addEvent(720 - wide - wide / 2, T_PRIMARY, TV_HIGH);
	s->addEvent(720 - wide, T_PRIMARY, TV_LOW);

	s->addEvent(720 - wide / 2, T_PRIMARY, TV_HIGH);
	s->addEvent(720, T_PRIMARY, TV_LOW);
}
