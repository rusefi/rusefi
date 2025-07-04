/*
 * @file test_aux_valves.cpp
 *
 * @date: Nov 23, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "aux_valves.h"

TEST(Actuators, AuxValves) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->auxValves[0] = Gpio::E14;
	engineConfiguration->auxValves[1] = Gpio::E12;
	eth.setTriggerType(trigger_type_e::TT_NISSAN_SR20VE);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);

	// force recalculation of aux valves:
	initAuxValves();
	// Engine must be "spinning" for scheduleByAngle to work
	engine->rpmCalculator.setRpmValue(1000);

	auto const auxPlainPinTurnOnAction{ action_s::make<auxPlainPinTurnOn>((AuxActor*){}) };

	// TODO: This whole block is passing some expected values which are ignored by check implementation, fix?

	eth.assertTriggerEvent("a0", 0, &engine->auxValves[0][0].open, auxPlainPinTurnOnAction, 0);
	eth.assertTriggerEvent("a1", 1, &engine->auxValves[0][1].open, auxPlainPinTurnOnAction, 360);
	eth.assertTriggerEvent("a2", 2, &engine->auxValves[1][0].open, auxPlainPinTurnOnAction, 180);
	eth.assertTriggerEvent("a3", 3, &engine->auxValves[1][1].open, auxPlainPinTurnOnAction, 540);

	// Execute the first one, ensure scheduling for the "close" event happens
	engine->module<TriggerScheduler>()->scheduleEventsUntilNextTriggerTooth(1000, 0, 0, 1);

	// Old head should now be missing - we just ran it
	eth.assertTriggerEvent("a1", 0, &engine->auxValves[0][1].open, auxPlainPinTurnOnAction, 360);
	eth.assertTriggerEvent("a2", 1, &engine->auxValves[1][0].open, auxPlainPinTurnOnAction, 180);
	eth.assertTriggerEvent("a3", 2, &engine->auxValves[1][1].open, auxPlainPinTurnOnAction, 540);

	// Execute the action it put on the regular scheduler
	eth.setTimeAndInvokeEventsUs(999999);

	eth.assertTriggerEvent("a1", 0, &engine->auxValves[0][1].open, auxPlainPinTurnOnAction, 360);
	eth.assertTriggerEvent("a2", 1, &engine->auxValves[1][0].open, auxPlainPinTurnOnAction, 180);
	eth.assertTriggerEvent("a3", 2, &engine->auxValves[1][1].open, auxPlainPinTurnOnAction, 540);
	// same event is back at the end of the list
	eth.assertTriggerEvent("a0", 3, &engine->auxValves[0][0].open, auxPlainPinTurnOnAction, 0);
	// PLUS the turn off event!
	action_s const noneAction{};
	eth.assertTriggerEvent("off", 4, &engine->auxValves[0][0].close, noneAction, 30);
}
