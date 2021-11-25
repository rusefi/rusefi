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
	EngineTestHelper eth (FORD_ASPIRE_1996);

	brain_pin_e brainPin = engineConfiguration->triggerInputDebugPins[0];
	ASSERT_TRUE(brainPin != GPIO_UNASSIGNED);
	ASSERT_TRUE(activeConfiguration.triggerInputDebugPins[0] != GPIO_UNASSIGNED);

	int pinIndex = brainPin_to_index(brainPin);
	ASSERT_TRUE(nullptr != getBrainUsedPin(pinIndex));

	// above we have asserted that triggerInputDebugPins is in fact used
	// now let's change into engine type without triggerInputDebugPins and assert shut down

	setEngineType((int)CITROEN_TU3JP);
	ASSERT_TRUE(activeConfiguration.triggerInputDebugPins[0] == GPIO_UNASSIGNED);

	ASSERT_TRUE(engineConfiguration->triggerInputDebugPins[0] == GPIO_UNASSIGNED);
	ASSERT_TRUE(engineConfiguration->triggerInputDebugPins[0] == GPIO_UNASSIGNED);

	ASSERT_TRUE(nullptr == getBrainUsedPin(pinIndex));
}

