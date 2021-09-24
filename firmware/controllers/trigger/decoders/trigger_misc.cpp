/*
 * trigger_misc.cpp
 *
 *  Created on: Oct 30, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "trigger_misc.h"
#include "trigger_universal.h"

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

// TT_TRI_TACH
void configureTriTach(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR);

	s->isSynchronizationNeeded = false;

	float toothWidth = 0.5;

	float engineCycle = FOUR_STROKE_ENGINE_CYCLE;

	int totalTeethCount = 135;
	float offset = 0;

	float angleDown = engineCycle / totalTeethCount * (0 + (1 - toothWidth));
	float angleUp = engineCycle / totalTeethCount * (0 + 1);
	s->addEventClamped(offset + angleDown, T_PRIMARY, TV_RISE, NO_LEFT_FILTER, NO_RIGHT_FILTER);
	s->addEventClamped(offset + angleDown + 0.1, T_SECONDARY, TV_RISE, NO_LEFT_FILTER, NO_RIGHT_FILTER);
	s->addEventClamped(offset + angleUp, T_PRIMARY, TV_FALL, NO_LEFT_FILTER, NO_RIGHT_FILTER);
	s->addEventClamped(offset + angleUp + 0.1, T_SECONDARY, TV_FALL, NO_LEFT_FILTER, NO_RIGHT_FILTER);


	addSkippedToothTriggerEvents(T_SECONDARY, s, totalTeethCount, /* skipped */ 0, toothWidth, offset, engineCycle,
			1.0 * FOUR_STROKE_ENGINE_CYCLE / 135,
			NO_RIGHT_FILTER);
}

/**
 * based on https://fordsix.com/threads/understanding-standard-and-signature-pip-thick-film-ignition.81515/
 * based on https://www.w8ji.com/distributor_stabbing.htm
 */
void configureFordPip(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

	s->tdcPosition = 662.5;

	s->setTriggerSynchronizationGap(0.66);
	s->setSecondTriggerSynchronizationGap(1.25);
	/**
	 * sensor is mounted on distributor but trigger shape is defined in engine cycle angles
	 */
	int oneCylinder = s->getCycleDuration() / 8;

	s->addEventAngle(oneCylinder * 0.75, T_PRIMARY, TV_RISE);
	s->addEventAngle(oneCylinder, T_PRIMARY, TV_FALL);


	for (int i = 2;i<=8;i++) {
		s->addEventAngle(oneCylinder * (i - 0.5), T_PRIMARY, TV_RISE);
		s->addEventAngle(oneCylinder * i, T_PRIMARY, TV_FALL);
	}

}

void configureFordST170(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);
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

void configureBarra3plus1cam(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

	s->useRiseEdge = true;
	// TODO: gapBothDirections?

	// This wheel has four teeth
	// two short gaps, and two long gaps
	// short = 60 deg
	// long = 120 deg

	{
		int offset = 120;

		s->addEventAngle(offset + 2 * 0 - 10, T_PRIMARY, TV_RISE);
		s->addEventAngle(offset + 2 * 0 +  0, T_PRIMARY, TV_FALL);

		// short gap 60 deg

		s->addEventAngle(offset + 2 * 60 - 10, T_PRIMARY, TV_RISE);
		s->addEventAngle(offset + 2 * 60 +  0, T_PRIMARY, TV_FALL);

		// long gap 120 deg

		s->addEventAngle(offset + 2 * 180 - 10, T_PRIMARY, TV_RISE);
		s->addEventAngle(offset + 2 * 180 +  0, T_PRIMARY, TV_FALL);

		// long gap 120 deg

		s->addEventAngle(offset + 2 * 300 - 10, T_PRIMARY, TV_RISE);
		s->addEventAngle(offset + 2 * 300 +  0, T_PRIMARY, TV_FALL);

		// short gap, 60 deg back to zero/720
	}

	// sync tooth is the zero tooth, the first short gap after two long gaps
	s->setTriggerSynchronizationGap3(0, 1.6f, 2.4f);
	// previous gap should be 1.0
	s->setTriggerSynchronizationGap3(1, 0.8f, 1.2f);
}
