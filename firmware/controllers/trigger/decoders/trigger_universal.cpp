/*
 * @file trigger_universal.cpp
 *
 * @date Jan 3, 2017
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "trigger_universal.h"

angle_t getEngineCycle(operation_mode_e operationMode) {
	return operationMode == TWO_STROKE ? 360 : 720;
}

void addSkippedToothTriggerEvents(trigger_wheel_e wheel, TriggerShape *s, int totalTeethCount, int skippedCount,
		float toothWidth, float offset, float engineCycle, float filterLeft, float filterRight DECLARE_ENGINE_PARAMETER_S) {
	efiAssertVoid(totalTeethCount > 0, "total count");
	efiAssertVoid(skippedCount >= 0, "skipped count");

	for (int i = 0; i < totalTeethCount - skippedCount - 1; i++) {
		float angleDown = engineCycle / totalTeethCount * (i + (1 - toothWidth));
		float angleUp = engineCycle / totalTeethCount * (i + 1);
		s->addEvent2(offset + angleDown, wheel, TV_RISE, filterLeft, filterRight PASS_ENGINE_PARAMETER);
		s->addEvent2(offset + angleUp, wheel, TV_FALL, filterLeft, filterRight PASS_ENGINE_PARAMETER);
	}

	float angleDown = engineCycle / totalTeethCount * (totalTeethCount - skippedCount - 1 + (1 - toothWidth));
	s->addEvent2(offset + angleDown, wheel, TV_RISE, filterLeft, filterRight PASS_ENGINE_PARAMETER);
	s->addEvent2(offset + engineCycle, wheel, TV_FALL, filterLeft, filterRight PASS_ENGINE_PARAMETER);
}

void initializeSkippedToothTriggerShapeExt(TriggerShape *s, int totalTeethCount, int skippedCount,
		operation_mode_e operationMode DECLARE_ENGINE_PARAMETER_S) {
	efiAssertVoid(totalTeethCount > 0, "totalTeethCount is zero");
	efiAssertVoid(s != NULL, "TriggerShape is NULL");
	s->initialize(operationMode, false);

	s->setTriggerSynchronizationGap(skippedCount + 1);
	s->isSynchronizationNeeded = (skippedCount != 0);

	addSkippedToothTriggerEvents(T_PRIMARY, s, totalTeethCount, skippedCount, 0.5, 0, getEngineCycle(operationMode),
	NO_LEFT_FILTER, NO_RIGHT_FILTER PASS_ENGINE_PARAMETER);
}

