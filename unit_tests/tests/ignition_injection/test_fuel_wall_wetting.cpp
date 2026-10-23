/*
 * @file test_fuel_wall_wetting.cpp
 *
 * See also test_accel_enrichment.cpp
 *
 * @date Oct 16, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "binary_mlg_logging.h"
#include "mlg_field.h"
#include "value_lookup.h"

struct MockWallController : public IWallFuelController {
	MOCK_METHOD(bool, getEnable, (), (const, override));
	MOCK_METHOD(float, getAlpha, (), (const, override));
	MOCK_METHOD(float, getBeta, (), (const, override));
};

// Exposes the protected tau/beta computation for direct testing
struct TestWallFuelController : public WallFuelController {
	using WallFuelController::computeTau;
	using WallFuelController::computeBeta;
};

// [tag:wall_fuel_telemetry] Model values stay in grams; TS, SD logs and Lua expose the same masses in mg.
TEST(fuel, wallWettingTelemetryMass) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockWallController wallController;
	EXPECT_CALL(wallController, getEnable()).WillRepeatedly(Return(true));
	EXPECT_CALL(wallController, getAlpha()).WillRepeatedly(Return(0.5f));
	EXPECT_CALL(wallController, getBeta()).WillRepeatedly(Return(0.25f));
	engine->engineModules.get<WallFuelController>().set(&wallController);

	auto& wall = engine->injectionEvents.elements[0].getWallFuel();
	EXPECT_NEAR(0.016f, wall.adjust(0.012f), 1e-7f);
	EXPECT_NEAR(0.004f, wall.wallFuel, 1e-7f);
	EXPECT_NEAR(0.004f, wall.wallFuelCorrection, 1e-7f);
	EXPECT_NEAR(0.004f, wall.getWallFuel(), 1e-7f);
	EXPECT_NEAR(4, getOutputValueByName("wallFuel"), 1e-5f);
	EXPECT_NEAR(4, getOutputValueByName("wallFuelCorrection"), 1e-5f);
	EXPECT_FLOAT_EQ(EFI_ERROR_CODE, getOutputValueByName("wallFuelAmount"));
	EXPECT_FLOAT_EQ(EFI_ERROR_CODE, getOutputValueByName("wallFuelCorrectionValue"));

	int matches = 0;
	MLG::forEachField([&](const MLG::Entries::Field& field) {
		if (strcmp(field.getName(), "Fuel Wall: amount") == 0 || strcmp(field.getName(), "Fuel Wall: correction") == 0) {
			matches++;
			EXPECT_STREQ("mg", field.getUnits());
			EXPECT_EQ(sizeof(float), field.getSize());
			EXPECT_FLOAT_EQ(1000, field.getMultiplier());
			EXPECT_NEAR(4, field.readValueAsDouble(), 1e-5f);
		}
	});
	EXPECT_EQ(2, matches);

	// Evaporation supplies more than requested: injection clamps to zero and the correction is negative.
	EXPECT_FLOAT_EQ(0, wall.adjust(0.001f));
	EXPECT_NEAR(0.002f, wall.wallFuel, 1e-7f);
	EXPECT_NEAR(-0.001f, wall.wallFuelCorrection, 1e-7f);
	EXPECT_NEAR(-1, getOutputValueByName("wallFuelCorrection"), 1e-5f);
	MLG::forEachField([&](const MLG::Entries::Field& field) {
		if (strcmp(field.getName(), "Fuel Wall: correction") == 0) {
			EXPECT_NEAR(-1, field.readValueAsDouble(), 1e-5f);
		}
	});

	// Retain float precision below the old packed channel's 0.01 mg resolution.
	wall.resetWF();
	EXPECT_NEAR(0.000016f, wall.adjust(0.000012f), 1e-10f);
	EXPECT_NEAR(0.004f, getOutputValueByName("wallFuel"), 1e-8f);
	EXPECT_NEAR(0.004f, getOutputValueByName("wallFuelCorrection"), 1e-8f);
}

TEST(fuel, wallWettingFlexFuelCompensation) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// Basic (constant) wall model so the base tau/beta are known scalars
	engineConfiguration->complexWallModel = false;
	engineConfiguration->wwaeTau = 0.4f;
	engineConfiguration->wwaeBeta = 0.3f;

	// Flex transient tables: tau x2, beta x1.5 everywhere
	setLinearCurve(config->flexTransientCltBins, -40, 100, 1);
	setLinearCurve(config->flexTransientEthanolBins, 0, 100, 1);
	setTable(config->flexWwTauMult, 2.0f);
	setTable(config->flexWwBetaMult, 1.5f);

	engineConfiguration->flexFuelTransientComp = true;
	Sensor::setMockValue(SensorType::Clt, 20);

	TestWallFuelController dut;

	// No flex sensor -> compensation neutral, base values pass through
	Sensor::resetMockValue(SensorType::FuelEthanolPercent);
	EXPECT_NEAR(0.4f, dut.computeTau(), EPS4D);
	EXPECT_NEAR(0.3f, dut.computeBeta(), EPS4D);

	// E100 with compensation enabled -> tau and beta scaled by the tables
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 100);
	EXPECT_NEAR(0.8f, dut.computeTau(), EPS4D);
	EXPECT_NEAR(0.45f, dut.computeBeta(), EPS4D);

	// Disable flag -> back to neutral even with a flex sensor present
	engineConfiguration->flexFuelTransientComp = false;
	EXPECT_NEAR(0.4f, dut.computeTau(), EPS4D);
	EXPECT_NEAR(0.3f, dut.computeBeta(), EPS4D);
}

TEST(fuel, testWallWettingEnrichmentMath) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

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

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = false;

	setCrankOperationMode();

	eth.setTriggerType(trigger_type_e::TT_HALF_MOON);


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
