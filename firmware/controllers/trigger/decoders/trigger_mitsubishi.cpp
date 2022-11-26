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

	s->addEvent720(x + y - 180, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(x, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(x + y, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(x + 180, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(360, TriggerValue::RISE, TriggerWheel::T_PRIMARY);

	s->addEvent720(x + 180 + y, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(x + 360, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(x + 360 + y, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(x + 540, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(720, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
}

/**
 * TT_MITSUBISHI = 11
 */
void initializeMitsubishi4g18(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Fall);

	s->setTriggerSynchronizationGap(1.6666);

	s->addEvent720(106.77999999999997, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent720(120.09999999999998, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(188.0775, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(286.33, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent720(298.875, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(354.91999999999996, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent720(366.6825, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(476.45, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(543.9749999999999, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(639.52, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent720(653.15, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(720.0, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->useOnlyPrimaryForSync = true;
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
	s->addEvent360(120 - w, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(120, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	s->addEvent360(12 + 120 - w, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(12 + 120, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	s->addEvent360(240 - w, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(240, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	s->addEvent360(360 - w, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(360, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	s->setTriggerSynchronizationGap(9);
	s->setSecondTriggerSynchronizationGap(0.11); // redundancy
}

void initializeVvt6G75(TriggerWaveform /* *s */) {
	//	s->shapeWithoutTdc = true;
	//	s->isSynchronizationNeeded = false;
}
