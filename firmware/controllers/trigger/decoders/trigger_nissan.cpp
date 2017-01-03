/**
 * @file	trigger_nissan.cpp
 *
 * @date Sep 19, 2015
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "trigger_nissan.h"

void initializeNissan(TriggerShape *s DECLARE_ENGINE_PARAMETER_S) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, false);
	s->isSynchronizationNeeded = false;

	float w = 5;

	float base = 180;

	// todo: what was this about? :) what Nissan? Need to dig the forum around Sep 19, 2015
	// was is the the one with 360 opto sensor?

	base += 180;
	// todo
	base += 180;
	// todo
	base += 180;

	s->addEvent2(base - w, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(base, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER);

}
