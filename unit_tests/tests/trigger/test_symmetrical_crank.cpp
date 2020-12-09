/*
 * @file test_symmetrical_crank.cpp
 *
 * @date Feb 14, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine_test_helper.h"

static void postFourEvents(EngineTestHelper *eth, float mult) {
	eth->fireFall(mult * 384);
	eth->fireRise(mult * 16);
	eth->fireFall(mult * 304);
	eth->fireRise(mult * 16);
}

TEST(engine, testSymmetricalCrank) {

	WITH_ENGINE_TEST_HELPER(MAZDA_MIATA_2003);

	// this test is not about isFasterEngineSpinUpEnabled so let's disable it to simplify things
	CONFIG(isFasterEngineSpinUpEnabled) = false;


	ASSERT_EQ(FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR, engine->getOperationMode(PASS_ENGINE_PARAMETER_SIGNATURE));

	float mult = 0.02;

	ASSERT_EQ( 0,  GET_RPM()) << "RPM#0";

	postFourEvents(&eth, mult);
	ASSERT_EQ( 0,  GET_RPM()) << "RPM#0";

	eth.fireFall(mult * 384);
	eth.fireRise(mult * 16);
	eth.fireFall(mult * 304);
	ASSERT_FALSE(engine->triggerCentral.triggerState.shaft_is_synchronized);
	eth.fireRise(mult * 16);
	ASSERT_TRUE(engine->triggerCentral.triggerState.shaft_is_synchronized);

	ASSERT_EQ( 0,  GET_RPM()) << "RPM#0";



	for (int i = 0; i < 6 ; i++) {
		postFourEvents(&eth, mult);
		ASSERT_EQ( 0,  GET_RPM()) << "RPM#0";
	}

	mult = 0.1;
	postFourEvents(&eth, mult);
	ASSERT_EQ( 1042,  GET_RPM()) << "RPM#11";

	postFourEvents(&eth, mult);
	ASSERT_EQ( 1042,  GET_RPM()) << "RPM#11";

	postFourEvents(&eth, mult);
	ASSERT_EQ( 1042,  GET_RPM()) << "RPM#11";
}
