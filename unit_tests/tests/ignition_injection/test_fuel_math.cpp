#include "pch.h"
#include "fuel_math.h"
#include "alphan_airmass.h"
#include "maf_airmass.h"
#include "speed_density_airmass.h"
#include "util/injection_crank_helper.h"

using ::testing::StrictMock;
using ::testing::FloatNear;
using ::testing::InSequence;
using ::testing::_;

TEST(FuelMath, getStandardAirCharge) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// Miata 1839cc 4cyl
	engineConfiguration->displacement = 1.839f;
	engineConfiguration->cylindersCount = 4;

	EXPECT_FLOAT_EQ(0.5535934f, getStandardAirCharge());

	// LS 5.3 liter v8
	engineConfiguration->displacement = 5.327f;
	engineConfiguration->cylindersCount = 8;

	EXPECT_FLOAT_EQ(0.80179232f, getStandardAirCharge());

	// Chainsaw - single cylinder 32cc
	engineConfiguration->displacement = 0.032f;
	engineConfiguration->cylindersCount = 1;
	EXPECT_FLOAT_EQ(0.038531788f, getStandardAirCharge());

	// Leopard 1 47.666 liter v12
	engineConfiguration->displacement = 47.666f;
	engineConfiguration->cylindersCount = 12;

	EXPECT_FLOAT_EQ(4.782959f, getStandardAirCharge());
}

TEST(AirmassModes, AlphaNNormal) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// 4 cylinder 4 liter = easy math
	engineConfiguration->displacement = 4.0f;
	engineConfiguration->cylindersCount = 4;

	StrictMock<MockVp3d> veTable;

	EXPECT_CALL(veTable, getValue(1200, FloatNear(0.71f, EPS4D)))
		.WillOnce(Return(35.0f));

	AlphaNAirmass dut(veTable);

	// that's 0.71% not 71%
	Sensor::setMockValue(SensorType::Tps1, 0.71f);

	// Mass of 1 liter of air * VE
	mass_t expectedAirmass = 1.2047f * 0.35f;

	auto result = dut.getAirmass(1200, false);
	EXPECT_NEAR(result.CylinderAirmass, expectedAirmass, EPS4D);
	EXPECT_NEAR(result.EngineLoadPercent, 0.71f, EPS4D);
}

TEST(AirmassModes, AlphaNUseIat) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// 4 cylinder 4 liter = easy math
	engineConfiguration->displacement = 4.0f;
	engineConfiguration->cylindersCount = 4;

	StrictMock<MockVp3d> veTable;

	EXPECT_CALL(veTable, getValue(1200, FloatNear(0.71f, EPS4D)))
		.WillRepeatedly(Return(35.0f));

	AlphaNAirmass dut(veTable);

	// that's 0.71% not 71%
	Sensor::setMockValue(SensorType::Tps1, 0.71f);

	// Mass of 1 liter of air * VE
	mass_t expectedAirmass = 1.2047f * 0.35f;

	EXPECT_NEAR(dut.getAirmass(1200, false).CylinderAirmass, expectedAirmass, EPS4D);

	engineConfiguration->alphaNUseIat = true;

	// Cold we get more airmass
	float expectedAirmassCold = expectedAirmass * (273.0f + 20) / (273.0f + 0);
	Sensor::setMockValue(SensorType::Iat, 0);
	EXPECT_NEAR(dut.getAirmass(1200, false).CylinderAirmass, expectedAirmassCold, EPS4D);

	// Hot we get less airmass
	float expectedAirmassHot = expectedAirmass * (273.0f + 20) / (273.0f + 40);
	Sensor::setMockValue(SensorType::Iat, 40);
	EXPECT_NEAR(dut.getAirmass(1200, false).CylinderAirmass, expectedAirmassHot, EPS4D);
}

TEST(AirmassModes, AlphaNFailedTps) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// Shouldn't get called
	StrictMock<MockVp3d> veTable;

	AlphaNAirmass dut(veTable);

	// explicitly reset the sensor
	Sensor::resetMockValue(SensorType::Tps1);
	// Ensure that it's actually failed
	ASSERT_FALSE(Sensor::get(SensorType::Tps1).Valid);

	auto result = dut.getAirmass(1200, false);
	EXPECT_EQ(result.CylinderAirmass, 0);
}

TEST(AirmassModes, MafNormal) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->displacement = 1.3;
	engineConfiguration->fuelAlgorithm = engine_load_mode_e::LM_REAL_MAF;
	engineConfiguration->injector.flow = 200;

	MockVp3d veTable;
	// Ensure that the correct cell is read from the VE table
	EXPECT_CALL(veTable, getValue(6000, FloatNear(70.9814f, EPS4D)))
		.WillOnce(Return(75.0f));

	MafAirmass dut(veTable);

	auto airmass = dut.getAirmassImpl(200, 6000, false);

	// Check results
	EXPECT_NEAR(0.277777f * 0.75f, airmass.CylinderAirmass, EPS4D);
	EXPECT_NEAR(70.9814f, airmass.EngineLoadPercent, EPS4D);
}

