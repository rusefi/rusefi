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
	int index;
	TriggerWaveform *form;
};

void func(TriggerCallback *callback) {
	int formIndex = callback->index % callback->form->getSize();
	Engine *engine = callback->engine;
	EXPAND_Engine;

	int value = callback->form->wave.channels[0].getState(formIndex);
	trigger_event_e signal;
	if (value) {
		 signal = SHAFT_PRIMARY_RISING;
	} else {
		 signal = SHAFT_PRIMARY_FALLING;
	}
	efitick_t nowNt = getTimeNowNt();
	engine->triggerCentral.handleShaftSignal(signal, nowNt PASS_ENGINE_PARAMETER_SUFFIX);

}

TEST(nissan, vq_vvt) {
	WITH_ENGINE_TEST_HELPER (HELLEN_121_NISSAN);
	engineConfiguration->isIgnitionEnabled = false;
	engineConfiguration->isInjectionEnabled = false;

	{
		static TriggerWaveform crank;
		initializeNissanVQcrank(&crank);

		int totalIndex = 0;

		/**
		 * yet another approach to trigger testing: let's schedule a huge list of events from heap
		 * and then execute those one
		 */
		for (int r = 0; r < 20; r++) {
			for (int i = 0; i < crank.getSize(); i++) {
				float angle = crank.getAngle(totalIndex);
				TriggerCallback *param = new TriggerCallback();
				param->engine = engine;
				param->index = totalIndex;
				param->form = &crank;

				scheduling_s *sch = new scheduling_s();
				engine->executor.scheduleByTimestamp(sch, 1000 * angle, { func, param });
				totalIndex++;
			}
		}
	}

	scheduling_s *head;
	while ((head = engine->executor.getHead()) != nullptr) {
		eth.setTimeAndInvokeEventsUs(head->momentX);
	}

	ASSERT_EQ(250, GET_RPM());
}
