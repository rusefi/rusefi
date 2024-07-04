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


void addToothRiseFall2(TriggerWaveform *s, angle_t angle) {
    s->addEvent360(angle - 10, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
    s->addEvent360(angle, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
}

void configureFordCoyote2(TriggerWaveform *s) {
    s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

    s->setTriggerSynchronizationGap3(0, 0.47, 0.7);
    s->setTriggerSynchronizationGap3(1, 0.95, 1.59);
    s->setTriggerSynchronizationGap3(2, 0.95, 1.59);

	addToothRiseFall2(s, 45);
	addToothRiseFall2(s, 90);

	addToothRiseFall2(s, 180 - 30);
	addToothRiseFall2(s, 180);

	addToothRiseFall2(s, 270 - 30);
	addToothRiseFall2(s, 270);

	addToothRiseFall2(s, 360);
}
