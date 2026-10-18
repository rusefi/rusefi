#include "pch.h"
#include "fuel_math.h"
#include "alphan_airmass.h"
#include "maf_airmass.h"
#include "speed_density_airmass.h"
#include "util/injection_crank_helper.h"
#include <stdexcept>

using ::testing::StrictMock;
using ::testing::FloatNear;
using ::testing::InSequence;
using ::testing::_;

TEST(FuelMath, InvalidAccelEnrichmentModeMessage) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
	EXPECT_CALL(*eth.mockAirmass, getAirmass(_, _))
		.WillRepeatedly(Return(AirmassResult{1.3440001f, 50.0f}));
	engine->rpmCalculator.setRpmValue(1200);
	engineConfiguration->accelEnrichmentMode = static_cast<accel_enrichment_mode_e>(182);

	// [tag:user_facing_ini_labels] #10363: the diagnostic must identify the setting
	// by its tunerstudio.template.ini field label and include the invalid value.
	try {
		getInjectionMass(1200);
		FAIL() << "Invalid acceleration enrichment mode must raise a firmware error";
	} catch (const std::logic_error& error) {
		EXPECT_STREQ("Invalid Accel Enrichment Mode: 182", error.what());
	}
}

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

TEST(AirmassModes, SpeedDensityCompensatedMap) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->displacement = 2.0f;
	engineConfiguration->cylindersCount = 4;

	StrictMock<MockVp3d> veTable;
	StrictMock<MockVp3d> mapFallback;

	{
		InSequence is;

		// Feature disabled: VE is looked up at actual MAP
		EXPECT_CALL(veTable, getValue(1200, FloatNear(80.0f, EPS4D))).WillOnce(Return(50.0f));
		// Enabled with baro at 80% of standard atmosphere: VE is looked up at MAP_ref = 80 / 0.8 = 100
		EXPECT_CALL(veTable, getValue(1200, FloatNear(100.0f, EPS4D))).WillOnce(Return(50.0f));
		// Enabled but no baro sensor: no compensation
		EXPECT_CALL(veTable, getValue(1200, FloatNear(80.0f, EPS4D))).WillOnce(Return(50.0f));
	}

	SpeedDensityAirmass dut(veTable, mapFallback);

	float tChargeK = 273.15f + 20.0f;
	engine->engineState.sd.tChargeK = tChargeK;

	float map = 80;
	mass_t expectedAirmass = SpeedDensityBase::getAirmassImpl(0.5f, map, tChargeK);

	// Feature disabled: load axis is actual MAP
	auto result = dut.getAirmass(1200, map, false);
	EXPECT_NEAR(result.CylinderAirmass, expectedAirmass, EPS4D);
	EXPECT_NEAR(result.EngineLoadPercent, 80.0f, EPS4D);

	// Enable with a baro sensor at 80% of standard atmosphere
	engineConfiguration->useCompensatedMap = true;
	Sensor::setMockValue(SensorType::BarometricPressure, 0.8f * STD_ATMOSPHERE);

	result = dut.getAirmass(1200, map, false);
	// Physical air mass calculation still uses actual MAP
	EXPECT_NEAR(result.CylinderAirmass, expectedAirmass, EPS4D);
	// Load axis is normalized to standard atmosphere
	EXPECT_NEAR(result.EngineLoadPercent, 100.0f, EPS4D);

	// Enabled but baro sensor missing: no compensation applied
	Sensor::resetMockValue(SensorType::BarometricPressure);
	result = dut.getAirmass(1200, map, false);
	EXPECT_NEAR(result.CylinderAirmass, expectedAirmass, EPS4D);
	EXPECT_NEAR(result.EngineLoadPercent, 80.0f, EPS4D);
}

// Exercise both paths reported in https://github.com/rusefi/rusefi/issues/10401.
// Compensated MAP selects table cells; baroCorrTable independently multiplies
// running fuel. Their coexistence must not apply MAP/baro to physical air mass.
class BarometricCompensation : public testing::Test {
protected:
	EngineTestHelper eth{engine_type_e::TEST_ENGINE};