TEST(AirmassModes, VeOverride) {
	StrictMock<MockVp3d> veTable;

	{
		InSequence is;

		// Default
		EXPECT_CALL(veTable, getValue(_, 10.0f)).WillOnce(Return(0));
		// TPS
		EXPECT_CALL(veTable, getValue(_, 30.0f)).WillOnce(Return(0));
	}

	struct DummyAirmassModel : public AirmassVeModelBase {
		DummyAirmassModel(const ValueProvider3D& veTable) : AirmassVeModelBase(veTable) {}

		AirmassResult getAirmass(float rpm, bool postState) override {
			// Default load value 10, will be overriden
			getVe(rpm, 10.0f, postState);

			return {};
		}
	};

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	DummyAirmassModel dut(veTable);

	// Use default mode - will call with 10
	dut.getAirmass(0, true);
	EXPECT_FLOAT_EQ(engine->engineState.veTableYAxis, 10.0f);

	// Override to TPS
	engineConfiguration->veOverrideMode = VE_TPS;
	Sensor::setMockValue(SensorType::Tps1, 30.0f);
	dut.getAirmass(0, true);
	EXPECT_FLOAT_EQ(engine->engineState.veTableYAxis, 30.0f);
}

TEST(AirmassModes, FallbackMap) {
	StrictMock<MockVp3d> veTable;
	StrictMock<MockVp3d> mapFallback;

	// Failed map -> use 75
	{
		InSequence is;

		// Working map -> return 33 (should be unused)
		EXPECT_CALL(mapFallback, getValue(1234, 20)).WillOnce(Return(33));

		// Failed map -> use 75
		EXPECT_CALL(mapFallback, getValue(5678, 20)).WillOnce(Return(75));
	}

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	SpeedDensityAirmass dut(veTable, mapFallback);

	// TPS at 20%
	Sensor::setMockValue(SensorType::Tps1, 20);

	// Working MAP sensor at 40 kPa
	Sensor::setMockValue(SensorType::Map, 40);
	EXPECT_FLOAT_EQ(dut.getMap(1234, false), 40);

	// Failed MAP sensor, should use table
	Sensor::resetMockValue(SensorType::Map);
	EXPECT_FLOAT_EQ(dut.getMap(5678, false), 75);
}

void setInjectionMode(int value);

#if FUEL_RPM_COUNT == 16
TEST(FuelMath, testDifferentInjectionModes) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);

	EXPECT_CALL(*eth.mockAirmass, getAirmass(_, _))
		.WillRepeatedly(Return(AirmassResult{1.3440001f, 50.0f}));

	setInjectionMode((int)IM_BATCH);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ( 20,  engine->engineState.injectionDuration) << "injection while batch";

	setInjectionMode((int)IM_SIMULTANEOUS);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ( 10,  engine->engineState.injectionDuration) << "injection while simultaneous";

	setInjectionMode((int)IM_SEQUENTIAL);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ( 40,  engine->engineState.injectionDuration) << "injection while IM_SEQUENTIAL";

	setInjectionMode((int)IM_SINGLE_POINT);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ( 40,  engine->engineState.injectionDuration) << "injection while IM_SINGLE_POINT";
	EXPECT_EQ( 0u, eth.recentWarnings()->getCount()) << "warningCounter#testDifferentInjectionModes";
}
#endif //FUEL_RPM_COUNT == 16

#if FUEL_RPM_COUNT == 16
TEST(FuelMath, deadtime) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);

	EXPECT_CALL(*eth.mockAirmass, getAirmass(_, _))
		.WillRepeatedly(Return(AirmassResult{1.3440001f, 50.0f}));

	// First test with no deadtime
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ( 20,  engine->engineState.injectionDuration);

	// Now add some deadtime
	setFlatInjectorLag(2.0f);

	// Should have deadtime now!
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ( 20 + 2,  engine->engineState.injectionDuration);
}
#endif //FUEL_RPM_COUNT == 16

