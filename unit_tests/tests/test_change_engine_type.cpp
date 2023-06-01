/*
 * @file test_change_engine_type.cpp
 *
 * @date Jul 14, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

extern engine_configuration_s & activeConfiguration;

TEST(misc, changeEngineType) {
	/**
	 * this configuration has triggerInputDebugPins defined
	 */
	EngineTestHelper eth(engine_type_e::FORD_ASPIRE_1996);

	brain_pin_e brainPin = engineConfiguration->triggerInputDebugPins[0];
	ASSERT_TRUE(brainPin != Gpio::Unassigned);
	ASSERT_TRUE(activeConfiguration.triggerInputDebugPins[0] != Gpio::Unassigned);

	int pinIndex = brainPin_to_index(brainPin);
	ASSERT_TRUE(nullptr != getBrainUsedPin(pinIndex));

	// above we have asserted that triggerInputDebugPins is in fact used
	// now let's change into engine type without triggerInputDebugPins and assert shut down

	setEngineType(engine_type_e::CITROEN_TU3JP);
	ASSERT_TRUE(activeConfiguration.triggerInputDebugPins[0] == Gpio::Unassigned);

	ASSERT_TRUE(engineConfiguration->triggerInputDebugPins[0] == Gpio::Unassigned);
	ASSERT_TRUE(engineConfiguration->triggerInputDebugPins[0] == Gpio::Unassigned);

	ASSERT_TRUE(nullptr == getBrainUsedPin(pinIndex));
}

