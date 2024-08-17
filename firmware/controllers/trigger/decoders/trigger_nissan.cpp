/**
 * @file	trigger_nissan.cpp
 *
 * https://rusefi.com/forum/viewtopic.php?f=3&t=1194&start=150#p27784
 *
 * @date Sep 19, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

includePath "pch.h"

#include "trigger_nissan.h"
#include "trigger_universal.h"
#include "trigger_subaru.h"

/**
 * 8,2,2,2 Nissan pattern
 */
void initializeNissanSR20VE_4(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Both);

	s->tdcPosition = 630;

	s->setTriggerSynchronizationGap2(9.67 * 0.75, 16);

	float width = 4;

	s->addEvent720(1 * 180 - 4 * width, TriggerValue::RISE);
	s->addEvent720(1 * 180, TriggerValue::FALL);

	s->addEvent720(2 * 180 - width, TriggerValue::RISE);
	s->addEvent720(2 * 180, TriggerValue::FALL);

	s->addEvent720(3 * 180 - width, TriggerValue::RISE);
	s->addEvent720(3 * 180, TriggerValue::FALL);

	s->addEvent720(4 * 180 - width, TriggerValue::RISE);
	s->addEvent720(4 * 180, TriggerValue::FALL);
}

void initializeNissanVQvvt(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

	int offset = 360 - 260;

	s->addToothRiseFall(offset + 20);
	s->addToothRiseFall(offset + 80);
	s->addToothRiseFall(offset + 100);
	s->addToothRiseFall(offset + 140);
	s->addToothRiseFall(offset + 160);
	s->addToothRiseFall(offset + 260);

	s->setTriggerSynchronizationGap2(4, 6);
	s->setSecondTriggerSynchronizationGap2(0.35f, 0.7f);
}

void makeNissanPattern(TriggerWaveform* s, size_t halfCylinderCount, size_t totalWheel, size_t missing) {

	auto toothAngle = 360.0f / totalWheel;

	auto patternTeeth = totalWheel / halfCylinderCount;
	auto toothCount = patternTeeth - missing;

	float currentAngle = missing * toothAngle;
	for (size_t i = 0; i < toothCount; i++) {
		currentAngle += toothAngle;
		s->addEventAngle(currentAngle - 5, TriggerValue::RISE);
		s->addEventAngle(currentAngle, TriggerValue::FALL);
	}
}

void initializeNissanVQ35crank(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_THREE_TIMES_CRANK_SENSOR, SyncEdge::RiseOnly);

	s->tdcPosition = 675;

	// 6 cylinder = 36 tooth wheel, missing 2 teeth in 3 spots
	makeNissanPattern(s, 3, 36, 2);
	s->setTriggerSynchronizationGap3(/*gapIndex*/0, 0.2, 0.5);
	s->setTriggerSynchronizationGap3(/*gapIndex*/1, 2, 4);
	s->setTriggerSynchronizationGap3(/*gapIndex*/2, 0.6, 1.4);
}

void initializeNissanMR18crank(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR, SyncEdge::RiseOnly);

	s->tdcPosition = 80;

	// 4 cylinder = 36 tooth wheel, missing 2 teeth in 2 spots
	makeNissanPattern(s, 2, 36, 2);
	s->setTriggerSynchronizationGap(0.33);
}

void initializeNissanQR25crank(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR, SyncEdge::RiseOnly);
	s->setTriggerSynchronizationGap(0.33);
	s->setSecondTriggerSynchronizationGap(3);

	s->tdcPosition = 585;

	float currentAngle = 20;
	for (int i = 0;i < 16;i++) {
		currentAngle += 10;
		s->addEventAngle(currentAngle - 5, TriggerValue::RISE);
		s->addEventAngle(currentAngle, TriggerValue::FALL);
	}
}

static void addvq30tooth(TriggerWaveform *s, float angle) {
	s->addEvent360(angle - 4, TriggerValue::RISE);
	s->addEvent360(angle, TriggerValue::FALL);
}