#ifndef SUPPRESS_FUEL_MATH_FUEL_TRIM_TEST
#if FUEL_RPM_COUNT == 16
TEST(FuelMath, CylinderFuelTrim) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	EXPECT_CALL(*eth.mockAirmass, getAirmass(_, _))
		.WillRepeatedly(Return(AirmassResult{1, 50.0f}));

	setTable(config->fuelTrims[0].table, -4);
	setTable(config->fuelTrims[1].table, -2);
	setTable(config->fuelTrims[2].table,  2);
	setTable(config->fuelTrims[3].table,  4);

	// run the fuel math
	engine->periodicFastCallback();

	// Check that each cylinder gets the expected amount of fuel
	float unadjusted = 0.072142f;
	EXPECT_NEAR(engine->engineState.injectionMass[0], unadjusted * 0.96, EPS4D);
	EXPECT_NEAR(engine->engineState.injectionMass[1], unadjusted * 0.98, EPS4D);
	EXPECT_NEAR(engine->engineState.injectionMass[2], unadjusted * 1.02, EPS4D);
	EXPECT_NEAR(engine->engineState.injectionMass[3], unadjusted * 1.04, EPS4D);
}
#endif //FUEL_RPM_COUNT == 16
#endif

struct MockIdle : public MockIdleController {
	bool isIdling = false;

	bool isIdlingOrTaper() const override {
		return isIdling;
	}
};

TEST(FuelMath, IdleVeTable) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	MockAirmass dut;

	// Install mock idle controller
	MockIdle idler;
	engine->engineModules.get<IdleController>().set(&idler);

	// Main VE table returns 50
	EXPECT_CALL(dut.veTable, getValue(_, _)).WillRepeatedly(Return(50));

	// Idle VE table returns 40
	setTable(config->idleVeTable, 40);

	// Enable separate idle VE table
	engineConfiguration->useSeparateVeForIdle = true;
	engineConfiguration->idlePidDeactivationTpsThreshold = 10;

	// Set TPS so this works
	Sensor::setMockValue(SensorType::Tps1, 0);

	// Gets normal VE table
	idler.isIdling = false;
	EXPECT_FLOAT_EQ(dut.getVe(1000, 50, false), 0.5f);

	// Gets idle VE table
	idler.isIdling = true;
	EXPECT_FLOAT_EQ(dut.getVe(1000, 50, false), 0.4f);

	// Below half threshold, fully use idle VE table
	Sensor::setMockValue(SensorType::Tps1, 0);
	EXPECT_FLOAT_EQ(dut.getVe(1000, 50, false), 0.4f);
	Sensor::setMockValue(SensorType::Tps1, 2);
	EXPECT_FLOAT_EQ(dut.getVe(1000, 50, false), 0.4f);
	Sensor::setMockValue(SensorType::Tps1, 5);
	EXPECT_FLOAT_EQ(dut.getVe(1000, 50, false), 0.4f);

	// As TPS approaches idle threshold, phase-out the idle VE table

	Sensor::setMockValue(SensorType::Tps1, 6);
	EXPECT_FLOAT_EQ(dut.getVe(1000, 50, false), 0.42f);
	Sensor::setMockValue(SensorType::Tps1, 8);
	EXPECT_FLOAT_EQ(dut.getVe(1000, 50, false), 0.46f);
	Sensor::setMockValue(SensorType::Tps1, 10);
	EXPECT_FLOAT_EQ(dut.getVe(1000, 50, false), 0.5f);
}

TEST(FuelMath, getCycleFuelMassTest) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setLinearCurve(config->crankingTpsCoef, /*from*/1, /*to*/8, 1);
	setTestFuelCrankingTable(4000 * 1.5f);

	Sensor::setMockValue(SensorType::DriverThrottleIntent, 35.0f);

	// test running fuel as crank fuel case
	engineConfiguration->useRunningMathForCranking = true;
	EXPECT_NEAR(getCycleFuelMass(true, 0.05f), 0.171f, EPS3D);

	engineConfiguration->useRunningMathForCranking = false;

	// simulate cranking
	for (size_t i = 0; i < 5; i++) {
		engine->rpmCalculator.onNewEngineCycle();
	}

	EXPECT_NEAR(getCycleFuelMass(true, 0.05f), 20.571f, EPS3D);
	EXPECT_NEAR(engine->engineState.crankingFuel.coolantTemperatureCoefficient, 1, EPS3D);
	EXPECT_NEAR(engine->engineState.crankingFuel.tpsCoefficient, 3.428f, EPS3D);

	for (size_t i = 0; i < 10; i++) {
		engine->rpmCalculator.onNewEngineCycle();
	}

	EXPECT_NEAR(getCycleFuelMass(true, 0.05f), 20.571f, EPS3D);
	EXPECT_NEAR(engine->engineState.crankingFuel.coolantTemperatureCoefficient, 1, EPS3D);
	EXPECT_NEAR(engine->engineState.crankingFuel.tpsCoefficient, 3.428f, EPS3D);

	// simulate TPS error:
	Sensor::setInvalidMockValue(SensorType::DriverThrottleIntent);
	for (size_t i = 0; i < 10; i++) {
		engine->rpmCalculator.onNewEngineCycle();
	}

	EXPECT_NEAR(getCycleFuelMass(true, 0.05f), 6.0f, EPS3D);
	EXPECT_NEAR(engine->engineState.crankingFuel.tpsCoefficient, 1, EPS3D);
}

