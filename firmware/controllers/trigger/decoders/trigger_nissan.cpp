/**
 * @file	trigger_nissan.cpp
 *
 * @date Sep 19, 2015
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "trigger_nissan.h"
#include "trigger_universal.h"

static void initializeNissanSR20VE_4_optional_360(TriggerShape *s, bool with2nd DECLARE_ENGINE_PARAMETER_S) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, with2nd);
	s->isSynchronizationNeeded = true;
	s->gapBothDirections = true;
	s->useOnlyPrimaryForSync = true;

	s->tdcPosition = 630;

	float w = 5;
	s->setTriggerSynchronizationGap2(9.67 * 0.75, 16);

	float left = 0;
	float right;

	int total = 360; // 360 on cam, over 720 crank degree

	float base = 180;
	right = base - 15;

	if (with2nd) {
		addSkippedToothTriggerEvents(T_PRIMARY, s, total, 0, 0.5, 0, 720,
				left, right PASS_ENGINE_PARAMETER);
	}

	s->addEvent2(right, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER);

	right = base;

	s->addEvent2(right, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER);

	// was is the the one with 360 opto sensor?

	base += 180;
	s->addEvent2(base - w, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(base, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER);

	base += 180;
	s->addEvent2(base - w, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(base, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER);

	base += 180;
	s->addEvent2(base - w, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(base, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER);

}


/**
 * Nissan Primera p11 year 1995-2002
 */

void initializeNissanSR20VE_4(TriggerShape *s DECLARE_ENGINE_PARAMETER_S) {
	initializeNissanSR20VE_4_optional_360(s, false PASS_ENGINE_PARAMETER);
}

void initializeNissanSR20VE_4_360(TriggerShape *s DECLARE_ENGINE_PARAMETER_S) {
	initializeNissanSR20VE_4_optional_360(s, true PASS_ENGINE_PARAMETER);
}