	void SetUp() override {
		engineConfiguration->fuelAlgorithm = engine_load_mode_e::LM_SPEED_DENSITY;
		engineConfiguration->displacement = 2.0f;
		engineConfiguration->cylindersCount = 4;
		engineConfiguration->injectionMode = IM_SEQUENTIAL;
		engineConfiguration->stoichRatioPrimary = 14.7f;
		engineConfiguration->veOverrideMode = VE_None;
		engineConfiguration->afrOverrideMode = AFR_None;
		engineConfiguration->coastingFuelCutEnabled = false;
		setTable(config->veTable, 80);
		setTable(config->lambdaTable, 1.0f);
		setArrayValues(config->cltFuelCorr, 1.0f);
		setArrayValues(config->iatFuelCorr, 1.0f);
		setTable(config->postCrankingFactor, 1.0f);
		Sensor::setMockValue(SensorType::Clt, 20);
		Sensor::setMockValue(SensorType::Iat, 20);
		Sensor::setMockValue(SensorType::Tps1, 0);
		engine->rpmCalculator.setRpmValue(2000);
	}

	void checkFuel(float map, float baro, float load, float ve, float correction) {
		Sensor::setMockValue(SensorType::Map, map);
		Sensor::setMockValue(SensorType::BarometricPressure, baro);
		engine->periodicFastCallback();

		const float expectedAir = SpeedDensityBase::getAirmassImpl(ve / 100, map, 293.15f);
		const float actualFuel = engine->engineState.injectionMass[0];
		ASSERT_TRUE(std::isfinite(actualFuel));
		ASSERT_GT(actualFuel, 0);
		EXPECT_NEAR(load, engine->engineState.veTableYAxis, EPS3D);
		EXPECT_NEAR(load, engine->engineState.fuelingLoad, EPS3D);
		EXPECT_NEAR(load, engine->fuelComputer.afrTableYAxis, EPS3D);
		EXPECT_NEAR(ve, engine->engineState.currentVe, EPS3D);
		EXPECT_NEAR(expectedAir, engine->fuelComputer.sdAirMassInOneCylinder, EPS4D);
		EXPECT_NEAR(correction, engine->engineState.baroCorrection, EPS4D);
		EXPECT_NEAR(correction, engine->fuelComputer.totalFuelCorrection, EPS4D);
		EXPECT_NEAR(expectedAir / 14.7f * correction, actualFuel, 1e-6f);
	}
};

TEST_F(BarometricCompensation, DefaultTableDoesNotDuplicateAltitudeCorrection) {
	// Keep the default neutral baro table and a flat VE table to isolate pressure.
	engineConfiguration->useCompensatedMap = false;
	checkFuel(100, STD_ATMOSPHERE, 100, 80, 1);
	const float seaLevelFuel = engine->engineState.injectionMass[0];

	checkFuel(80, 0.8f * STD_ATMOSPHERE, 80, 80, 1);
	EXPECT_NEAR(0.8f * seaLevelFuel, engine->engineState.injectionMass[0], 1e-6f);

	// At altitude the load returns to 100, but actual MAP still reduces fuel
	// to 80% of the sea-level value. There is no second altitude multiplier.
	engineConfiguration->useCompensatedMap = true;
	checkFuel(80, 0.8f * STD_ATMOSPHERE, 100, 80, 1);
	EXPECT_NEAR(0.8f * seaLevelFuel, engine->engineState.injectionMass[0], 1e-6f);
}

