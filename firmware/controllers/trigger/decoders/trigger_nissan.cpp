/**
 * @file	trigger_nissan.cpp
 *
 * https://rusefi.com/forum/viewtopic.php?f=3&t=1194&start=150#p27784
 *
 * @date Sep 19, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "trigger_nissan.h"
#include "trigger_universal.h"

/**
 * 8,2,2,2 Nissan pattern
 */
static void initializeNissanSR20VE_4_optional_360(TriggerWaveform *s, bool with2nd) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);
	s->gapBothDirections = true;
	s->useOnlyPrimaryForSync = true;

	s->tdcPosition = 630;

	float width = 4;
	s->setTriggerSynchronizationGap2(9.67 * 0.75, 16);

	float left = 0;
	float right;

	int total = 360; // 360 on cam, over 720 crank degree

	float base = 180;
	right = base - 4 * width;

	if (with2nd) {
		addSkippedToothTriggerEvents(T_SECONDARY, s, total, 0, 0.5, 0, 720,
				left, right);
	}

	s->addEvent720(right, T_PRIMARY, TV_RISE);

	left = right;
	right = base;
	if (with2nd) {
		addSkippedToothTriggerEvents(T_SECONDARY, s, total, 0, 0.5, 0, 720,
				left, right);
	}
	s->addEvent720(right, T_PRIMARY, TV_FALL);

	// was is the the one with 360 opto sensor?

	base += 180;

	left = right;
	right = base - width;
	if (with2nd) {
//		addSkippedToothTriggerEvents(T_SECONDARY, s, total, 0, 0.5, 0, 720,
//				left, right PASS_ENGINE_PARAMETER_SUFFIX);
	}
	s->addEvent720(right, T_PRIMARY, TV_RISE);

	left = right;
	right = base;
	if (with2nd) {
//		addSkippedToothTriggerEvents(T_SECONDARY, s, total, 0, 0.5, 0, 720,
//				left, right PASS_ENGINE_PARAMETER_SUFFIX);
	}
	s->addEvent720(right, T_PRIMARY, TV_FALL);

	base += 180;
	left = right;
	right = base - width;
	if (with2nd) {
//		addSkippedToothTriggerEvents(T_SECONDARY, s, total, 0, 0.5, 0, 720,
//				left, right PASS_ENGINE_PARAMETER_SUFFIX);
	}
	s->addEvent720(right, T_PRIMARY, TV_RISE);

	left = right;
	right = base;
	if (with2nd) {
//		addSkippedToothTriggerEvents(T_SECONDARY, s, total, 0, 0.5, 0, 720,
//				left, right PASS_ENGINE_PARAMETER_SUFFIX);
	}
	s->addEvent720(right, T_PRIMARY, TV_FALL);

	base += 180;
	left = right;
	right = base - width;

	s->addEvent720(right, T_PRIMARY, TV_RISE);
	left = right;
	right = base;
	s->addEvent720(right, T_PRIMARY, TV_FALL);
}


/**
 * Nissan Primera p11 year 1995-2002
 */

void initializeNissanSR20VE_4(TriggerWaveform *s) {
	initializeNissanSR20VE_4_optional_360(s, false);
}

void initializeNissanSR20VE_4_360(TriggerWaveform *s) {
	initializeNissanSR20VE_4_optional_360(s, true);
}

static void addPrimaryToothEndingAt(TriggerWaveform *s, float fallAngle) {
	int vvtWidth = 20;

	s->addEventAngle(fallAngle - vvtWidth, T_PRIMARY, TV_RISE);
	s->addEventAngle(fallAngle, T_PRIMARY, TV_FALL);

}

void initializeNissanVQvvt(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

	int offset = 720 - 520;

	addPrimaryToothEndingAt(s, offset + 40);
	addPrimaryToothEndingAt(s, offset + 160);
	addPrimaryToothEndingAt(s, offset + 200);
	addPrimaryToothEndingAt(s, offset + 280);
	addPrimaryToothEndingAt(s, offset + 320);
	addPrimaryToothEndingAt(s, offset + 520);

	s->setTriggerSynchronizationGap(5);
}

void makeNissanPattern(TriggerWaveform* s, size_t halfCylinderCount, size_t totalWheel, size_t missing) {
	s->setTriggerSynchronizationGap(0.33);

	auto toothAngle = 360.0f / totalWheel;

	auto patternTeeth = totalWheel / halfCylinderCount;
	auto toothCount = patternTeeth - missing;
	
	float currentAngle = missing * toothAngle;
	for (int i = 0; i < toothCount; i++) {
		currentAngle += toothAngle;
		s->addEventAngle(currentAngle - 5, T_PRIMARY, TV_RISE);
		s->addEventAngle(currentAngle, T_PRIMARY, TV_FALL);
	}
}

void initializeNissanVQ35crank(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_THREE_TIMES_CRANK_SENSOR);

	s->tdcPosition = 675;

	// 6 cylinder = 36 tooth wheel, missing 2 teeth in 3 spots
	makeNissanPattern(s, 3, 36, 2);
}

void initializeNissanMR18crank(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR);

	s->tdcPosition = 640;

	// 4 cylinder = 36 tooth wheel, missing 2 teeth in 2 spots
	makeNissanPattern(s, 2, 36, 2);
}

void initializeNissanQR25crank(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR);
	s->setTriggerSynchronizationGap(0.33);
	s->setSecondTriggerSynchronizationGap(3);

	s->tdcPosition = 585;

	float currentAngle = 20;
	for (int i = 0;i < 16;i++) {
		currentAngle += 10;
		s->addEventAngle(currentAngle - 5, T_PRIMARY, TV_RISE);
		s->addEventAngle(currentAngle, T_PRIMARY, TV_FALL);
	}
}

static void addvq30tooth(TriggerWaveform *s, float angle) {
	s->addEvent360(angle - 4, T_PRIMARY, TV_RISE);
	s->addEvent360(angle, T_PRIMARY, TV_FALL);
}

void initializeNissanVQ30cam(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

	s->tdcPosition = 00;

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
