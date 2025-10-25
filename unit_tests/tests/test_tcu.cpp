/*
 * @file	test_tcu.cpp
 *
 * @date Oct 24, 2025
 * @author David Holdeman, (c) 2025
*/

#include "gear_controller.h"
#include "pch.h"

TEST(tcu, testButtonshift) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->gearControllerMode = GearControllerMode::ButtonShift;
	initGearController();

	ASSERT_NE(nullptr, engine->gearController);
	ASSERT_EQ(NEUTRAL, engine->gearController->getDesiredGear());
}
