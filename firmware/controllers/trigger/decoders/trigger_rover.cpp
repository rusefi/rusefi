/*
 * @file trigger_rover.cpp
 *
 * @date Dec 27, 2015
 * @author PhilTheGeek
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "trigger_rover.h"

/**
 * https://en.wikipedia.org/wiki/Rover_K-series_engine
 */
void initializeRoverK(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, false);

	float tooth = 20;

	s->setTriggerSynchronizationGap(2.0);
	s->setThirdTriggerSynchronizationGap(2);
	s->isSynchronizationNeeded = true;

	float base = 0;

	for (int i = 0; i < 2; i++) {
		s->addEvent2(base + tooth / 2, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
		s->addEvent2(base + tooth, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
		base += tooth;
	}
	base += tooth;
	for (int i = 0; i < 2; i++) {
		s->addEvent2(base + tooth / 2, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
		s->addEvent2(base + tooth, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
		base += tooth;
	}
	base += tooth;
	for (int i = 0; i < 14; i++) {
		s->addEvent2(base + tooth / 2, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
		s->addEvent2(base + tooth, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
		base += tooth;
	}
	base += tooth;
	for (int i = 0; i < 3; i++) {
		s->addEvent2(base + tooth / 2, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
		s->addEvent2(base + tooth, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
		base += tooth;
	}
	base += tooth;
	for (int i = 0; i < 11; i++) {
		s->addEvent2(base + tooth / 2, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
		s->addEvent2(base + tooth, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
		base += tooth;
	}

}
