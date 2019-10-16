/*
 * @file test_fuel_wall_wetting.cpp
 *
 * See also test_accel_enrichment.cpp
 *
 * @date Oct 16, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */


#include "engine_test_helper.h"


TEST(fuel, testWallWettingEnrichment) {
	WITH_ENGINE_TEST_HELPER(FORD_ASPIRE_1996);

	engineConfiguration->wwaeTau = 1.0f;
	engineConfiguration->wwaeBeta = 0.40f;

	engine->rpmCalculator.setRpmValue(3000 PASS_ENGINE_PARAMETER_SUFFIX);

	ASSERT_NEAR(16.6666, ENGINE(wallFuel).adjust(0, 10.0 PASS_ENGINE_PARAMETER_SUFFIX), EPS4D);


	ASSERT_NEAR(16.198, ENGINE(wallFuel).adjust(0, 10.0 PASS_ENGINE_PARAMETER_SUFFIX), EPS4D);


}
