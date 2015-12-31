/**
 * @file	trigger_gm.cpp
 *
 * @date Mar 28, 2014
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "trigger_gm.h"

void configureGmTriggerShape(TriggerShape *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, false);

	// all angles are x2 here - so, 5 degree width is 10
	float w = 10;

	s->addEvent(120 - w, T_PRIMARY, TV_RISE);
	s->addEvent(120.0, T_PRIMARY, TV_FALL);

	s->addEvent(240 - w, T_PRIMARY, TV_RISE);
	s->addEvent(240.0, T_PRIMARY, TV_FALL);

	s->addEvent(360 - w, T_PRIMARY, TV_RISE);
	s->addEvent(360.0, T_PRIMARY, TV_FALL);

	s->addEvent(480 - w, T_PRIMARY, TV_RISE);
	s->addEvent(480.0, T_PRIMARY, TV_FALL);

	s->addEvent(600 - w, T_PRIMARY, TV_RISE);
	s->addEvent(600.0, T_PRIMARY, TV_FALL);

	s->addEvent(700 - w, T_PRIMARY, TV_RISE);
	s->addEvent(700.0, T_PRIMARY, TV_FALL);

	s->addEvent(720 - w, T_PRIMARY, TV_RISE);
	s->addEvent(720.0, T_PRIMARY, TV_FALL);
}

