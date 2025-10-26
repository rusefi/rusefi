/*
 * @file	test_tcu.cpp
 *
 * @date Oct 24, 2025
 * @author David Holdeman, (c) 2025
*/

#include "gear_controller.h"
#include "pch.h"
#include "rusefi_enums.h"

TEST(tcu, testButtonshift) {
	EngineTestHelper eth(engine_type_e::TCU_4R70W);
	engineConfiguration->gearControllerMode = GearControllerMode::ButtonShift;
	initGearController();

	// pinMode is PI_PULLUP, so true = off        
	setMockState(engineConfiguration->tcuUpshiftButtonPin, true);

	ASSERT_NE(nullptr, engine->gearController);
	ASSERT_EQ(NEUTRAL, engine->gearController->getDesiredGear());

	engine->gearController->update();

	// Close switch/button        
	setMockState(engineConfiguration->tcuUpshiftButtonPin, false);
	engine->gearController->update();
	// For 200ms
	eth.moveTimeForwardAndInvokeEventsUs(200000);
	// And release        
	setMockState(engineConfiguration->tcuUpshiftButtonPin, true);

	engine->gearController->update();

	ASSERT_EQ(GEAR_1, engine->gearController->getDesiredGear());
}
