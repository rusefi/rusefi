/*
 * @file trigger_universal.cpp
 *
 * @date Jan 3, 2017
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "trigger_universal.h"

angle_t getEngineCycle(operation_mode_e operationMode) {
	return operationMode == TWO_STROKE ? 360 : 720;
}

void addSkippedToothTriggerEvents(trigger_wheel_e wheel, TriggerShape *s, int totalTeethCount, int skippedCount,
		float toothWidth, float offset, float engineCycle, float filterLeft, float filterRight DECLARE_ENGINE_PARAMETER_SUFFIX) {
	efiAssertVoid(totalTeethCount > 0, "total count");
	efiAssertVoid(skippedCount >= 0, "skipped count");

	for (int i = 0; i < totalTeethCount - skippedCount - 1; i++) {
		float angleDown = engineCycle / totalTeethCount * (i + (1 - toothWidth));
		float angleUp = engineCycle / totalTeethCount * (i + 1);
		s->addEvent2(offset + angleDown, wheel, TV_RISE, filterLeft, filterRight PASS_ENGINE_PARAMETER_SUFFIX);
		s->addEvent2(offset + angleUp, wheel, TV_FALL, filterLeft, filterRight PASS_ENGINE_PARAMETER_SUFFIX);
	}

	float angleDown = engineCycle / totalTeethCount * (totalTeethCount - skippedCount - 1 + (1 - toothWidth));
	s->addEvent2(offset + angleDown, wheel, TV_RISE, filterLeft, filterRight PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(offset + engineCycle, wheel, TV_FALL, filterLeft, filterRight PASS_ENGINE_PARAMETER_SUFFIX);
}

void initializeSkippedToothTriggerShapeExt(TriggerShape *s, int totalTeethCount, int skippedCount,
		operation_mode_e operationMode DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (totalTeethCount <= 0) {
		warning(CUSTOM_OBD_TRIGGER_SHAPE, "totalTeethCount is zero or less: %d", totalTeethCount);
		s->shapeDefinitionError = true;
		return;
	}
	efiAssertVoid(s != NULL, "TriggerShape is NULL");
	s->initialize(operationMode, false);

	s->setTriggerSynchronizationGap(skippedCount + 1);
	s->isSynchronizationNeeded = (totalTeethCount > 2) && (skippedCount != 0);

	addSkippedToothTriggerEvents(T_PRIMARY, s, totalTeethCount, skippedCount, 0.5, 0, getEngineCycle(operationMode),
	NO_LEFT_FILTER, NO_RIGHT_FILTER PASS_ENGINE_PARAMETER_SUFFIX);
}


void configureOnePlusOne(TriggerShape *s, operation_mode_e operationMode DECLARE_ENGINE_PARAMETER_SUFFIX) {
	float engineCycle = getEngineCycle(operationMode);

	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

	s->addEvent2(180, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(360, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent2(540, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(720, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->isSynchronizationNeeded = false;
	s->useOnlyPrimaryForSync = true;
}

void configureOnePlus60_2(TriggerShape *s, operation_mode_e operationMode DECLARE_ENGINE_PARAMETER_SUFFIX) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

	int totalTeethCount = 60;
	int skippedCount = 2;

	s->addEvent2(2, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	addSkippedToothTriggerEvents(T_SECONDARY, s, totalTeethCount, skippedCount, 0.5, 0, 360, 2, 20 PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(20, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
	addSkippedToothTriggerEvents(T_SECONDARY, s, totalTeethCount, skippedCount, 0.5, 0, 360, 20, NO_RIGHT_FILTER PASS_ENGINE_PARAMETER_SUFFIX);

	addSkippedToothTriggerEvents(T_SECONDARY, s, totalTeethCount, skippedCount, 0.5, 360, 360, NO_LEFT_FILTER,
	NO_RIGHT_FILTER PASS_ENGINE_PARAMETER_SUFFIX);

	s->isSynchronizationNeeded = false;
	s->useOnlyPrimaryForSync = true;
}

void configure3_1_cam(TriggerShape *s, operation_mode_e operationMode DECLARE_ENGINE_PARAMETER_SUFFIX) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);


	const float crankW = 360 / 3 / 2;


	trigger_wheel_e crank = T_SECONDARY;

	s->addEvent2(10, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(50, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);


	float a = 2 * crankW;

	// #1/3
	s->addEvent2(a += crankW, crank, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(a += crankW, crank, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
	// #2/3
	s->addEvent2(a += crankW, crank, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(a += crankW, crank, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
	// #3/3
	a += crankW;
	a += crankW;

	// 2nd #1/3
	s->addEvent2(a += crankW, crank, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(a += crankW, crank, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	// 2nd #2/3
	s->addEvent2(a += crankW, crank, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(a += crankW, crank, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->isSynchronizationNeeded = false;
}
