/*
 * @file test_aux_valves.cpp
 *
 * @date: Nov 23, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine_test_helper.h"
#include "aux_valves.h"

TEST(misc, testAuxValves) {
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);

	WITH_ENGINE_TEST_HELPER(NISSAN_PRIMERA);

	engine->needTdcCallback = false;

	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth, IM_SEQUENTIAL);
	engineConfiguration->isInjectionEnabled = false;

	eth.fireTriggerEvents2(2 /* count */ , 600 /* ms */);
	ASSERT_EQ( 100,  GET_RPM()) << "spinning-RPM#1";

	eth.assertTriggerEvent("a0", 0, &engine->auxValves[0][0].open, (void*)&auxPlainPinTurnOn, 7, 86);
	eth.assertTriggerEvent("a1", 1, &engine->auxValves[0][1].open, (void*)&auxPlainPinTurnOn, 3, 86);
	eth.assertTriggerEvent("a2", 2, &engine->auxValves[1][0].open, (void*)&auxPlainPinTurnOn, 1, 86);

}
