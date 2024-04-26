/*
 * @file	trigger_honda.cpp
 *
 * @date May 27, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "trigger_honda.h"
#include "trigger_universal.h"

void configureHondaCbr600(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

  s->tdcPosition = 0; // todo: hard-code TDC position once we know it
  s->setTriggerSynchronizationGap2(/*from*/3.9, /*to*/8);

	s->addEvent720(350.0f, TriggerValue::FALL);
	s->addEvent720(360.0f, TriggerValue::RISE);

	s->addEvent720(650.0f, TriggerValue::FALL);
	s->addEvent720(660.0f, TriggerValue::RISE);

	s->addEvent720(710.0f, TriggerValue::FALL);
	s->addEvent720(720.0f, TriggerValue::RISE);
}

// todo: what is this 1+16 trigger about? should it have been defined as skipped + cam or else?
void configureOnePlus16(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

	int count = 16;
	float tooth = s->getCycleDuration() / 2 / count;
	float width = tooth / 2; // for VR we only handle rises so width does not matter much

	s->addEventAngle(1, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEventAngle(5, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	for (int i = 1; i <= count; i++) {
		s->addToothRiseFall(tooth * i, width, TriggerWheel::T_SECONDARY);
	}

	s->isSynchronizationNeeded = false;
}

// TT_HONDA_K_CRANK_12_1
void configureHondaK_12_1(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);

	// nominal gap 0.33
	s->setSecondTriggerSynchronizationGap2(0.2f, 0.7f);
	// nominal gap 2.0
	s->setTriggerSynchronizationGap2(1.1f, 2.4f);

	int count = 12;
	float tooth = 360 / count; // hint: tooth = 30

	// for VR we only handle rises so width does not matter much
    int width = 4;

	// Extra "+1" tooth happens 1/3 of the way between first two teeth
	s->addToothRiseFall(tooth / 3, width);

	for (int i = 1; i <= count; i++) {
		s->addToothRiseFall(tooth * i, width);
	}
}

/**
 * Exhaust cam shaft, not variable on Honda K
 * 2003 Honda Element
 */
void configureHondaK_4_1(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

	s->setTriggerSynchronizationGap3(/*gapIndex*/0, 1.5, 4.5);	// nominal 2.27
	s->setTriggerSynchronizationGap3(/*gapIndex*/1, 0.1, 0.5);	// nominal 0.28

	angle_t end = 70.5;
	int w = 15;
	s->addToothRiseFall(end + 90 * 0, w);
	s->addToothRiseFall(end + 90 * 1, w);
	s->addToothRiseFall(end + 90 * 2, w);
	s->addToothRiseFall(end + 90 * 3, w);

	s->addToothRiseFall(360, 7);
}
