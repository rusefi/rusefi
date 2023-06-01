/**
 * @file	trigger_mitsubishi.cpp
 *
 * @date Aug 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "trigger_mitsubishi.h"
#include "trigger_universal.h"

void configureFordAspireTriggerWaveform(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);
	s->isSynchronizationNeeded = false;
	s->isSecondWheelCam = true;

	float x = 121.90;
	float y = 110.86;

	s->addEvent720(x + y - 180, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(x, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(x + y, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(x + 180, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(360, true, TriggerWheel::T_PRIMARY);

	s->addEvent720(x + 180 + y, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(x + 360, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(x + 360 + y, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(x + 540, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(720, false, TriggerWheel::T_PRIMARY);
}

static void addMitsu93(TriggerWaveform *s, bool addSecondary) {
	float narrowW = 66;

	float offset = addSecondary ? 0 : 120;

	s->addEvent720(offset + 60, true, TriggerWheel::T_PRIMARY);
	if (addSecondary)
		s->addEvent720(offset + 180 - narrowW, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(offset + 146, false, TriggerWheel::T_PRIMARY);
	if (addSecondary)
			s->addEvent720(offset + 180, false, TriggerWheel::T_SECONDARY);

	if (addSecondary)
			s->addEvent720(offset + 360 - narrowW, true, TriggerWheel::T_SECONDARY);
	if (addSecondary)
			s->addEvent720(offset + 360, false, TriggerWheel::T_SECONDARY);

	s->addEvent720(offset + 421, true, TriggerWheel::T_PRIMARY);
	if (addSecondary)
			s->addEvent720(offset + 540 - narrowW, true, TriggerWheel::T_SECONDARY);
	if (addSecondary)
			s->addEvent720(offset + 540, false, TriggerWheel::T_SECONDARY);

	s->addEvent720(offset + 600, false, TriggerWheel::T_PRIMARY);
	if (addSecondary)
			s->addEvent720(offset + 720 - narrowW, true, TriggerWheel::T_SECONDARY);
	if (addSecondary)
			s->addEvent720(offset + 720.0, false, TriggerWheel::T_SECONDARY);

	s->useOnlyPrimaryForSync = addSecondary;
}

/**
 * trigger_type_e::TT_MITSUBISHI_4G93 = 11
 */
void initializeMitsubishi4G93(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Fall);

	s->setTriggerSynchronizationGap(1.6666);

    addMitsu93(s, /*addSecondary*/true);
}

void initializeMitsubishi4g9xCam(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Both);

	// nominal gap 0.5
	s->setSecondTriggerSynchronizationGap2(0.2, 0.8);

	// nominal gap 3.0
	s->setTriggerSynchronizationGap2(2.0f, 5.0f);

	// 131 deg before #1 TDC
	s->addEvent720(270, true, TriggerWheel::T_PRIMARY);

	// 49 deg after #1 TDC
	s->addEvent720(450, false, TriggerWheel::T_PRIMARY);

	// 131 deg before #4 TDC
	s->addEvent720(630, true, TriggerWheel::T_PRIMARY);

	// 41 deg before #4 TDC
	s->addEvent720(720, false, TriggerWheel::T_PRIMARY);
}

void initializeMitsubishi4g63Cam(TriggerWaveform *s) {
	// TODO: is this actually the same as 4G9x or not?
	initializeMitsubishi4g9xCam(s);
}

void initializeMitsubishi4gMess(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Both);

	s->setTriggerSynchronizationGap(3.2);

    addMitsu93(s, /*addSecondary*/false);
}

void initialize36_2_1_1(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);
	s->tdcPosition = 90;
	int totalTeethCount = 36;

	float engineCycle = FOUR_STROKE_ENGINE_CYCLE;
	float toothWidth = 0.5;

	float oneTooth = 720 / totalTeethCount;

	float offset = (36 - 11 - 12 - 11) * oneTooth;

	addSkippedToothTriggerEvents(TriggerWheel::T_PRIMARY, s, totalTeethCount, 0, toothWidth, /*offset*/offset, engineCycle,
			NO_LEFT_FILTER, offset + 11 * oneTooth + 1);

	offset += (11 + 1) * oneTooth;

	addSkippedToothTriggerEvents(TriggerWheel::T_PRIMARY, s, totalTeethCount, 0, toothWidth, /*offset*/offset, engineCycle,
			NO_LEFT_FILTER, offset + 11 * oneTooth + 1);


	offset += (11 + 1) * oneTooth;

	addSkippedToothTriggerEvents(TriggerWheel::T_PRIMARY, s, totalTeethCount, 0, toothWidth, /*offset*/offset, engineCycle,
			NO_LEFT_FILTER, offset + 10 * oneTooth + 1);

	s->setTriggerSynchronizationGap(3);
	s->setSecondTriggerSynchronizationGap(1); // redundancy
}

// Mitsubishi 4B11
void initialize36_2_1(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);
	s->tdcPosition = 90;
	int totalTeethCount = 36;

	float engineCycle = FOUR_STROKE_ENGINE_CYCLE;
	float toothWidth = 0.5;

	float oneTooth = 720 / totalTeethCount;

	float offset = (36 - 16 - 2 - 17) * oneTooth;

	addSkippedToothTriggerEvents(TriggerWheel::T_PRIMARY, s, totalTeethCount, 0, toothWidth, /*offset*/offset, engineCycle,
			NO_LEFT_FILTER, offset + 16 * oneTooth + 1);

	offset += (16 + 2) * oneTooth;

	addSkippedToothTriggerEvents(TriggerWheel::T_PRIMARY, s, totalTeethCount, 0, toothWidth, /*offset*/offset, engineCycle,
			NO_LEFT_FILTER, offset + 17 * oneTooth + 1);


	s->setTriggerSynchronizationGap(3);
	s->setSecondTriggerSynchronizationGap(1); // redundancy
}

void initializeVvt3A92(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);

	int w = 5;
	s->addEvent360(120 - w, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(120, false, TriggerWheel::T_PRIMARY);

	s->addEvent360(12 + 120 - w, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(12 + 120, false, TriggerWheel::T_PRIMARY);

	s->addEvent360(240 - w, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(240, false, TriggerWheel::T_PRIMARY);

	s->addEvent360(360 - w, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(360, false, TriggerWheel::T_PRIMARY);

	s->setTriggerSynchronizationGap(9);
	s->setSecondTriggerSynchronizationGap(0.11); // redundancy
}

void initializeVvt6G75(TriggerWaveform /* *s */) {
	//	s->shapeWithoutTdc = true;
	//	s->isSynchronizationNeeded = false;
}
