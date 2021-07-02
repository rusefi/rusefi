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
	engine->triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING, nowNt PASS_ENGINE_PARAMETER_SUFFIX);

}

TEST(nissan, vq_vvt) {
	WITH_ENGINE_TEST_HELPER (HELLEN_121_NISSAN);

	{

		TriggerWaveform crank;
		initializeNissanVQcrank(&crank);

		int totalIndex = 0;

		for (int r = 0; r < 20; r++) {
			for (int i = 0; i < crank.getSize(); i++) {
				float angle = crank.getAngle(totalIndex);
				TriggerCallback *param = new TriggerCallback();
				param->engine = engine;
				param->index = totalIndex;

				scheduling_s *sch = new scheduling_s();
				engine->executor.scheduleByTimestamp(sch, 1000 * angle, { func, param });
				totalIndex++;
			}
		}
	}

	for (int r = 0; r < 20 * 720; r++) {
//		eth.moveTimeForwardMs(1);
//		eth.executeActions();
	}
	ASSERT_EQ(0, GET_RPM());


}