TEST_F(BarometricCompensation, LoadAxisAndFuelMultiplierRemainIndependent) {
	// Deliberately vary VE with load: 80% at load 80, 100% at load 100.
	setLinearCurve(config->veLoadBins, 20, 140);
	for (size_t row = 0; row < efi::size(config->veTable); row++) {
		setArrayValues(config->veTable[row], config->veLoadBins[row]);
	}

	engineConfiguration->useCompensatedMap = false;
	checkFuel(80, 0.8f * STD_ATMOSPHERE, 80, 80, 1);
	const float baselineFuel = engine->engineState.injectionMass[0];

	engineConfiguration->useCompensatedMap = true;
	checkFuel(80, 0.8f * STD_ATMOSPHERE, 100, 100, 1);
	EXPECT_NEAR(1.25f * baselineFuel, engine->engineState.injectionMass[0], 1e-6f);

	// A separately calibrated baro table: 1.0 at sea level, 1.1 at 80% baro.
	for (size_t row = 0; row < efi::size(config->baroCorrTable); row++) {
		const float correction = 1.0f + (STD_ATMOSPHERE - config->baroCorrPressureBins[row]) / (2 * STD_ATMOSPHERE);
		setArrayValues(config->baroCorrTable[row], correction);
	}

	// Both effects can be configured to add fuel, but each applies exactly once:
	// 25% from the chosen VE cells and 10% from the explicit baro multiplier.
	checkFuel(80, 0.8f * STD_ATMOSPHERE, 100, 100, 1.1f);
	EXPECT_NEAR(1.25f * 1.1f * baselineFuel, engine->engineState.injectionMass[0], 1e-6f);

	engineConfiguration->useCompensatedMap = false;
	checkFuel(80, 0.8f * STD_ATMOSPHERE, 80, 80, 1.1f);
	EXPECT_NEAR(1.1f * baselineFuel, engine->engineState.injectionMass[0], 1e-6f);
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
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);

	// Gets normal VE table
	idler.isIdling = false;
	EXPECT_FLOAT_EQ(dut.getVe(1000, 50, false), 0.5f);

	// Gets idle VE table
	idler.isIdling = true;
	EXPECT_FLOAT_EQ(dut.getVe(1000, 50, false), 0.4f);

	// Below half threshold, fully use idle VE table
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);
	EXPECT_FLOAT_EQ(dut.getVe(1000, 50, false), 0.4f);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 2);
	EXPECT_FLOAT_EQ(dut.getVe(1000, 50, false), 0.4f);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 5);
	EXPECT_FLOAT_EQ(dut.getVe(1000, 50, false), 0.4f);

	// As TPS approaches idle threshold, phase-out the idle VE table

	Sensor::setMockValue(SensorType::DriverThrottleIntent, 6);
	EXPECT_FLOAT_EQ(dut.getVe(1000, 50, false), 0.42f);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 8);
	EXPECT_FLOAT_EQ(dut.getVe(1000, 50, false), 0.46f);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 10);
	EXPECT_FLOAT_EQ(dut.getVe(1000, 50, false), 0.5f);
}

TEST(FuelMath, VeSwitchTable) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	MockAirmass dut;

	// Primary VE table returns 50%
	EXPECT_CALL(dut.veTable, getValue(_, _)).WillRepeatedly(Return(50));

	// Second VE table returns 80%
	setTable(secondTablesGetState()->secondVeTable, 80);

	secondTablesGetState()->secondVeTableInput = Gpio::A0;

	// Pin HIGH -> second table
	setMockState(Gpio::A0, true);
	EXPECT_NEAR(dut.getVe(1000, 50, false), 0.8f, EPS4D);

	// Pin LOW -> primary table
	setMockState(Gpio::A0, false);
	EXPECT_NEAR(dut.getVe(1000, 50, false), 0.5f, EPS4D);

	// No pin configured -> primary table
	secondTablesGetState()->secondVeTableInput = Gpio::Unassigned;
	EXPECT_NEAR(dut.getVe(1000, 50, false), 0.5f, EPS4D);
}

