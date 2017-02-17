/**
 * @file	trigger_mitsubishi.cpp
 *
 * @date Aug 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "trigger_mitsubishi.h"

void configureFordAspireTriggerShape(TriggerShape * s DECLARE_ENGINE_PARAMETER_S) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);
	s->isSynchronizationNeeded = false;

	float x = 121.90;
	float y = 110.86;

	s->addEvent2(x + y - 180, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(x, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER);
	s->addEvent2(x + y, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(x + 180, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER);
	s->addEvent2(360, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER);

	s->addEvent2(x + 180 + y, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(x + 360, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER);
	s->addEvent2(x + 360 + y, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(x + 540, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER);
	s->addEvent2(720, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER);
	s->useOnlyPrimaryForSync = true;
}

void initializeMitsubishi4g18(TriggerShape *s DECLARE_ENGINE_PARAMETER_S) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);
	s->useRiseEdge = false;

	s->setTriggerSynchronizationGap(1.6666);

	int secondaryWidth = 70;

	s->addEvent2(180.0 - 131 + 5, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(180.0 - secondaryWidth, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(180.0, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER);
	s->addEvent2(180.0 + 49 + 5, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER);

	s->addEvent2(360.0 - secondaryWidth, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(360.0, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER);

	s->addEvent2(540.0 - 131 + 5, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(540.0 - secondaryWidth, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(540.0 - 131 + 5 + 90, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER);
	s->addEvent2(540.0, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER);

	s->addEvent2(720.0 - secondaryWidth, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(720.0, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER);
	s->useOnlyPrimaryForSync = true;
}
