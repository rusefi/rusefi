/**
 * @file	trigger_bmw.cpp
 *
 * @date May 11, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "trigger_bmw.h"

static inline float addPair(trigger_shape_s *s, float a, float w) {
	s->addEvent(a, T_SECONDARY, TV_HIGH);
	a += w;
	s->addEvent(a, T_SECONDARY, TV_LOW);
	a += w;
	return a;
}

void configureMiniCooperTriggerShape(trigger_shape_s *s) {

	s->reset(FOUR_STROKE_CAM_SENSOR);

//	s->initialState[0] = 1;

	float w = 360.0 / 121;
	float a = w / 2;
	s->addEvent(a, T_SECONDARY, TV_LOW);
	a += w;

	for (int i = 0; i <= 22; i++) {
		a = addPair(s, a, w);
	}

	a += 3 * w;

	float firstGapAngle = a;

	s->addEvent(a, T_SECONDARY, TV_HIGH);
	a += 3 * w;
	s->addEvent(a, T_SECONDARY, TV_LOW);
	a += w;

	for (int i = 0; i < 36; i++) {
		a = addPair(s, a, w);
	}

	s->addEvent(376, T_PRIMARY, TV_HIGH);

	for (int i = 0; i < 21; i++) {
		a = addPair(s, a, w);
	}
	a += 3 * w;

	efiAssertVoid(absF(firstGapAngle + 360 - a) < 0.1, "shape constraint");

	s->addEvent(a, T_SECONDARY, TV_HIGH);
	a += 3 * w;
	s->addEvent(a, T_SECONDARY, TV_LOW);
	a += w;

	for (int i = 0; i < 33; i++) {
		a = addPair(s, a, w);
	}

	efiAssertVoid(absF(720 - w / 2 - a) < 0.1, "shape constraint");
	s->addEvent(a, T_SECONDARY, TV_HIGH);

	s->addEvent(720.0, T_PRIMARY, TV_LOW);

	/**
	 * With just one tooth on camshaft synchronization is not needed
	 */
	s->isSynchronizationNeeded = false;
}
