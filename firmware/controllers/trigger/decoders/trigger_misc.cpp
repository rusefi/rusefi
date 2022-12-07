/*
 * trigger_misc.cpp
 *
 *  Created on: Oct 30, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "trigger_misc.h"
#include "trigger_universal.h"

// TT_FIAT_IAW_P8
void configureFiatIAQ_P8(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Rise);

	int width = 60;
	s->tdcPosition = width;

	s->addEvent720(width, TriggerValue::RISE);
	s->addEvent720(180, TriggerValue::FALL);

	s->addEvent720(180 + width, TriggerValue::RISE);
	s->addEvent720(720, TriggerValue::FALL);
	s->setTriggerSynchronizationGap(3);
}

// TT_TRI_TACH
void configureTriTach(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);

	s->isSynchronizationNeeded = false;

	float toothWidth = 0.5;

	float engineCycle = FOUR_STROKE_ENGINE_CYCLE;

	int totalTeethCount = 135;
	float offset = 0;

	float angleDown = engineCycle / totalTeethCount * (0 + (1 - toothWidth));
	float angleUp = engineCycle / totalTeethCount * (0 + 1);
	s->addEventClamped(offset + angleDown, TriggerValue::RISE, TriggerWheel::T_PRIMARY, NO_LEFT_FILTER, NO_RIGHT_FILTER);
	s->addEventClamped(offset + angleDown + 0.1, TriggerValue::RISE, TriggerWheel::T_SECONDARY, NO_LEFT_FILTER, NO_RIGHT_FILTER);
	s->addEventClamped(offset + angleUp, TriggerValue::FALL, TriggerWheel::T_PRIMARY, NO_LEFT_FILTER, NO_RIGHT_FILTER);
	s->addEventClamped(offset + angleUp + 0.1, TriggerValue::FALL, TriggerWheel::T_SECONDARY, NO_LEFT_FILTER, NO_RIGHT_FILTER);


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

	s->addEventAngle(oneCylinder * 0.75, TriggerValue::RISE);
	s->addEventAngle(oneCylinder, TriggerValue::FALL);


	for (int i = 2;i<=8;i++) {
		s->addEventAngle(oneCylinder * (i - 0.5), TriggerValue::RISE);
		s->addEventAngle(oneCylinder * i, TriggerValue::FALL);
	}

}

void configureFordST170(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);
	int width = 10;

	int total = s->getCycleDuration() / 8;

	s->addEventAngle(1 * total - width, TriggerValue::RISE);
	s->addEventAngle(1 * total, TriggerValue::FALL);

	s->addEventAngle(2 * total - width, TriggerValue::RISE);
	s->addEventAngle(2 * total, TriggerValue::FALL);

	s->addEventAngle(4 * total - width, TriggerValue::RISE);
	s->addEventAngle(4 * total, TriggerValue::FALL);

	s->addEventAngle(6 * total - width, TriggerValue::RISE);
	s->addEventAngle(6 * total, TriggerValue::FALL);

	s->addEventAngle(8 * total - width, TriggerValue::RISE);
	s->addEventAngle(8 * total, TriggerValue::FALL);
}

void configureDaihatsu4(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

	int width = 10;

	s->setTriggerSynchronizationGap(0.125);

	s->addEventAngle(30 - width, TriggerValue::RISE);
	s->addEventAngle(30, TriggerValue::FALL);


	s->addEventAngle(s->getCycleDuration() / 3 - width, TriggerValue::RISE);
	s->addEventAngle(s->getCycleDuration() / 3, TriggerValue::FALL);

	s->addEventAngle(s->getCycleDuration() / 3 * 2 - width, TriggerValue::RISE);
	s->addEventAngle(s->getCycleDuration() / 3 * 2, TriggerValue::FALL);

	s->addEventAngle(s->getCycleDuration() - width, TriggerValue::RISE);
	s->addEventAngle(s->getCycleDuration(), TriggerValue::FALL);

}

void configureBarra3plus1cam(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

	// This wheel has four teeth
	// two short gaps, and two long gaps
	// short = 60 deg
	// long = 120 deg

	{
		int offset = 120;

		s->addEventAngle(offset + 2 * 0 - 10, TriggerValue::RISE);
		s->addEventAngle(offset + 2 * 0 +  0, TriggerValue::FALL);

		// short gap 60 deg

		s->addEventAngle(offset + 2 * 60 - 10, TriggerValue::RISE);
		s->addEventAngle(offset + 2 * 60 +  0, TriggerValue::FALL);

		// long gap 120 deg

		s->addEventAngle(offset + 2 * 180 - 10, TriggerValue::RISE);
		s->addEventAngle(offset + 2 * 180 +  0, TriggerValue::FALL);

		// long gap 120 deg

		s->addEventAngle(offset + 2 * 300 - 10, TriggerValue::RISE);
		s->addEventAngle(offset + 2 * 300 +  0, TriggerValue::FALL);

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

	s->addEvent360(angle / 2 /* 22.5 */, TriggerValue::RISE);
	s->addEvent360(angle /*     45   */, TriggerValue::FALL);

	for (int i = 0;i< 24 - 3;i++) {
		angle += magic / 2;
		s->addEvent360(angle, TriggerValue::RISE);

		angle += magic / 2;
		s->addEvent360(angle, TriggerValue::FALL);
	}
}
