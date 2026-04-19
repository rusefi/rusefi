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

/**
 * last fall aligned at 720 and skipped area is right before 720
 */
void addSkippedToothTriggerEvents(TriggerWheel wheel, TriggerWaveform *s, int totalTeethCount, int skippedCount,
		float toothWidthPercentage, float offset, float engineCycle, float filterLeft, float filterRight) {
	criticalAssertVoid(totalTeethCount > 0, "total count");
	criticalAssertVoid(skippedCount >= 0, "skipped count");
	criticalAssertVoid(toothWidthPercentage < 1, "toothWidthPercentage");

	float oneTooth = engineCycle / totalTeethCount;

	for (int i = 0; i < totalTeethCount - skippedCount - 1; i++) {
		float angleDown = oneTooth * (i + (1 - toothWidthPercentage));
		float angleUp = oneTooth * (i + 1);
		s->addEventClamped(offset + angleDown, TriggerValue::RISE, wheel, filterLeft, filterRight);
		s->addEventClamped(offset + angleUp, TriggerValue::FALL, wheel, filterLeft, filterRight);
	}

	float angleDown = oneTooth * (totalTeethCount - skippedCount - 1 + (1 - toothWidthPercentage));
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

	s->isSecondWheelCam = true;
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

	s->addToothRiseFall(360, /* width*/10.80);
}

// TT_VVT_BOSCH_QUICK_START
void configureQuickStartSenderWheel(TriggerWaveform *s) {
	// todo: most cam wheels are defined as 'SyncEdge::Rise' or 'SyncEdge::RiseOnly' shall we unify?
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Fall);

	// our preference is to sync not too close to crank sync point
	s->setTriggerSynchronizationGap(0.645);
	s->setSecondTriggerSynchronizationGap(1.556);

	s->addToothRiseFall(90, /* width*/ 70);
	s->addToothRiseFall(130, /* width*/ 20);
	s->addToothRiseFall(220, /* width*/ 20);
	s->addToothRiseFall(360, /* width*/ 70);
}

static void commonSymmetrical(TriggerWaveform* s, int count, float gapFrom, float gapTo) {
	s->shapeWithoutTdc = true;

	// Sync after 2 good teeth
	for (size_t i = 0; i < 2; i++) {
		/**
		 * https://github.com/rusefi/rusefi/issues/4943#issuecomment-1376289608
		 * gaps would be nice during running but horrible during running
		 * Hopefully we do not want variable gap logic yet?
		 */
		s->setTriggerSynchronizationGap3(i, gapFrom, gapTo);
	}

    float width = 360 / count;

	// Just a single tooth with 50% duty cycle
	s->addEventAngle(width / 2, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEventAngle(width, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
}

// Useful for:
// - Honda 24+1 (set this on crank primary, single tooth cam)
// - AEM 24+1 CAS wheel (same config as Honda)
void configure12ToothCrank(TriggerWaveform* s) {
	s->initialize(FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR, SyncEdge::RiseOnly);

	// 2JZ would be global trigger offset 65 but same wheel could be Honda, not hard coding for now
  commonSymmetrical(s, 12, 0.2f, 3.4f);
}

void configure3ToothCrank(TriggerWaveform* s) {
	s->initialize(FOUR_STROKE_THREE_TIMES_CRANK_SENSOR, SyncEdge::RiseOnly);
  commonSymmetrical(s, 3, 0.5, 1.4);
}

void configure6ToothCrank(TriggerWaveform* s) {
	s->initialize(FOUR_STROKE_SIX_TIMES_CRANK_SENSOR, SyncEdge::RiseOnly);
  commonSymmetrical(s, 6, 0.7, 1.4);
}
