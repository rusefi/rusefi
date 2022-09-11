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
	s->initialize(FOUR_STROKE_CAM_SENSOR);
	s->isSynchronizationNeeded = false;
	s->isSecondWheelCam = true;

	float x = 121.90;
	float y = 110.86;

	s->addEvent720(x + y - 180, T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(x, T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(x + y, T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(x + 180, T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(360, T_PRIMARY, TriggerValue::RISE);

	s->addEvent720(x + 180 + y, T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(x + 360, T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(x + 360 + y, T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(x + 540, T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(720, T_PRIMARY, TriggerValue::FALL);
}

/**
 * TT_MITSUBISHI = 11
 */
void initializeMitsubishi4g18(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);
	s->useRiseEdge = false;

	s->setTriggerSynchronizationGap(1.6666);

	s->addEvent720(106.77999999999997, T_PRIMARY, TriggerValue::FALL);
	s->addEvent720(120.09999999999998, T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(188.0775, T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(286.33, T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(298.875, T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(354.91999999999996, T_PRIMARY, TriggerValue::FALL);
	s->addEvent720(366.6825, T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(476.45, T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(543.9749999999999, T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(639.52, T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(653.15, T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(720.0, T_SECONDARY, TriggerValue::FALL);
	s->useOnlyPrimaryForSync = true;
}

void initialize36_2_1_1(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR);
	s->tdcPosition = 90;
	int totalTeethCount = 36;

	float engineCycle = FOUR_STROKE_ENGINE_CYCLE;
	float toothWidth = 0.5;

	float oneTooth = 720 / totalTeethCount;

	float offset = (36 - 11 - 12 - 11) * oneTooth;

	addSkippedToothTriggerEvents(T_PRIMARY, s, totalTeethCount, 0, toothWidth, /*offset*/offset, engineCycle,
			NO_LEFT_FILTER, offset + 11 * oneTooth + 1);

	offset += (11 + 1) * oneTooth;

	addSkippedToothTriggerEvents(T_PRIMARY, s, totalTeethCount, 0, toothWidth, /*offset*/offset, engineCycle,
			NO_LEFT_FILTER, offset + 11 * oneTooth + 1);


	offset += (11 + 1) * oneTooth;

	addSkippedToothTriggerEvents(T_PRIMARY, s, totalTeethCount, 0, toothWidth, /*offset*/offset, engineCycle,
			NO_LEFT_FILTER, offset + 10 * oneTooth + 1);

	s->setTriggerSynchronizationGap(3);
	s->setSecondTriggerSynchronizationGap(1); // redundancy
}

void initialize36_2_1(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR);
	s->tdcPosition = 90;
	int totalTeethCount = 36;

	float engineCycle = FOUR_STROKE_ENGINE_CYCLE;
	float toothWidth = 0.5;

	float oneTooth = 720 / totalTeethCount;

	float offset = (36 - 17 - 2 - 16) * oneTooth;

	addSkippedToothTriggerEvents(T_PRIMARY, s, totalTeethCount, 0, toothWidth, /*offset*/offset, engineCycle,
			NO_LEFT_FILTER, offset + 17 * oneTooth + 1);

	offset += (17 + 2) * oneTooth;

	addSkippedToothTriggerEvents(T_PRIMARY, s, totalTeethCount, 0, toothWidth, /*offset*/offset, engineCycle,
			NO_LEFT_FILTER, offset + 16 * oneTooth + 1);


	s->setTriggerSynchronizationGap(3);
	s->setSecondTriggerSynchronizationGap(1); // redundancy
}

void initializeVvt3A92(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR);

	int w = 5;
	s->addEvent360(120 - w, T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(120, T_PRIMARY, TriggerValue::FALL);

	s->addEvent360(12 + 120 - w, T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(12 + 120, T_PRIMARY, TriggerValue::FALL);

	s->addEvent360(240 - w, T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(240, T_PRIMARY, TriggerValue::FALL);

	s->addEvent360(360 - w, T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(360, T_PRIMARY, TriggerValue::FALL);

	s->setTriggerSynchronizationGap(9);
	s->setSecondTriggerSynchronizationGap(0.11); // redundancy
}

void initializeVvt6G75(TriggerWaveform *s) {
	//	s->shapeWithoutTdc = true;
	//	s->isSynchronizationNeeded = false;
}
