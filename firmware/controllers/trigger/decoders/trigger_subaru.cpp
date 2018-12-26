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
void initialize36_2_2_2(TriggerShape *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

	float wide = 30 * 2;
	float narrow = 10 * 2;

	s->isSynchronizationNeeded = true;
	s->setTriggerSynchronizationGap(0.5);
	s->setSecondTriggerSynchronizationGap(1);

	float base = 0;

	for (int i = 0; i < 12; i++) {
		s->addEvent720(base + narrow / 2, T_PRIMARY, TV_RISE);
		s->addEvent720(base + narrow, T_PRIMARY, TV_FALL);
		base += narrow;
	}

	s->addEvent720(base + wide / 2, T_PRIMARY, TV_RISE);
	s->addEvent720(base + wide, T_PRIMARY, TV_FALL);
	base += wide;

	for (int i = 0; i < 15; i++) {
		s->addEvent720(base + narrow / 2, T_PRIMARY, TV_RISE);
		s->addEvent720(base + narrow, T_PRIMARY, TV_FALL);
		base += narrow;
	}

	s->addEvent720(720 - wide - wide / 2, T_PRIMARY, TV_RISE);
	s->addEvent720(720 - wide, T_PRIMARY, TV_FALL);

	s->addEvent720(720 - wide / 2, T_PRIMARY, TV_RISE);
	s->addEvent720(720, T_PRIMARY, TV_FALL);
	s->useOnlyPrimaryForSync = true;
}

void initializeSubaru7_6(TriggerShape *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

	float magic = 333;

	float width = 5;

	s->addEvent720(25 - width, T_PRIMARY, TV_RISE);
	s->addEvent720(25, T_PRIMARY, TV_FALL);

	s->addEvent720(magic - 180 - 87 - width, T_SECONDARY, TV_RISE);
	s->addEvent720(magic - 180 - 87, T_SECONDARY, TV_FALL);


	s->addEvent720(magic - 180 - 55 - width, T_SECONDARY, TV_RISE);
	s->addEvent720(magic - 180 - 55, T_SECONDARY, TV_FALL);

	s->addEvent720(magic - 180 - width, T_SECONDARY, TV_RISE);
	s->addEvent720(magic - 180, T_SECONDARY, TV_FALL);

	s->addEvent720(182 - width, T_PRIMARY, TV_RISE);
	s->addEvent720(182, T_PRIMARY, TV_FALL);

	s->addEvent720(magic - 87 - width, T_SECONDARY, TV_RISE);
	s->addEvent720(magic - 87, T_SECONDARY, TV_FALL);
	s->addEvent720(magic - 55 - width, T_SECONDARY, TV_RISE);
	s->addEvent720(magic - 55, T_SECONDARY, TV_FALL);
	s->addEvent720(magic - width, T_SECONDARY, TV_RISE);
	s->addEvent720(magic, T_SECONDARY, TV_FALL);


	s->addEvent720(343 - width, T_PRIMARY, TV_RISE);
	s->addEvent720(343, T_PRIMARY, TV_FALL);

	s->addEvent720(366 - width, T_PRIMARY, TV_RISE);
	s->addEvent720(366, T_PRIMARY, TV_FALL);

	s->addEvent720(384 - width, T_PRIMARY, TV_RISE);
	s->addEvent720(384, T_PRIMARY, TV_FALL);

	s->addEvent720(magic + 180 - 87 - width, T_SECONDARY, TV_RISE);
	s->addEvent720(magic + 180 - 87, T_SECONDARY, TV_FALL);
	s->addEvent720(magic + 180 - 55 - width, T_SECONDARY, TV_RISE);
	s->addEvent720(magic + 180 - 55, T_SECONDARY, TV_FALL);
	s->addEvent720(magic + 180 - width, T_SECONDARY, TV_RISE);
	s->addEvent720(magic + 180, T_SECONDARY, TV_FALL);


	s->addEvent720(538 - width, T_PRIMARY, TV_RISE);
	s->addEvent720(538, T_PRIMARY, TV_FALL);

	s->addEvent720(magic + 360 - 87 - width, T_SECONDARY, TV_RISE);
	s->addEvent720(magic + 360 - 87, T_SECONDARY, TV_FALL);
	s->addEvent720(magic + 360 - 55 - width, T_SECONDARY, TV_RISE);
	s->addEvent720(magic + 360 - 55, T_SECONDARY, TV_FALL);
	s->addEvent720(magic + 360 - width, T_SECONDARY, TV_RISE);
	s->addEvent720(magic + 360, T_SECONDARY, TV_FALL);

	s->addEvent720(720 - width, T_PRIMARY, TV_RISE);
	s->addEvent720(720, T_PRIMARY, TV_FALL);

	s->setTriggerSynchronizationGap2(4.9, 9);
	s->setTriggerSynchronizationGap3(1, 0.6, 1.25);

	s->isSynchronizationNeeded = true;

	s->useOnlyPrimaryForSync = true;

}
