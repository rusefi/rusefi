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