TEST(FuelMath, VeSwitchTableBlend) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	MockAirmass dut;

	// Primary VE table returns 50%, switch table returns 80%
	EXPECT_CALL(dut.veTable, getValue(_, _)).WillRepeatedly(Return(50));
	setTable(secondTablesGetState()->secondVeTable, 80);

	secondTablesGetState()->secondVeTableInput = Gpio::Unassigned; // no pin, blend controls it

	// Configure blend: TPS controls blend, 0% TPS -> 0% blend, 100% TPS -> 100% blend
	secondTablesGetState()->secondVeBlendParameter = GPPWM_Tps;
	setLinearCurve(secondTablesGetState()->secondVeBlendBins, 0, 100, 1);
	setLinearCurve(secondTablesGetState()->secondVeBlendValues, 0, 100, 1);

	// TPS = 0 -> 0% blend -> primary table (50%)
	Sensor::setMockValue(SensorType::Tps1, 0);
	EXPECT_NEAR(dut.getVe(1000, 50, false), 0.5f, EPS4D);

	// TPS = 50 -> 50% blend -> midpoint between 50% and 80% = 65%
	Sensor::setMockValue(SensorType::Tps1, 50);
	EXPECT_NEAR(dut.getVe(1000, 50, true), 0.65f, EPS4D);
	EXPECT_FLOAT_EQ(65.0f, engine->engineState.currentVe);

	// TPS = 100 -> 100% blend -> fully switched (80%)
	Sensor::setMockValue(SensorType::Tps1, 100);
	EXPECT_NEAR(dut.getVe(1000, 50, false), 0.8f, EPS4D);
	// Calculations with postState=false must not replace the published value.
	EXPECT_FLOAT_EQ(65.0f, engine->engineState.currentVe);

	// Pin takes priority over blend when active
	secondTablesGetState()->secondVeTableInput = Gpio::A0;
	setMockState(Gpio::A0, true);
	Sensor::setMockValue(SensorType::Tps1, 0); // blend would give primary, but pin forces switch
	EXPECT_NEAR(dut.getVe(1000, 50, false), 0.8f, EPS4D);
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

// flexCranking selects the cranking coolant-multiplier source: the 1D crankingFuelCoef curve when off
// (or when no flex sensor is present), and the 2D crankingFuelFlexTable when on with a flex sensor.
TEST(FuelMath, crankingFlexFallbackToCurve) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// Pass the base mass straight through so the return value equals the coolant multiplier.
	engineConfiguration->useRunningMathForCranking = true;

	setArrayValues(config->crankingFuelCoef, 2.0f);  // flat 1D curve
	Sensor::setMockValue(SensorType::Clt, 20);

	// flex off -> 1D curve, regardless of any ethanol reading.
	engineConfiguration->flexCranking = false;
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 85);
	EXPECT_NEAR(2.0f, getCrankingFuel3(1, 0), EPS4D);

	// flex on but no flex sensor present -> still the 1D curve.
	engineConfiguration->flexCranking = true;
	Sensor::resetMockValue(SensorType::FuelEthanolPercent);
	EXPECT_NEAR(2.0f, getCrankingFuel3(1, 0), EPS4D);
}

// flexCranking on + flex sensor present resolves the cranking coolant multiplier from the 2D ethanol table.
TEST(FuelMath, crankingFlex2dTable) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->useRunningMathForCranking = true;
	engineConfiguration->flexCranking = true;

	Sensor::setMockValue(SensorType::Clt, 20);

	// Ethanol axis plus a table that is flat across coolant so only the ethanol axis matters.
	const uint8_t ethanolBins[] = { 0, 35, 65, 100 };
	copyArray(config->crankingFuelFlexBins, ethanolBins);

	const float rowMult[] = { 2.0f, 2.5f, 3.5f, 5.0f };
	for (size_t row = 0; row < efi::size(config->crankingFuelFlexTable); row++) {
		for (size_t col = 0; col < CRANKING_CURVE_SIZE; col++) {
			config->crankingFuelFlexTable[row][col] = rowMult[row];
		}
	}

	// Exact ethanol bins hit their row.
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 0);
	EXPECT_NEAR(2.0f, getCrankingFuel3(1, 0), EPS4D);
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 35);
	EXPECT_NEAR(2.5f, getCrankingFuel3(1, 0), EPS4D);
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 100);
	EXPECT_NEAR(5.0f, getCrankingFuel3(1, 0), EPS4D);

	// Between bins -> linear interpolation on the ethanol axis (35 -> 65 at 50%).
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 50);
	EXPECT_NEAR(3.0f, getCrankingFuel3(1, 0), EPS4D);
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

