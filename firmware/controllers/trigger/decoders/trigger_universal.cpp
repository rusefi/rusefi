/*
 * @file trigger_universal.cpp
 *
 * @date Jan 3, 2017
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "trigger_universal.h"

/**
 * @see getCycleDuration
 */
angle_t getEngineCycle(operation_mode_e operationMode) {
	return operationMode == TWO_STROKE ? 360 : FOUR_STROKE_ENGINE_CYCLE;
}

void addSkippedToothTriggerEvents(trigger_wheel_e wheel, TriggerWaveform *s, int totalTeethCount, int skippedCount,
		float toothWidth, float offset, float engineCycle, float filterLeft, float filterRight) {
	efiAssertVoid(CUSTOM_ERR_6586, totalTeethCount > 0, "total count");
	efiAssertVoid(CUSTOM_ERR_6587, skippedCount >= 0, "skipped count");

	for (int i = 0; i < totalTeethCount - skippedCount - 1; i++) {
		float angleDown = engineCycle / totalTeethCount * (i + (1 - toothWidth));
		float angleUp = engineCycle / totalTeethCount * (i + 1);
		s->addEventClamped(offset + angleDown, wheel, TV_RISE, filterLeft, filterRight);
		s->addEventClamped(offset + angleUp, wheel, TV_FALL, filterLeft, filterRight);
	}

	float angleDown = engineCycle / totalTeethCount * (totalTeethCount - skippedCount - 1 + (1 - toothWidth));
	s->addEventClamped(offset + angleDown, wheel, TV_RISE, filterLeft, filterRight);
	// custom handling of last event in order to avoid rounding error
	s->addEventClamped(offset + engineCycle, wheel, TV_FALL, filterLeft, filterRight);
}

void initializeSkippedToothTriggerWaveformExt(TriggerWaveform *s, int totalTeethCount, int skippedCount,
		operation_mode_e operationMode) {
	if (totalTeethCount <= 0) {
		firmwareError(CUSTOM_OBD_TRIGGER_WAVEFORM, "Invalid total tooth count for missing tooth decoder: %d", totalTeethCount);
		s->setShapeDefinitionError(true);
		return;
	}
	efiAssertVoid(CUSTOM_NULL_SHAPE, s != NULL, "TriggerWaveform is NULL");
	s->initialize(operationMode);

	s->setTriggerSynchronizationGap(skippedCount + 1);
	s->shapeWithoutTdc = (totalTeethCount > 1) && (skippedCount == 0);
	s->isSynchronizationNeeded = (totalTeethCount > 2) && (skippedCount != 0);


	addSkippedToothTriggerEvents(T_PRIMARY, s, totalTeethCount, skippedCount, 0.5, 0, getEngineCycle(operationMode),
	NO_LEFT_FILTER, NO_RIGHT_FILTER);
}

void configureOnePlusOne(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

	s->addEvent360( 90, T_PRIMARY, TV_RISE);
	s->addEvent360(180, T_PRIMARY, TV_FALL);

	s->addEvent360(270, T_SECONDARY, TV_RISE);
	s->addEvent360(360, T_SECONDARY, TV_FALL);

	s->isSynchronizationNeeded = false;
	s->useOnlyPrimaryForSync = true;
}

void configure3_1_cam(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);


	const float crankW = 360 / 3 / 2;


	trigger_wheel_e crank = T_SECONDARY;

	s->addEvent720(10, T_PRIMARY, TV_RISE);
	s->addEvent720(50, T_PRIMARY, TV_FALL);


	float a = 2 * crankW;

	// #1/3
	s->addEvent720(a += crankW, crank, TV_RISE);
	s->addEvent720(a += crankW, crank, TV_FALL);
	// #2/3
	s->addEvent720(a += crankW, crank, TV_RISE);
	s->addEvent720(a += crankW, crank, TV_FALL);
	// #3/3
	a += crankW;
	a += crankW;

	// 2nd #1/3
	s->addEvent720(a += crankW, crank, TV_RISE);
	s->addEvent720(a += crankW, crank, TV_FALL);

	// 2nd #2/3
	s->addEvent720(a += crankW, crank, TV_RISE);
	s->addEvent720(a += crankW, crank, TV_FALL);

	s->isSynchronizationNeeded = false;
}

/**
 * https://rusefi.com/forum/viewtopic.php?f=5&t=1977
 */
void configureKawaKX450F(TriggerWaveform *s) {
	float engineCycle = FOUR_STROKE_ENGINE_CYCLE;
	s->initialize(FOUR_STROKE_CRANK_SENSOR);

	s->setTriggerSynchronizationGap(2.28);

	float toothWidth = 3 / 20.0;

	addSkippedToothTriggerEvents(T_PRIMARY, s, 18, 0, toothWidth, 0, engineCycle,
			NO_LEFT_FILTER, 720 - 39);

	s->addEvent(0.97, T_PRIMARY, TV_RISE);
	s->addEvent(1, T_PRIMARY, TV_FALL);
}

void configureQuickStartSenderWheel(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

	s->useRiseEdge = false;

	int offset = 20;

	s->setTriggerSynchronizationGap3(0, 2, 3);

	s->addEvent360(offset + 0, T_PRIMARY, TV_RISE);
	s->addEvent360(offset + 70, T_PRIMARY, TV_FALL);

	s->addEvent360(offset + 90, T_PRIMARY, TV_RISE);
	s->addEvent360(offset + 110, T_PRIMARY, TV_FALL);

	s->addEvent360(offset + 180, T_PRIMARY, TV_RISE);
	s->addEvent360(offset + 200, T_PRIMARY, TV_FALL);

	s->addEvent360(offset + 270, T_PRIMARY, TV_RISE);
	s->addEvent360(offset + 340, T_PRIMARY, TV_FALL);
}
