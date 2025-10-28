/*
 * @file	test_tcu.cpp
 *
 * @date Oct 24, 2025
 * @author David Holdeman, (c) 2025
*/

#include "gear_controller.h"
#include "pch.h"

void blipGearControllerPin(EngineTestHelper* eth, brain_pin_e pin, int time) {
	engine->gearController->update();
	// Close switch/button
	setMockState(pin, false);
	engine->gearController->update();

	// update gearController every ms
	for (int i = 0; i < time; i = i+1000) {
		eth->moveTimeForwardAndInvokeEventsUs(minI(time - i, 1000));
		engine->gearController->update();
	}
	// And release
	setMockState(pin, true);
	engine->gearController->update();
}

TEST(tcu, testButtonshift) {
	EngineTestHelper eth(engine_type_e::TCU_4R70W);
	engineConfiguration->gearControllerMode = GearControllerMode::ButtonShift;
	initGearController();

	// pinMode is PI_PULLUP, so true = off
	setMockState(engineConfiguration->tcuUpshiftButtonPin, true);
	setMockState(engineConfiguration->tcuDownshiftButtonPin, true);

	ASSERT_NE(nullptr, engine->gearController);
	ASSERT_EQ(NEUTRAL, engine->gearController->getDesiredGear());

	// Press upshift button for 200ms
	blipGearControllerPin(&eth, engineConfiguration->tcuUpshiftButtonPin, 200000);

	// and now a bounce
	eth.moveTimeForwardAndInvokeEventsUs(20);
	blipGearControllerPin(&eth, engineConfiguration->tcuUpshiftButtonPin, 20);

	ASSERT_EQ(GEAR_1, engine->gearController->getDesiredGear());

	// Wait 500ms
	eth.moveTimeForwardAndInvokeEventsUs(500000);
	// Press upshift button for 200ms
	blipGearControllerPin(&eth, engineConfiguration->tcuUpshiftButtonPin, 200000);

	ASSERT_EQ(GEAR_2, engine->gearController->getDesiredGear());

	// Wait 500ms
	eth.moveTimeForwardAndInvokeEventsUs(500000);
	// Press upshift button for 1ms
	blipGearControllerPin(&eth, engineConfiguration->tcuUpshiftButtonPin, 1000);

	ASSERT_EQ(GEAR_3, engine->gearController->getDesiredGear());

	// Wait 500ms
	eth.moveTimeForwardAndInvokeEventsUs(500000);
	// Press upshift button for 3s
	blipGearControllerPin(&eth, engineConfiguration->tcuUpshiftButtonPin, 3000000);

	ASSERT_EQ(GEAR_4, engine->gearController->getDesiredGear());

	// Wait 10ms
	// Because this is a different pin, the 500ms debounce timeout is not in play.
	eth.moveTimeForwardAndInvokeEventsUs(10000);
	// Press downshift button for 200ms
	blipGearControllerPin(&eth, engineConfiguration->tcuDownshiftButtonPin, 200000);

	ASSERT_EQ(GEAR_3, engine->gearController->getDesiredGear());

	// Wait 10ms
	// This shouldn't be long enough for the debounce to have reset,
	// so the downshift won't trigger until ~490ms after pressing the button.
	eth.moveTimeForwardAndInvokeEventsUs(10000);
	// Press downshift button for 1.2s
	blipGearControllerPin(&eth, engineConfiguration->tcuDownshiftButtonPin, 1200000);

	ASSERT_EQ(GEAR_1, engine->gearController->getDesiredGear());
}

TEST(tcu, testGenericGC) {
	EngineTestHelper eth(engine_type_e::TCU_4R70W);
	engineConfiguration->gearControllerMode = GearControllerMode::Generic;
	initGearController();

	// Need to set some engine settings for airmass calc
	engineConfiguration->cylindersCount = 8.0;

	// pinMode is PI_PULLUP, so true = off
	setMockState(engineConfiguration->tcuUpshiftButtonPin, true);
	setMockState(engineConfiguration->tcuDownshiftButtonPin, true);
	setMockState(engineConfiguration->tcu_rangeInput[1], true);
	setMockState(engineConfiguration->tcu_rangeInput[2], true);

	ASSERT_NE(nullptr, engine->gearController);
	ASSERT_EQ(NEUTRAL, engine->gearController->getDesiredGear());

	Sensor::setMockValue(SensorType::VehicleSpeed, 55);
	Sensor::setMockValue(SensorType::Rpm, 2500);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 15);
	Sensor::setMockValue(SensorType::Maf, 0.1f);

	engine->gearController->update();
	// Make sure we stay in neutral with undefined range selector pins
	ASSERT_EQ(NEUTRAL, engine->gearController->getDesiredGear());

	Sensor::setMockValue(SensorType::RangeInput1, 2000);
	engine->gearController->update();
	ASSERT_EQ(GEAR_2, engine->gearController->getDesiredGear());
}
