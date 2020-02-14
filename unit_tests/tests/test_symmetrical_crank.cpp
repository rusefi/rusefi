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

	float x = 0.02;

	ASSERT_EQ( 0,  GET_RPM()) << "RPM#0";

	for (int i = 0; i < 8 ; i++) {
		postFourEvents(&eth, x);
		ASSERT_EQ( 0,  GET_RPM()) << "RPM#0";
	}

	x = 0.1;
	postFourEvents(&eth, x);
	ASSERT_EQ( 1041,  GET_RPM()) << "RPM#11";

	postFourEvents(&eth, x);
	ASSERT_EQ( 1041,  GET_RPM()) << "RPM#11";

	postFourEvents(&eth, x);
	ASSERT_EQ( 1041,  GET_RPM()) << "RPM#11";
}