TEST(FuelMath, postCrankingFactorAxis){
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
	engine->periodicFastCallback();
	setTestFuelCrankingTable(4000 * 1.5f);

	// simulate cranking
	for (size_t i = 0; i < 30; i++) {
		engine->rpmCalculator.onNewEngineCycle();
	}
	setLinearCurve(config->postCrankingCLTBins, /*from*/-20, /*to*/80, 20);
	setLinearCurve(config->postCrankingDurationBins, /*from*/0, /*to*/150, 40);
	setTable(config->postCrankingFactor, 5);

	config->postCrankingFactor[0][0] = 1;
	config->postCrankingFactor[0][1] = 1;

	Sensor::setMockValue(SensorType::Clt, -20);
	engine->periodicFastCallback();

	EXPECT_NEAR(engine->fuelComputer.running.postCrankingFuelCorrection, 1, EPS3D);

	Sensor::setMockValue(SensorType::Clt, 70);
	engine->periodicFastCallback();
	EXPECT_NEAR(engine->fuelComputer.running.postCrankingFuelCorrection, 5, EPS3D);
}


TEST(AirmassModes, PredictiveMapCalculation) {
	StrictMock<MockVp3d> veTable;
	StrictMock<MockVp3d> mapFallback;

	// Configure the mock MAP estimation table to return specific values
	EXPECT_CALL(mapFallback, getValue(1500, 30.0f))
		.WillRepeatedly(Return(85.0f)); // Predicted MAP is 85 kPa

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// Configure engine for predictive MAP mode
	engineConfiguration->accelEnrichmentMode = AE_MODE_PREDICTIVE_MAP;

  // FIXME!  setLinearCurve(config->predictiveMapBlendDurationValues, /*value*/0.5, /*precision*/0.1);
  	for (auto index = 0;index < efi::size(config->predictiveMapBlendDurationValues);index++) {
  	  config->predictiveMapBlendDurationValues[index] = 0.5f;
  	}

	// Create our speed density airmass model
	SpeedDensityAirmass dut(veTable, mapFallback);

	// Setup TPS sensor
	Sensor::setMockValue(SensorType::Tps1, 30.0f);

	// Setup MAP sensor
	Sensor::setMockValue(SensorType::Map, 65.0f);

    auto& tpsAccel = *engine->module<TpsAccelEnrichment>();
	// Without an acceleration event, we should get the actual MAP sensor value
	EXPECT_FLOAT_EQ(dut.getMap(1500, false), 65.0f);

	// Now trigger acceleration event
	tpsAccel.m_accelEventJustOccurred = true;

	// Now the predictive MAP should be used, which is 85 kPa from the map estimation table
	EXPECT_FLOAT_EQ(dut.getMap(1500, true), 85.0f);

	// Subsequent calls should still give predicted until blend starts
	EXPECT_FLOAT_EQ(dut.getMap(1500, false), 85.0f);

	// Test at 25% blend progress (75% predicted, 25% actual)
	// Simulate passing 25% of blend time (125ms)
	eth.moveTimeForwardMs(125); // 25% of 500ms
	// Expected: 85 - (85-65) * 0.25 = 85 - 5 = 80
	EXPECT_NEAR(dut.getMap(1500, false), 80.0f, EPS4D);

	// At 50% blend progress
	eth.moveTimeForwardMs(125); // Now at 250ms
	// Expected: 85 - (85-65) * 0.5 = 85 - 10 = 75
	EXPECT_NEAR(dut.getMap(1500, false), 75.0f, EPS4D);

	// At 75% blend progress
	eth.moveTimeForwardMs(125); // Now at 375ms
	// Expected: 85 - (85-65) * 0.75 = 85 - 15 = 70
	EXPECT_NEAR(dut.getMap(1500, false), 70.0f, EPS4D);

	// Test after blend is complete - should return to sensor value
	// Move time past blend duration (more than 500ms total)
	eth.moveTimeForwardMs(125); // Now at 500ms
	// Should be back to the actual MAP value
	EXPECT_FLOAT_EQ(dut.getMap(1500, false), 65.0f);

	// Test MAP prediction cancellation
	tpsAccel.m_accelEventJustOccurred = true;
	EXPECT_FLOAT_EQ(dut.getMap(1500, false), 85.0f);

	// Now increase the actual MAP to exceed the predicted value
	Sensor::setMockValue(SensorType::Map, 90.0f);

	// Prediction should be canceled, and we should get the actual MAP
	EXPECT_FLOAT_EQ(dut.getMap(1500, false), 90.0f);

	// 5. Test with failed MAP sensor
	Sensor::resetMockValue(SensorType::Map);

	// Should use the fallback MAP from the table
	EXPECT_FLOAT_EQ(dut.getMap(1500, false), 85.0f);
}
