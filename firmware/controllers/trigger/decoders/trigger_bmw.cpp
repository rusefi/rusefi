/**
 * @file	trigger_bmw.cpp
 *
 * @date May 11, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "trigger_bmw.h"

static inline float addPair(TriggerShape *s, float a, float w DECLARE_ENGINE_PARAMETER_S) {
	s->addEvent2(a, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER);
	a += w;
	s->addEvent2(a, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER);
	a += w;
	return a;
}

void configureMiniCooperTriggerShape(TriggerShape *s DECLARE_ENGINE_PARAMETER_S) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

//	s->initialState[0] = 1;

	float w = 360.0 / 121;
	float a = w / 2;
	s->addEvent2(a, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER);
	a += w;

	for (int i = 0; i <= 22; i++) {
		a = addPair(s, a, w PASS_ENGINE_PARAMETER);
	}

	a += 3 * w;

	float firstGapAngle = a;

	s->addEvent2(a, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER);
	a += 3 * w;
	s->addEvent2(a, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER);
	a += w;

	for (int i = 0; i < 36; i++) {
		a = addPair(s, a, w PASS_ENGINE_PARAMETER);
	}

	s->addEvent2(376, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER);

	for (int i = 0; i < 21; i++) {
		a = addPair(s, a, w PASS_ENGINE_PARAMETER);
	}
	a += 3 * w;

	efiAssertVoid(absF(firstGapAngle + 360 - a) < 0.1, "shape constraint");

	s->addEvent2(a, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER);
	a += 3 * w;
	s->addEvent2(a, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER);
	a += w;

	for (int i = 0; i < 33; i++) {
		a = addPair(s, a, w PASS_ENGINE_PARAMETER);
	}

	efiAssertVoid(absF(720 - w / 2 - a) < 0.1, "shape constraint");
	s->addEvent2(a, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER);

	s->addEvent2(720.0, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER);

	/**
	 * With just one tooth on camshaft synchronization is not needed
	 */
	s->isSynchronizationNeeded = false;
	s->useOnlyPrimaryForSync = true;
}
