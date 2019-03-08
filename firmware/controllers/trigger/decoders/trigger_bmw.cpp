/**
 * @file	trigger_bmw.cpp
 *
 * @date May 11, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "trigger_bmw.h"

static inline float addPair(TriggerShape *s, float a, float w) {
	s->addEvent720(a, T_SECONDARY, TV_RISE);
	a += w;
	s->addEvent720(a, T_SECONDARY, TV_FALL);
	a += w;
	return a;
}

void configureMiniCooperTriggerShape(TriggerShape *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

//	s->initialState[0] = 1;

	float w = 360.0 / 121;
	float a = w / 2;
	s->addEvent720(a, T_SECONDARY, TV_FALL);
	a += w;

	for (int i = 0; i <= 22; i++) {
		a = addPair(s, a, w);
	}

	a += 3 * w;

	float firstGapAngle = a;
	/* may be */UNUSED(firstGapAngle);

	s->addEvent720(a, T_SECONDARY, TV_RISE);
	a += 3 * w;
	s->addEvent720(a, T_SECONDARY, TV_FALL);
	a += w;

	for (int i = 0; i < 36; i++) {
		a = addPair(s, a, w);
	}

	s->addEvent720(376, T_PRIMARY, TV_RISE);

	for (int i = 0; i < 21; i++) {
		a = addPair(s, a, w);
	}
	a += 3 * w;

	efiAssertVoid(CUSTOM_ERR_6584, absF(firstGapAngle + 360 - a) < 0.1, "shape constraint");

	s->addEvent720(a, T_SECONDARY, TV_RISE);
	a += 3 * w;
	s->addEvent720(a, T_SECONDARY, TV_FALL);
	a += w;

	for (int i = 0; i < 33; i++) {
		a = addPair(s, a, w);
	}

	efiAssertVoid(CUSTOM_ERR_6585, absF(720 - w / 2 - a) < 0.1, "shape constraint");
	s->addEvent720(a, T_SECONDARY, TV_RISE);

	s->addEvent720(720.0, T_PRIMARY, TV_FALL);

	/**
	 * With just one tooth on camshaft synchronization is not needed
	 */
	s->isSynchronizationNeeded = false;
	s->useOnlyPrimaryForSync = true;
}
