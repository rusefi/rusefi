/**
 * @file	trigger_ford.cpp
 *
 * @author Andrey Belomutskiy, (c) rusEFI LLC 2012-2023
 */

#include "pch.h"

#include "trigger_ford.h"

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

// todo: reuse s->addToothFallRise?
static void addToothFallRise(TriggerWaveform *s, angle_t angle) {
    s->addEvent360(angle - 10, TriggerValue::FALL);
    s->addEvent360(angle, TriggerValue::RISE);
}

void configureFordCoyote2(TriggerWaveform *s) {
    s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

    s->setTriggerSynchronizationGap3(0, 0.47, 0.7);
    s->setTriggerSynchronizationGap3(1, 0.95, 1.59);
    s->setTriggerSynchronizationGap3(2, 0.95, 1.59);

	addToothFallRise(s, 45);
	addToothFallRise(s, 90);

	addToothFallRise(s, 180 - 30);
	addToothFallRise(s, 180);

	addToothFallRise(s, 270 - 30);
	addToothFallRise(s, 270);

	addToothFallRise(s, 360);
}
