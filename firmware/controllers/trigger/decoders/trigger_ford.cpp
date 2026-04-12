/**
 * @file	trigger_ford.cpp
 *
 * @author Andrey Belomutskiy, (c) rusEFI LLC 2012-2023
 */

#include "pch.h"

#include "trigger_ford.h"

/**
 * based on https://fordsix.com/threads/understanding-standard-and-signature-pip-thick-film-ignition.81515/
 * based on https://www.w8ji.com/distributor_stabbing.htm
 */
static void configureFordPip(TriggerWaveform * s, size_t count) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Rise);

	s->tdcPosition = 662.5;

	s->setTriggerSynchronizationGap(0.66);
	s->setSecondTriggerSynchronizationGap(1.25);
	/**
	 * sensor is mounted on distributor but trigger shape is defined in engine cycle angles
	 */
	int oneCylinder = s->getCycleDuration() / count;

	s->addEventAngle(oneCylinder * 0.75, TriggerValue::RISE);
	s->addEventAngle(oneCylinder, TriggerValue::FALL);


	for (int i = 2;i<=count;i++) {
		s->addEventAngle(oneCylinder * (i - 0.5), TriggerValue::RISE);
		s->addEventAngle(oneCylinder * i, TriggerValue::FALL);
	}

}

void configureFordPip6(TriggerWaveform * s) {
	configureFordPip(s, 6);
}

void configureFordPip8(TriggerWaveform * s) {
	configureFordPip(s, 8);
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

void configureFordCoyote(TriggerWaveform *s) {
    s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

    s->setTriggerSynchronizationGap(3);
    s->setSecondTriggerSynchronizationGap(0.5);

	s->addToothRiseFall(45 /*, width = 10*/);
	s->addToothRiseFall(90);

	s->addToothRiseFall(180 - 30);
	s->addToothRiseFall(180);

	s->addToothRiseFall(270 - 30);
	s->addToothRiseFall(270);

	s->addToothRiseFall(360);
}
