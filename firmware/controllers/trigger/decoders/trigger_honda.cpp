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
	s->useOnlyPrimaryForSync = true;
	s->setTriggerSynchronizationGap(6);


	int totalTeethCount = 24;
	int skippedCount = 0;

	addSkippedToothTriggerEvents(TriggerWheel::T_SECONDARY, s, totalTeethCount, skippedCount, 0.5, 0, 720,
	0, 349);

	s->addEvent720(350.0f, false, TriggerWheel::T_PRIMARY);
	s->addEvent720(360.0f, true, TriggerWheel::T_PRIMARY);

	s->addEvent720(360 + 0.2, false, TriggerWheel::T_SECONDARY);

	addSkippedToothTriggerEvents(TriggerWheel::T_SECONDARY, s, totalTeethCount, skippedCount, 0.5, 0, 720,
	361, 649);



	s->addEvent720(650.0f, false, TriggerWheel::T_PRIMARY);
	s->addEvent720(660.0f, true, TriggerWheel::T_PRIMARY);

	s->addEvent720(660 + 0.2, false, TriggerWheel::T_SECONDARY);


	addSkippedToothTriggerEvents(TriggerWheel::T_SECONDARY, s, totalTeethCount, skippedCount, 0.5, 0, 720,
	661, 709);


//	exit(-1);

	s->addEvent720(710.0f, false, TriggerWheel::T_PRIMARY);

	s->addEvent720(720.0f - 1, false, TriggerWheel::T_SECONDARY);

	s->addEvent720(720.0f, true, TriggerWheel::T_PRIMARY);
}

void configureOnePlus16(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

	int count = 16;
	float tooth = s->getCycleDuration() / count;
	float width = tooth / 2; // for VR we only handle rises so width does not matter much

	s->addEventAngle(1, true, TriggerWheel::T_PRIMARY);
	s->addEventAngle(5, false, TriggerWheel::T_PRIMARY);

	for (int i = 1; i <= count; i++) {
		s->addEventAngle(tooth * i - width, true, TriggerWheel::T_SECONDARY);
		s->addEventAngle(tooth * i,         false, TriggerWheel::T_SECONDARY);
	}

	s->isSynchronizationNeeded = false;
}

static void kseriesTooth(TriggerWaveform* s, float end) {
	// for VR we only handle rises so width does not matter much
	s->addEvent360(end - 4, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(end    , false, TriggerWheel::T_PRIMARY);
}

// trigger_type_e::TT_HONDA_K_CRANK_12_1
void configureHondaK_12_1(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);

	// nominal gap 0.33
	s->setSecondTriggerSynchronizationGap2(0.2f, 0.7f);
	// nominal gap 2.0
	s->setTriggerSynchronizationGap2(1.1f, 2.4f);

	int count = 12;
	float tooth = 360 / count; // hint: tooth = 30

	// Extra "+1" tooth happens 1/3 of the way between first two teeth
	kseriesTooth(s, tooth / 3);

	for (int i = 1; i <= count; i++) {
		kseriesTooth(s, tooth * i);
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

	angle_t start = 55.5;
	angle_t end = 70.5;
	s->addEvent360(start + 90 * 0, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(end + 90 * 0, false, TriggerWheel::T_PRIMARY);

	s->addEvent360(start + 90 * 1, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(end + 90 * 1, false, TriggerWheel::T_PRIMARY);

	s->addEvent360(start + 90 * 2, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(end + 90 * 2, false, TriggerWheel::T_PRIMARY);

	s->addEvent360(start + 90 * 3, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(end + 90 * 3, false, TriggerWheel::T_PRIMARY);

	s->addEvent360(353, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(360, false, TriggerWheel::T_PRIMARY);
}