// Verifies that the blend target tracks the rising sensor value rather than
// the stale pre-event snapshot, avoiding a lean dip during long transients.
TEST(AirmassModes, PredictiveMapBlendTowardRisingSensor) {
	StrictMock<MockVp3d> veTable;
	StrictMock<MockVp3d> mapFallback;

	EXPECT_CALL(mapFallback, getValue(1500, 30.0f))
		.WillRepeatedly(Return(85.0f));

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->accelEnrichmentMode = AE_MODE_PREDICTIVE_MAP;
	for (auto index = 0; index < efi::size(config->predictiveMapBlendDurationValues); index++) {
		config->predictiveMapBlendDurationValues[index] = 0.5f; // 500ms blend
	}

	SpeedDensityAirmass dut(veTable, mapFallback);

	Sensor::setMockValue(SensorType::Tps1, 30.0f);
	Sensor::setMockValue(SensorType::Map, 65.0f);

	// Trigger prediction: predicted=85, sensor=65
	auto& tpsAccel = *engine->module<TpsAccelEnrichment>();
	tpsAccel.m_accelEventJustOccurred = true;
	EXPECT_FLOAT_EQ(dut.getMap(1500, false), 85.0f);

	// Sensor rises to 75 kPa (manifold filling up)
	Sensor::setMockValue(SensorType::Map, 75.0f);

	// At 50% blend with rising sensor: 85 + (75-85)*0.5 = 80.0
	// Old code would give: 85 + (65-85)*0.5 = 75.0
	eth.moveTimeForwardMs(250);
	EXPECT_NEAR(dut.getMap(1500, false), 80.0f, EPS4D);

	// Sensor catches up to predicted — prediction exits early
	Sensor::setMockValue(SensorType::Map, 90.0f);
	EXPECT_FLOAT_EQ(dut.getMap(1500, false), 90.0f);
}

// [tag:predictive_map_lifetime] Blend restarts must not extend the event's lifetime.
TEST(AirmassModes, PredictiveMapRetriggersRespectSessionDuration) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->accelEnrichmentMode = AE_MODE_PREDICTIVE_MAP;
	engineConfiguration->tpsAccelEnrichmentThreshold = 5;
	for (auto& duration : config->predictiveMapBlendDurationValues) {
		duration = 0.5f;
	}

	StrictMock<MockVp3d> veTable;
	StrictMock<MockVp3d> mapFallback;
	EXPECT_CALL(mapFallback, getValue(1500, 30)).WillRepeatedly(Return(85));
	EXPECT_CALL(mapFallback, getValue(1500, 40)).WillRepeatedly(Return(95));
	EXPECT_CALL(mapFallback, getValue(1500, 50)).WillRepeatedly(Return(105));
	SpeedDensityAirmass dut(veTable, mapFallback);
	Sensor::setMockValue(SensorType::Map, 65);
	Sensor::setMockValue(SensorType::Tps1, 30);
	auto& ae = *engine->module<TpsAccelEnrichment>();
	ae.isAboveAccelThreshold = true;
	ae.m_accelEventJustOccurred = true;
	ASSERT_FLOAT_EQ(dut.getMap(1500, true), 85);
	ASSERT_TRUE(engine->outputChannels.isMapPredictionActive);

	eth.moveTimeForwardMs(200);
	Sensor::setMockValue(SensorType::Tps1, 40);
	ae.m_accelEventJustOccurred = true;
	EXPECT_FLOAT_EQ(dut.getMap(1500, true), 95);
	eth.moveTimeForwardMs(200);
	Sensor::setMockValue(SensorType::Tps1, 50);
	ae.m_accelEventJustOccurred = true;
	EXPECT_FLOAT_EQ(dut.getMap(1500, true), 105);

	eth.moveTimeForwardMs(99);
	EXPECT_NEAR(dut.getMap(1500, true), 97.08f, EPS4D);
	ASSERT_TRUE(engine->outputChannels.isMapPredictionActive);
	eth.moveTimeForwardMs(1);
	// The session ends at 500 ms, despite the last blend restart at 400 ms.
	EXPECT_FLOAT_EQ(dut.getMap(1500, true), 65);
	EXPECT_FALSE(engine->outputChannels.isMapPredictionActive);
	EXPECT_EQ(engine->outputChannels.mapPredEventOver, 1);
	EXPECT_EQ(engine->outputChannels.predTimerResetCnt, 1);

	// Pending acceleration cannot immediately restart a capped event.
	ae.m_accelEventJustOccurred = true;
	eth.moveTimeForwardMs(100);
	EXPECT_FLOAT_EQ(dut.getMap(1500, true), 65);
	EXPECT_EQ(engine->outputChannels.mapPredEventOver, 1);
	EXPECT_EQ(engine->outputChannels.predTimerResetCnt, 1);

	// Clearing the detector threshold permits a new event with its own timer.
	ae.m_accelEventJustOccurred = false;
	ae.isAboveAccelThreshold = false;
	EXPECT_FLOAT_EQ(dut.getMap(1500, true), 65);
	ae.isAboveAccelThreshold = true;
	ae.m_accelEventJustOccurred = true;
	EXPECT_FLOAT_EQ(dut.getMap(1500, true), 105);
	eth.moveTimeForwardMs(250);
	EXPECT_FLOAT_EQ(dut.getMap(1500, true), 85);
	EXPECT_TRUE(engine->outputChannels.isMapPredictionActive);
	EXPECT_EQ(engine->outputChannels.predTimerResetCnt, 2);
}

