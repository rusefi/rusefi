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

void initialize_one_of_24_2_2(TriggerWaveform *s, int firstCount, int secondCount) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);

	float narrow = 360 / 24;
	float wide = narrow * 2;

	float base = 0;

	for (int i = 0; i < firstCount; i++) {
		s->addToothFallRise(base + narrow, narrow / 2);
		base += narrow;
	}

	s->addToothFallRise(base + wide, wide / 2);
	base += wide;

	for (int i = 0; i < secondCount; i++) {
		s->addToothFallRise(base + narrow, narrow / 2);
		base += narrow;
	}

       s->addToothFallRise(360, narrow/2);
}

void configureHondaJ30A2_24_1_1(TriggerWaveform *s) {
	initialize_one_of_24_2_2(s, 6, 14);

    size_t count = 6;

    s->tdcPosition = 0;

 s->setTriggerSynchronizationGap3(/*gapIndex*/0, 1.6, 4);
  for (size_t i = 1 ; i < count ; i++) {
    s->setTriggerSynchronizationGap3(/*gapIndex*/i, 0.65, 1.4);
  }
  s->setTriggerSynchronizationGap3(/*gapIndex*/count, 0.2, 0.55);
  }