/*
 * @file test_fuel_wall_wetting.cpp
 *
 * See also test_accel_enrichment.cpp
 *
 * @date Oct 16, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

struct MockWallController : public IWallFuelController {
	MOCK_METHOD(bool, getEnable, (), (const, override));
	MOCK_METHOD(float, getAlpha, (), (const, override));
	MOCK_METHOD(float, getBeta, (), (const, override));
};

TEST(fuel, testWallWettingEnrichmentMath) {
	EngineTestHelper eth(engine_type_e::FORD_ASPIRE_1996);

	MockWallController wallController;

	// WW is enabled!
	EXPECT_CALL(wallController, getEnable()).WillRepeatedly(Return(true));

	// 1/2 of fuel remains on walls
	EXPECT_CALL(wallController, getAlpha()).WillRepeatedly(Return(0.5f));

	// 1/4 of fuel is lands on walls
	EXPECT_CALL(wallController, getBeta()).WillRepeatedly(Return(0.25f));

	// install our mock in to the engine
	engine->engineModules.get<WallFuelController>().set(&wallController);

	WallFuel wallFuel;

	// each invocation of 'adjust' changes WallWetting internal state
	EXPECT_NEAR(1.3333, wallFuel.adjust(1), EPS4D);
	EXPECT_NEAR(1.1111, wallFuel.adjust(1), EPS4D);
	EXPECT_NEAR(1.0370, wallFuel.adjust(1), EPS4D);
	EXPECT_NEAR(1.0123, wallFuel.adjust(1), EPS4D);

	// get to steady state
	for (size_t i = 0; i < 50; i++) {
		wallFuel.adjust(1);
	}

	EXPECT_NEAR(1, wallFuel.adjust(1), EPS4D);

	// now run half the fuel
	EXPECT_NEAR(0.3333, wallFuel.adjust(0.5), EPS4D);
	EXPECT_NEAR(0.4444, wallFuel.adjust(0.5), EPS4D);
	EXPECT_NEAR(0.4815, wallFuel.adjust(0.5), EPS4D);
	EXPECT_NEAR(0.4938, wallFuel.adjust(0.5), EPS4D);

	for (size_t i = 0; i < 50; i++) {
		wallFuel.adjust(0.5);
	}

	EXPECT_NEAR(0.5, wallFuel.adjust(0.5), EPS4D);
}

TEST(fuel, testWallWettingEnrichmentScheduling) {

	EngineTestHelper eth(engine_type_e::FORD_ASPIRE_1996);
	engineConfiguration->isFasterEngineSpinUpEnabled = false;

	setCrankOperationMode();

	eth.setTriggerType(trigger_type_e::TT_ONE);


	eth.fireTriggerEvents2(/* count */ 4, 25 /* ms */);
	ASSERT_EQ( 1200,  Sensor::getOrZero(SensorType::Rpm)) << "RPM";

	int expectedInvocationCounter = 1;

	for	(int i = 0; i < 4; i++) {
		ASSERT_EQ(expectedInvocationCounter, engine->injectionEvents.elements[i].getWallFuel().invocationCounter);
	}

	// Cylinder 5 doesn't exist - shouldn't have been called!
	ASSERT_EQ(0, engine->injectionEvents.elements[5].getWallFuel().invocationCounter);

	eth.engine.periodicFastCallback();
	eth.engine.periodicFastCallback();
	eth.engine.periodicFastCallback();

	// still same 1 per cylinder - wall wetting is NOT invoked from 'periodicFastCallback'
	for	(int i = 0; i < 4; i++) {
		ASSERT_EQ(expectedInvocationCounter, engine->injectionEvents.elements[i].getWallFuel().invocationCounter);
	}

	// Cylinder 5 doesn't exist - shouldn't have been called!
	ASSERT_EQ(0, engine->injectionEvents.elements[5].getWallFuel().invocationCounter);
}