// yes, this is CAM shaft shape NOT crank shaft shape!
// we will add crank shape once Pavel makes progress
void initializeNissanVQ30cam(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

	s->tdcPosition = 120;

	int x = 360 + 52;

	addvq30tooth(s, x - (360 - 9 * 0));
	addvq30tooth(s, x - (360 - 9 * 1));
	addvq30tooth(s, x - (360 - 9 * 2));

	addvq30tooth(s, x - (252 + 9 * 5));
	addvq30tooth(s, x - (252 + 9 * 4));
	addvq30tooth(s, x - (252 + 9 * 3));
	addvq30tooth(s, x - (252 + 9 * 2));
	addvq30tooth(s, x - (252 + 9 * 1));
	addvq30tooth(s, x - (252 + 9 * 0));

	addvq30tooth(s, x - (236        ));

	addvq30tooth(s, x - (152 + 9 * 3));
	addvq30tooth(s, x - (152 + 9 * 2));
	addvq30tooth(s, x - (152 + 9 * 1));
	addvq30tooth(s, x - (152 + 9 * 0));

	addvq30tooth(s, x - (85 + 9 * 4));
	addvq30tooth(s, x - (85 + 9 * 3));
	addvq30tooth(s, x - (85 + 9 * 2));
	addvq30tooth(s, x - (85 + 9 * 1));
	addvq30tooth(s, x - (85 + 9 * 0));

	addvq30tooth(s, x - (52 + 9 * 1));
	addvq30tooth(s, x - (52 + 9 * 0));

	s->setTriggerSynchronizationGap3(/*gapIndex*/0, 5.78 * TRIGGER_GAP_DEVIATION_LOW, 5.78 * TRIGGER_GAP_DEVIATION_HIGH);
	s->setTriggerSynchronizationGap3(/*gapIndex*/1, 0.38 * TRIGGER_GAP_DEVIATION_LOW, 0.38 * TRIGGER_GAP_DEVIATION_HIGH);
	s->setTriggerSynchronizationGap3(/*gapIndex*/2, 2.67 * TRIGGER_GAP_DEVIATION_LOW, 2.67 * TRIGGER_GAP_DEVIATION_HIGH);
}

void initializeNissanMRvvt(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);
	s->tdcPosition = 0;

	int x = 73;

	// All "groups" start every 90 degrees of cam rotation
	// The groups have 1, 3, 4, 2 teeth each (which is the firing order?)

	// Teeth within a group are spaced 17 cam degrees apart
	int toothSpacing = 17;

	// "1"
	addvq30tooth(s, x + 0);	// <-- sync point here

	// "3"
	addvq30tooth(s, x + 90 + 0 * toothSpacing);
	addvq30tooth(s, x + 90 + 1 * toothSpacing);
	addvq30tooth(s, x + 90 + 2 * toothSpacing);

	// "4"
	addvq30tooth(s, x + 180 + 0 * toothSpacing);
	addvq30tooth(s, x + 180 + 1 * toothSpacing);
	addvq30tooth(s, x + 180 + 2 * toothSpacing);
	addvq30tooth(s, x + 180 + 3 * toothSpacing);

	// "2"
	addvq30tooth(s, x + 270 + 0 * toothSpacing);
	addvq30tooth(s, x + 270 + 1 * toothSpacing);

	// nominal gap 4.31
	s->setTriggerSynchronizationGap2(3.8, 5);

	// nominal gap 0.44
	s->setSecondTriggerSynchronizationGap2(0.3, 0.55);
}

void initializeNissanHRcrank(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);
  size_t count = 9;
  initialize_one_of_36_2_2_2(s, count, 36 - count - 7);
  s->tdcPosition = 155;

  s->setTriggerSynchronizationGap3(/*gapIndex*/0, 2.25, 5.8);
  for (size_t i = 1 ; i < count ; i++) {
    s->setTriggerSynchronizationGap3(/*gapIndex*/i, 0.65, 1.7);
  }
  s->setTriggerSynchronizationGap3(/*gapIndex*/count, 0.1, 0.6);
}
