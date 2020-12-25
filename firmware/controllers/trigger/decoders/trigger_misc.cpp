/*
 * trigger_misc.cpp
 *
 *  Created on: Oct 30, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "trigger_misc.h"

// TT_FIAT_IAW_P8
void configureFiatIAQ_P8(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

	int width = 60;
	s->tdcPosition = width;

	s->addEvent720(width, T_PRIMARY, TV_RISE);
	s->addEvent720(180, T_PRIMARY, TV_FALL);

	s->addEvent720(180 + width, T_PRIMARY, TV_RISE);
	s->addEvent720(720, T_PRIMARY, TV_FALL);
	s->setTriggerSynchronizationGap(3);
}

void configureFordST170(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);
	s->isSynchronizationNeeded = true;
	int width = 10;

	int total = s->getCycleDuration() / 8;

	s->addEventAngle(1 * total - width, T_PRIMARY, TV_RISE);
	s->addEventAngle(1 * total, T_PRIMARY, TV_FALL);

	s->addEventAngle(2 * total - width, T_PRIMARY, TV_RISE);
	s->addEventAngle(2 * total, T_PRIMARY, TV_FALL);

	s->addEventAngle(4 * total - width, T_PRIMARY, TV_RISE);
	s->addEventAngle(4 * total, T_PRIMARY, TV_FALL);

	s->addEventAngle(6 * total - width, T_PRIMARY, TV_RISE);
	s->addEventAngle(6 * total, T_PRIMARY, TV_FALL);

	s->addEventAngle(8 * total - width, T_PRIMARY, TV_RISE);
	s->addEventAngle(8 * total, T_PRIMARY, TV_FALL);
}

void configureDaihatsu4(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

	int width = 10;

	s->setTriggerSynchronizationGap(0.125);

	s->addEventAngle(30 - width, T_PRIMARY, TV_RISE);
	s->addEventAngle(30, T_PRIMARY, TV_FALL);


	s->addEventAngle(s->getCycleDuration() / 3 - width, T_PRIMARY, TV_RISE);
	s->addEventAngle(s->getCycleDuration() / 3, T_PRIMARY, TV_FALL);

	s->addEventAngle(s->getCycleDuration() / 3 * 2 - width, T_PRIMARY, TV_RISE);
	s->addEventAngle(s->getCycleDuration() / 3 * 2, T_PRIMARY, TV_FALL);

	s->addEventAngle(s->getCycleDuration() - width, T_PRIMARY, TV_RISE);
	s->addEventAngle(s->getCycleDuration(), T_PRIMARY, TV_FALL);

}
