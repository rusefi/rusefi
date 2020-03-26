/*
 * @file test_ignition_scheduling.cpp
 *
 * @date Nov 17, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine_test_helper.h"
#include "spark_logic.h"

TEST(ignition, twoCoils) {
	WITH_ENGINE_TEST_HELPER(BMW_M73_F);

	// first one to fire uses first coil
	ASSERT_EQ(ENGINE(ignitionPin[ID2INDEX(1)]), 0);
	ASSERT_EQ(ENGINE(ignitionPin[ID2INDEX(2)]), 1);
	ASSERT_EQ(ENGINE(ignitionPin[ID2INDEX(3)]), 0);
	ASSERT_EQ(ENGINE(ignitionPin[ID2INDEX(4)]), 1);

	ASSERT_EQ(ENGINE(ignitionPin[ID2INDEX(11)]), 0);
	ASSERT_EQ(ENGINE(ignitionPin[ID2INDEX(12)]), 1);

	// let's recalculate with zero timing so that we can focus on relation advance between cylinders
	engine->engineState.timingAdvance = 0;
	initializeIgnitionActions(PASS_ENGINE_PARAMETER_SIGNATURE);

	ASSERT_EQ(engine->ignitionEvents.elements[0].sparkAngle, 0);
	ASSERT_EQ((void*)engine->ignitionEvents.elements[0].outputs[0], (void*)&enginePins.coils[0]);


	ASSERT_EQ(engine->ignitionEvents.elements[1].sparkAngle, 720 / 12);
	ASSERT_EQ((void*)engine->ignitionEvents.elements[1].outputs[0], (void*)&enginePins.coils[6]);

	ASSERT_EQ(engine->ignitionEvents.elements[3].sparkAngle, 3 * 720 / 12);
	ASSERT_EQ((void*)engine->ignitionEvents.elements[3].outputs[0], (void*)&enginePins.coils[6]);


}



