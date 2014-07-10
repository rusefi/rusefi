/**
 * @file	trigger_gm.cpp
 *
 * @date Mar 28, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "trigger_gm.h"

void configureGmTriggerShape(trigger_config_s *triggerConfig, trigger_shape_s *s) {
	s->reset(FOUR_STROKE_CAM_SENSOR);

	// all angles are x2 here - so, 5 degree width is 10
	float w = 10;

	s->shaftPositionEventCount = 14;

	s->addEvent(120 - w, T_PRIMARY, TV_HIGH);
	s->addEvent(120, T_PRIMARY, TV_LOW);

	s->addEvent(240 - w, T_PRIMARY, TV_HIGH);
	s->addEvent(240, T_PRIMARY, TV_LOW);

	s->addEvent(360 - w, T_PRIMARY, TV_HIGH);
	s->addEvent(360, T_PRIMARY, TV_LOW);

	s->addEvent(480 - w, T_PRIMARY, TV_HIGH);
	s->addEvent(480, T_PRIMARY, TV_LOW);

	s->addEvent(600 - w, T_PRIMARY, TV_HIGH);
	s->addEvent(600, T_PRIMARY, TV_LOW);

	s->addEvent(700 - w, T_PRIMARY, TV_HIGH);
	s->addEvent(700, T_PRIMARY, TV_LOW);

	s->addEvent(720 - w, T_PRIMARY, TV_HIGH);
	s->addEvent(720, T_PRIMARY, TV_LOW);

}

