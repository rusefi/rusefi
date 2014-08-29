/**
 * @file	trigger_mitsubishi.cpp
 *
 * @date Aug 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "trigger_mitsubishi.h"

void initializeMitsubishi4g18(trigger_shape_s *s) {
	s->reset(FOUR_STROKE_CAM_SENSOR);
	s->useRiseEdge = false;

	setTriggerSynchronizationGap(s, 1.6666);

	int secondaryWidth = 70;

	s->addEvent(180.0 - 131 + 5, T_PRIMARY, TV_HIGH);
	s->addEvent(180.0 - secondaryWidth, T_SECONDARY, TV_HIGH);
	s->addEvent(180.0, T_SECONDARY, TV_LOW);
	s->addEvent(180.0 + 49 + 5, T_PRIMARY, TV_LOW);

	s->addEvent(360.0 - secondaryWidth, T_SECONDARY, TV_HIGH);
	s->addEvent(360.0, T_SECONDARY, TV_LOW);

	s->addEvent(540.0 - 131 + 5, T_PRIMARY, TV_HIGH);
	s->addEvent(540.0 - secondaryWidth, T_SECONDARY, TV_HIGH);
	s->addEvent(540.0 - 131 + 5 + 90, T_PRIMARY, TV_LOW);
	s->addEvent(540.0, T_SECONDARY, TV_LOW);

	s->addEvent(720.0 - secondaryWidth, T_SECONDARY, TV_HIGH);
	s->addEvent(720.0, T_SECONDARY, TV_LOW);

	s->shaftPositionEventCount = s->getSize();
}
