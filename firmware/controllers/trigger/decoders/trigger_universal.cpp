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
