/*
 * @file test_aux_valves.cpp
 *
 * @date: Nov 23, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "aux_valves.h"

TEST(Actuators, AuxValves) {
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);

	EngineTestHelper eth(engine_type_e::NISSAN_PRIMERA);

	// Engine must be "spinning" for scheduleByAngle to work
	engine->rpmCalculator.setRpmValue(1000);

	eth.assertTriggerEvent("a0", 0, &engine->auxValves[0][0].open, (void*)&auxPlainPinTurnOn, 0);
	eth.assertTriggerEvent("a1", 1, &engine->auxValves[0][1].open, (void*)&auxPlainPinTurnOn, 360);
	eth.assertTriggerEvent("a2", 2, &engine->auxValves[1][0].open, (void*)&auxPlainPinTurnOn, 180);
	eth.assertTriggerEvent("a3", 3, &engine->auxValves[1][1].open, (void*)&auxPlainPinTurnOn, 540);

	// Execute the first one, ensure scheduling for the "close" event happens
	engine->module<TriggerScheduler>()->scheduleEventsUntilNextTriggerTooth(1000, 0, 0, 1);

	// Old head should now be missing - we just ran it
	eth.assertTriggerEvent("a1", 0, &engine->auxValves[0][1].open, (void*)&auxPlainPinTurnOn, 360);
	eth.assertTriggerEvent("a2", 1, &engine->auxValves[1][0].open, (void*)&auxPlainPinTurnOn, 180);
	eth.assertTriggerEvent("a3", 2, &engine->auxValves[1][1].open, (void*)&auxPlainPinTurnOn, 540);

	// Execute the action it put on the regular scheduler
	eth.setTimeAndInvokeEventsUs(999999);

	eth.assertTriggerEvent("a1", 0, &engine->auxValves[0][1].open, (void*)&auxPlainPinTurnOn, 360);
	eth.assertTriggerEvent("a2", 1, &engine->auxValves[1][0].open, (void*)&auxPlainPinTurnOn, 180);
	eth.assertTriggerEvent("a3", 2, &engine->auxValves[1][1].open, (void*)&auxPlainPinTurnOn, 540);
	// same event is back at the end of the list
	eth.assertTriggerEvent("a0", 3, &engine->auxValves[0][0].open, (void*)&auxPlainPinTurnOn, 0);
	// PLUS the turn off event!
	eth.assertTriggerEvent("off", 4, &engine->auxValves[0][0].close, nullptr, 30);
}
