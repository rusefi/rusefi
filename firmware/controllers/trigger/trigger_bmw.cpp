/**
 * @file	trigger_bmw.cpp
 *
 * @date May 11, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "trigger_bmw.h"

void configureMiniCooperTriggerShape(engine_configuration_s *engineConfiguration,
		engine_configuration2_s *engineConfiguration2) {
	trigger_shape_s *s = &engineConfiguration2->triggerShape;

	s->reset();

	s->initialState[0] = 1;

	float w = 2.96;
	float a = w / 2;
	for (int i = 0; i < 19; i++) {
		a += w;
		s->addEvent(a, T_SECONDARY, TV_HIGH);
		a += w;
		s->addEvent(a, T_SECONDARY, TV_LOW);
	}
	a += 3 * w;
	s->addEvent(a, T_SECONDARY, TV_HIGH);
	a += 2 * w;
	s->addEvent(a, T_SECONDARY, TV_LOW);



	s->addEvent(376.4444444, T_PRIMARY, TV_LOW);
	s->addEvent(720, T_PRIMARY, TV_HIGH);

	s->shaftPositionEventCount = s->getSize();
	/**
	 * With just one tooth on camshaft synchronization is not needed
	 */
	engineConfiguration->triggerConfig.isSynchronizationNeeded = FALSE;
}
