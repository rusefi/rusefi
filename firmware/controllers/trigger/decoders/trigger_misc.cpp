/*
 * trigger_misc.cpp
 *
 *  Created on: Oct 30, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "trigger_misc.h"
#include "trigger_universal.h"

// trigger_type_e::TT_FIAT_IAW_P8
void configureFiatIAQ_P8(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Rise);

	int width = 60;
	s->tdcPosition = width;

	s->addEvent720(width, true);
	s->addEvent720(180, false);

	s->addEvent720(180 + width, true);
	s->addEvent720(720, false);
	s->setTriggerSynchronizationGap(3);
}

// trigger_type_e::TT_TRI_TACH
void configureTriTach(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);

	s->isSynchronizationNeeded = false;

	float toothWidth = 0.5;

	float engineCycle = FOUR_STROKE_ENGINE_CYCLE;

	int totalTeethCount = 135;
	float offset = 0;

	float angleDown = engineCycle / totalTeethCount * (0 + (1 - toothWidth));
	float angleUp = engineCycle / totalTeethCount * (0 + 1);
	s->addEventClamped(offset + angleDown, true, TriggerWheel::T_PRIMARY, NO_LEFT_FILTER, NO_RIGHT_FILTER);
	s->addEventClamped(offset + angleDown + 0.1, true, TriggerWheel::T_SECONDARY, NO_LEFT_FILTER, NO_RIGHT_FILTER);
	s->addEventClamped(offset + angleUp, false, TriggerWheel::T_PRIMARY, NO_LEFT_FILTER, NO_RIGHT_FILTER);
	s->addEventClamped(offset + angleUp + 0.1, false, TriggerWheel::T_SECONDARY, NO_LEFT_FILTER, NO_RIGHT_FILTER);


	addSkippedToothTriggerEvents(TriggerWheel::T_SECONDARY, s, totalTeethCount, /* skipped */ 0, toothWidth, offset, engineCycle,
			1.0 * FOUR_STROKE_ENGINE_CYCLE / 135,
			NO_RIGHT_FILTER);
}

/**
 * based on https://fordsix.com/threads/understanding-standard-and-signature-pip-thick-film-ignition.81515/
 * based on https://www.w8ji.com/distributor_stabbing.htm
 */
void configureFordPip(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Rise);

	s->tdcPosition = 662.5;

	s->setTriggerSynchronizationGap(0.66);
	s->setSecondTriggerSynchronizationGap(1.25);
	/**
	 * sensor is mounted on distributor but trigger shape is defined in engine cycle angles
	 */
	int oneCylinder = s->getCycleDuration() / 8;

	s->addEventAngle(oneCylinder * 0.75, true);
	s->addEventAngle(oneCylinder, false);


	for (int i = 2; i <= 8; i++) {
		s->addEventAngle(oneCylinder * (i - 0.5), true);
		s->addEventAngle(oneCylinder * i, false);
	}

}

void configureFordST170(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);
	int width = 10;

	int total = s->getCycleDuration() / 8;

	s->addEventAngle(1 * total - width, true);
	s->addEventAngle(1 * total, false);

	s->addEventAngle(2 * total - width, true);
	s->addEventAngle(2 * total, false);

	s->addEventAngle(4 * total - width, true);
	s->addEventAngle(4 * total, false);

	s->addEventAngle(6 * total - width, true);
	s->addEventAngle(6 * total, false);

	s->addEventAngle(8 * total - width, true);
	s->addEventAngle(8 * total, false);
}

void configureDaihatsu4(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

	int width = 10;

	s->setTriggerSynchronizationGap(0.125);

	s->addEventAngle(30 - width, true);
	s->addEventAngle(30, false);


	s->addEventAngle(s->getCycleDuration() / 3 - width, true);
	s->addEventAngle(s->getCycleDuration() / 3, false);

	s->addEventAngle(s->getCycleDuration() / 3 * 2 - width, true);
	s->addEventAngle(s->getCycleDuration() / 3 * 2, false);

	s->addEventAngle(s->getCycleDuration() - width, true);
	s->addEventAngle(s->getCycleDuration(), false);

}

void configureBarra3plus1cam(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

	// This wheel has four teeth
	// two short gaps, and two long gaps
	// short = 60 deg
	// long = 120 deg

	{
		int offset = 120;

		s->addEventAngle(offset + 2 * 0 - 10, true);
		s->addEventAngle(offset + 2 * 0 +  0, false);

		// short gap 60 deg

		s->addEventAngle(offset + 2 * 60 - 10, true);
		s->addEventAngle(offset + 2 * 60 +  0, false);

		// long gap 120 deg

		s->addEventAngle(offset + 2 * 180 - 10, true);
		s->addEventAngle(offset + 2 * 180 +  0, false);

		// long gap 120 deg

		s->addEventAngle(offset + 2 * 300 - 10, true);
		s->addEventAngle(offset + 2 * 300 +  0, false);

		// short gap, 60 deg back to zero/720
	}

	// sync tooth is the zero tooth, the first short gap after two long gaps
	s->setTriggerSynchronizationGap3(0, 1.6f, 2.4f);
	// previous gap should be 1.0
	s->setTriggerSynchronizationGap3(1, 0.8f, 1.2f);
}

void configureBenelli(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);

    constexpr float magic = 15;

	float angle = 3 * magic;

	s->addEvent360(angle / 2 /* 22.5 */, true);
	s->addEvent360(angle /*     45   */, false);

	for (int i = 0; i < 24 - 3; i++) {
		angle += magic / 2;
		s->addEvent360(angle, true);

		angle += magic / 2;
		s->addEvent360(angle, false);
	}
}
