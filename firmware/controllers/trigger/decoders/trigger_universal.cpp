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

void addSkippedToothTriggerEvents(TriggerWheel wheel, TriggerWaveform *s, int totalTeethCount, int skippedCount,
		float toothWidth, float offset, float engineCycle, float filterLeft, float filterRight) {
	efiAssertVoid(ObdCode::CUSTOM_ERR_6586, totalTeethCount > 0, "total count");
	efiAssertVoid(ObdCode::CUSTOM_ERR_6587, skippedCount >= 0, "skipped count");

	for (int i = 0; i < totalTeethCount - skippedCount - 1; i++) {
		float angleDown = engineCycle / totalTeethCount * (i + (1 - toothWidth));
		float angleUp = engineCycle / totalTeethCount * (i + 1);
		s->addEventClamped(offset + angleDown, TriggerValue::RISE, wheel, filterLeft, filterRight);
		s->addEventClamped(offset + angleUp, TriggerValue::FALL, wheel, filterLeft, filterRight);
	}

	float angleDown = engineCycle / totalTeethCount * (totalTeethCount - skippedCount - 1 + (1 - toothWidth));
	s->addEventClamped(offset + angleDown, TriggerValue::RISE, wheel, filterLeft, filterRight);
	// custom handling of last event in order to avoid rounding error
	s->addEventClamped(offset + engineCycle, TriggerValue::FALL, wheel, filterLeft, filterRight);
}

void initializeSkippedToothTrigger(TriggerWaveform *s, int totalTeethCount, int skippedCount,
		operation_mode_e operationMode, SyncEdge syncEdge) {
	if (totalTeethCount <= 0) {
		firmwareError(ObdCode::CUSTOM_OBD_TRIGGER_WAVEFORM, "Invalid total tooth count for missing tooth decoder: %d", totalTeethCount);
		s->setShapeDefinitionError(true);
		return;
	}
	efiAssertVoid(ObdCode::CUSTOM_NULL_SHAPE, s != NULL, "TriggerWaveform is NULL");

	s->initialize(operationMode, syncEdge);

#if EFI_UNIT_TEST
	s->knownOperationMode = false;
#endif // EFI_UNIT_TEST

	s->setTriggerSynchronizationGap(skippedCount + 1);
	if (totalTeethCount > 6 && skippedCount > 0) {
		// this gap is not required to synch on perfect signal but is needed to handle to reject cranking transition noise and potentially high rev noise as well
		s->setSecondTriggerSynchronizationGap(1);
	}
	s->shapeWithoutTdc = (totalTeethCount > 1) && (skippedCount == 0);
	s->isSynchronizationNeeded = (totalTeethCount > 2) && (skippedCount != 0);


	addSkippedToothTriggerEvents(TriggerWheel::T_PRIMARY, s, totalTeethCount, skippedCount, 0.5, 0, getEngineCycle(operationMode),
	NO_LEFT_FILTER, NO_RIGHT_FILTER);
}

void configureOnePlusOne(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Rise);

	s->addEvent360( 90, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(180, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	s->addEvent360(270, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent360(360, TriggerValue::FALL, TriggerWheel::T_SECONDARY);

	s->isSynchronizationNeeded = false;
	s->useOnlyPrimaryForSync = true;
}

void configure3_1_cam(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);


	const float crankW = 360 / 3 / 2;


	TriggerWheel crank = TriggerWheel::T_SECONDARY;

	s->addEvent720(10, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent720(50, TriggerValue::FALL, TriggerWheel::T_PRIMARY);


	float a = 2 * crankW;

	// #1/3
	s->addEvent720(a += crankW, TriggerValue::RISE, crank);
	s->addEvent720(a += crankW, TriggerValue::FALL, crank);
	// #2/3
	s->addEvent720(a += crankW, TriggerValue::RISE, crank);
	s->addEvent720(a += crankW, TriggerValue::FALL, crank);
	// #3/3
	a += crankW;
	a += crankW;

	// 2nd #1/3
	s->addEvent720(a += crankW, TriggerValue::RISE, crank);
	s->addEvent720(a += crankW,  TriggerValue::FALL, crank);

	// 2nd #2/3
	s->addEvent720(a += crankW, TriggerValue::RISE, crank);
	s->addEvent720(a += crankW, TriggerValue::FALL, crank);

	s->isSynchronizationNeeded = false;
}

/**
 * https://rusefi.com/forum/viewtopic.php?f=5&t=1977
 */
void configureKawaKX450F(TriggerWaveform *s) {
	float engineCycle = FOUR_STROKE_ENGINE_CYCLE;
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::Rise);

	s->setTriggerSynchronizationGap(2.28);

	float toothWidth = 3 / 20.0;

	addSkippedToothTriggerEvents(TriggerWheel::T_PRIMARY, s, 18, 0, toothWidth, 0, engineCycle,
			NO_LEFT_FILTER, 720 - 39);

	s->addEvent(0.97, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent(1, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
}

void configureQuickStartSenderWheel(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Fall);

	int offset = 20;

	// our preference is to sync not too close to crank sync point
	s->setTriggerSynchronizationGap(0.645);
	s->setSecondTriggerSynchronizationGap(1.556);

	s->addEvent360(offset + 0, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(offset + 70, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	s->addEvent360(offset + 90, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(offset + 110, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	s->addEvent360(offset + 180, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(offset + 200, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	s->addEvent360(offset + 270, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(offset + 340, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
}

// Useful for:
// - Honda 24+1 (set this on crank primary, single tooth cam)
// - AEM 24+1 CAS wheel (same config as Honda)
void configure12ToothCrank(TriggerWaveform* s) {
	s->initialize(FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR, SyncEdge::RiseOnly);

	// 2JZ would be global trigger offset 65 but same wheel could be Honda, not hard coding for now
	s->shapeWithoutTdc = true;

	// Sync after 2 good teeth
	for (size_t i = 0; i < 2; i++) {
		/**
		 * https://github.com/rusefi/rusefi/issues/4943#issuecomment-1376289608
		 * gaps would be nice during running but horrible during running
		 * Hopefully we do not want variable gap logic yet?
		 */
		s->setTriggerSynchronizationGap3(i, 0.2f, 3.4f);
	}

    float width = 360 / 12;

	// Just a single tooth with 50% duty cycle
	s->addEventAngle(width / 2, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEventAngle(width, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
}