TEST(AirmassModes, PredictiveMapSmallerEventHasItsOwnTpsPeak) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->accelEnrichmentMode = AE_MODE_PREDICTIVE_MAP;
	engineConfiguration->tpsAccelEnrichmentThreshold = 5;
	for (auto& duration : config->predictiveMapBlendDurationValues) {
		duration = 0.5f;
	}

	StrictMock<MockVp3d> veTable;
	StrictMock<MockVp3d> mapFallback;
	EXPECT_CALL(mapFallback, getValue(1500, 80)).WillRepeatedly(Return(100));
	EXPECT_CALL(mapFallback, getValue(1500, 0)).WillRepeatedly(Return(40));
	EXPECT_CALL(mapFallback, getValue(1500, 30)).WillRepeatedly(Return(70));
	EXPECT_CALL(mapFallback, getValue(1500, 20)).WillRepeatedly(Return(60));
	SpeedDensityAirmass dut(veTable, mapFallback);
	Sensor::setMockValue(SensorType::Map, 40);
	Sensor::setMockValue(SensorType::Tps1, 80);
	auto& ae = *engine->module<TpsAccelEnrichment>();
	ae.isAboveAccelThreshold = true;
	ae.m_accelEventJustOccurred = true;
	ASSERT_FLOAT_EQ(dut.getMap(1500, true), 100);
	ASSERT_FLOAT_EQ(dut.getMap(1500, true), 100); // Record the first event's peak.

	eth.moveTimeForwardMs(100);
	Sensor::setMockValue(SensorType::Tps1, 0);
	ae.isAboveAccelThreshold = false;
	ASSERT_FLOAT_EQ(dut.getMap(1500, true), 40);
	ASSERT_FALSE(engine->outputChannels.isMapPredictionActive);

	eth.moveTimeForwardMs(500);
	Sensor::setMockValue(SensorType::Tps1, 30);
	ae.isAboveAccelThreshold = true;
	ae.m_accelEventJustOccurred = true;
	ASSERT_FLOAT_EQ(dut.getMap(1500, true), 70);
	ASSERT_TRUE(engine->outputChannels.isMapPredictionActive);
	// The new event retains its 30% peak instead of inheriting the old 80% peak.
	EXPECT_FLOAT_EQ(dut.getMap(1500, true), 70);
	EXPECT_TRUE(engine->outputChannels.isMapPredictionActive);
	eth.moveTimeForwardMs(250);
	EXPECT_FLOAT_EQ(dut.getMap(1500, true), 55);

	// A real throttle release still cancels prediction before MAP catches up.
	Sensor::setMockValue(SensorType::Tps1, 20);
	EXPECT_FLOAT_EQ(dut.getMap(1500, true), 40);
	EXPECT_FALSE(engine->outputChannels.isMapPredictionActive);
}
