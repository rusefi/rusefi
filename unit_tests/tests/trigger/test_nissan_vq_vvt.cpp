/*
 * @file test_nissan_vq_vvt.cpp
 *
 *  Created on: Jul 2, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "engine_test_helper.h"
#include "trigger_nissan.h"

class TriggerCallback {
public:
	Engine *engine;
	int toothIndex;
	TriggerWaveform *form;
	bool isVvt;
	int vvtIndex;
};

void func(TriggerCallback *callback) {
	int formIndex = callback->toothIndex % callback->form->getSize();
	Engine *engine = callback->engine;
	EXPAND_Engine;

	int value = callback->form->wave.channels[0].getState(formIndex);
	efitick_t nowNt = getTimeNowNt();
	if (callback->isVvt) {
		trigger_value_e v = value ? TV_RISE : TV_FALL;
		hwHandleVvtCamSignal(v, nowNt, callback->vvtIndex PASS_ENGINE_PARAMETER_SUFFIX);
	} else {
		handleShaftSignal(0, value, nowNt PASS_ENGINE_PARAMETER_SUFFIX);
	}
}


static void scheduleTriggerEvents(TriggerWaveform *shape, int count, bool isVvt,
		int vvtIndex,
		int vvtOffset
		DECLARE_ENGINE_PARAMETER_SUFFIX) {
	int totalIndex = 0;

	/**
	 * yet another approach to trigger testing: let's schedule a huge list of events from heap
	 * and then execute those one
	 */
	for (int r = 0; r < count; r++) {
		for (int i = 0; i < shape->getSize(); i++) {
			float angle = vvtOffset + shape->getAngle(totalIndex);
			TriggerCallback *param = new TriggerCallback();
			param->engine = engine;
			param->toothIndex = totalIndex;
			param->form = shape;
			param->isVvt = isVvt;
			param->vvtIndex = vvtIndex;

			scheduling_s *sch = new scheduling_s();
			engine->executor.scheduleByTimestamp(sch, 1000 * angle, { func, param });
			totalIndex++;
		}
	}
}


TEST(nissan, vq_vvt) {
	WITH_ENGINE_TEST_HELPER (HELLEN_121_NISSAN);
	engineConfiguration->isIgnitionEnabled = false;
	engineConfiguration->isInjectionEnabled = false;

	int cyclesCount = 12;

	angle_t offsetBetweenCams = 360;

	{
		static TriggerWaveform crank;
		initializeNissanVQcrank(&crank);

		scheduleTriggerEvents(&crank, cyclesCount, false, -1, 0 PASS_ENGINE_PARAMETER_SUFFIX);
	}

	{
		static TriggerWaveform vvt;
		initializeNissanVQvvt(&vvt);

		scheduleTriggerEvents(&vvt, cyclesCount / 6, true,
				/* vvtIndex */ 0,
				/* vvtOffset */ 0
				PASS_ENGINE_PARAMETER_SUFFIX);
	}

	{
		static TriggerWaveform vvt;
		initializeNissanVQvvt(&vvt);

		scheduleTriggerEvents(&vvt, cyclesCount / 6, true,
				/* vvtIndex */1,
				/* vvtOffset */ offsetBetweenCams
				PASS_ENGINE_PARAMETER_SUFFIX);
	}

	scheduling_s *head;
	while ((head = engine->executor.getHead()) != nullptr) {
		eth.setTimeAndInvokeEventsUs(head->momentX);
	}

	ASSERT_EQ(250, GET_RPM());
	angle_t firstVVTangle = -7.5;
	ASSERT_EQ(firstVVTangle, engine->triggerCentral.currentVVTEventPosition[0][0]);
	// hmm, why 540 not 360?
	// actually for any vvtOffset in this test the offset between cam shafts is somehow 1.5 * offsetBetweenCams?
	ASSERT_EQ(firstVVTangle + offsetBetweenCams * 1.5, engine->triggerCentral.currentVVTEventPosition[0][1]);
}
