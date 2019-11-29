/*
 * @file test_aux_valves.cpp
 *
 * @date: Nov 23, 2019
 * @Author: Andrey Belomutskiy, (c) 2012-2019
 */

#include "engine_test_helper.h"
#include "aux_valves.h"

TEST(misc, testAuxValves) {
	WITH_ENGINE_TEST_HELPER(NISSAN_PRIMERA);

	engine->auxSchedulingIndex = 0;
	engine->needTdcCallback = false;

	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth, IM_SEQUENTIAL);
	engineConfiguration->isInjectionEnabled = false;

	eth.fireTriggerEvents2(2 /* count */ , 600 /* ms */);
	ASSERT_EQ( 100,  GET_RPM()) << "spinning-RPM#1";

	scheduling_s *sch;

	sch = eth.assertScheduling("a0", 0, (void*)&engine->auxTurnOnEvent, &plainPinTurnOn, -600000);

	sch = eth.assertScheduling("a1", 1, (void*)&engine->auxTurnOffEvent, &plainPinTurnOff, -550000);
	sch = eth.assertScheduling("a2", 2, (void*)&engine->auxTurnOnEvent[1][0][0], &plainPinTurnOn, -300000);

}
