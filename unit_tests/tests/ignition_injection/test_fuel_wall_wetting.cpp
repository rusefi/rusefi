/*
 * @file test_fuel_wall_wetting.cpp
 *
 * See also test_accel_enrichment.cpp
 *
 * @date Oct 16, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

TEST(fuel, testWallWettingEnrichmentMath) {
	WITH_ENGINE_TEST_HELPER(FORD_ASPIRE_1996);

	engineConfiguration->wwaeTau = 1.0f;
	engineConfiguration->wwaeBeta = 0.40f;

	engine->rpmCalculator.setRpmValue(3000);

	WallFuel wallFuel;

	// each invocation of 'adjust' changes WallWetting internal state
	ASSERT_NEAR(16.6666, wallFuel.adjust(10.0), EPS4D);
	ASSERT_NEAR(16.198, wallFuel.adjust(10.0), EPS4D);
}

TEST(fuel, testWallWettingEnrichmentScheduling) {

	WITH_ENGINE_TEST_HELPER(FORD_ASPIRE_1996);

	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;

	eth.setTriggerType(TT_ONE);


	eth.fireTriggerEvents2(/* count */ 5, 25 /* ms */);
	ASSERT_EQ( 1200,  GET_RPM()) << "RPM";

	int expectedInvocationCounter = 1;

	for	(int i = 0; i < 4; i++) {
		ASSERT_EQ(expectedInvocationCounter, ENGINE(injectionEvents.elements[i]).wallFuel.invocationCounter);
	}

	// Cylinder 5 doesn't exist - shouldn't have been called!
	ASSERT_EQ(0, ENGINE(injectionEvents.elements[5]).wallFuel.invocationCounter);

	eth.engine.periodicFastCallback();
	eth.engine.periodicFastCallback();
	eth.engine.periodicFastCallback();

	// still same 1 per cylinder - wall wetting is NOT invoked from 'periodicFastCallback'
	for	(int i = 0; i < 4; i++) {
		ASSERT_EQ(expectedInvocationCounter, ENGINE(injectionEvents.elements[i]).wallFuel.invocationCounter);
	}

	// Cylinder 5 doesn't exist - shouldn't have been called!
	ASSERT_EQ(0, ENGINE(injectionEvents.elements[5]).wallFuel.invocationCounter);
}
