/**
 * @file	trigger_subaru.cpp
 *
 * @date Sep 10, 2015
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "trigger_subaru.h"

/**
 * This trigger is also used by Nissan and Mazda
 */
void initialize36_2_2_2(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

	float wide = 30 * 2;
	float narrow = 10 * 2;

	s->isSynchronizationNeeded = true;
	s->setTriggerSynchronizationGap(0.5);
	s->setSecondTriggerSynchronizationGap(1);

	float base = 0;

	for (int i = 0; i < 12; i++) {
		s->addEvent2(base + narrow / 2, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
		s->addEvent2(base + narrow, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
		base += narrow;
	}

	s->addEvent2(base + wide / 2, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(base + wide, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
	base += wide;

	for (int i = 0; i < 15; i++) {
		s->addEvent2(base + narrow / 2, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
		s->addEvent2(base + narrow, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
		base += narrow;
	}

	s->addEvent2(720 - wide - wide / 2, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(720 - wide, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent2(720 - wide / 2, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(720, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
	s->useOnlyPrimaryForSync = true;
}

void initializeSubaru7_6(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

	float magic = 333;

	float width = 5;

	s->addEvent2(25 - width, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(25, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent2(magic - 180 - 87 - width, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(magic - 180 - 87, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);


	s->addEvent2(magic - 180 - 55 - width, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(magic - 180 - 55, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent2(magic - 180 - width, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(magic - 180, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent2(182 - width, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(182, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent2(magic - 87 - width, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(magic - 87, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(magic - 55 - width, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(magic - 55, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(magic - width, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(magic, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);


	s->addEvent2(343 - width, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(343, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent2(366 - width, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(366, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent2(384 - width, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(384, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent2(magic + 180 - 87 - width, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(magic + 180 - 87, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(magic + 180 - 55 - width, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(magic + 180 - 55, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(magic + 180 - width, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(magic + 180, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);


	s->addEvent2(538 - width, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(538, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent2(magic + 360 - 87 - width, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(magic + 360 - 87, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(magic + 360 - 55 - width, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(magic + 360 - 55, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(magic + 360 - width, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(magic + 360, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent2(720 - width, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(720, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->setTriggerSynchronizationGap2(4.9, 9);
	s->setSecondTriggerSynchronizationGap2(0.6, 1.25);

	s->isSynchronizationNeeded = true;

	s->useOnlyPrimaryForSync = true;

}
