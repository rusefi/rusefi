/*
 * @file	test_engine_math.c
 *
 * @date Nov 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "speed_density.h"
#include "maf.h"

TEST(misc, testIgnitionPlanning) {
	printf("*************************************************** testIgnitionPlanning\r\n");
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setTable(config->lambdaTable, 0.92f);
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);

	engine->periodicFastCallback();
	ASSERT_NEAR(12.857, engine->fuelComputer.targetAFR, EPS4D) << "testIgnitionPlanning_AFR";

	ASSERT_EQ(IM_BATCH, engineConfiguration->injectionMode);
}

TEST(misc, testEngineMath) {
	printf("*************************************************** testEngineMath\r\n");

	// todo: let's see if we can make 'engine' unneeded in this test?
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setTable(config->lambdaTable, 0.92f);
	setTable(config->veTable, 80);

    setCamOperationMode();
	engineConfiguration->fuelAlgorithm = engine_load_mode_e::LM_SPEED_DENSITY;

	ASSERT_NEAR( 50,  getOneDegreeTimeMs(600) * 180, EPS4D) << "600 RPM";
	ASSERT_EQ( 5,  getOneDegreeTimeMs(6000) * 180) << "6000 RPM";

	auto fuelComputer = &engine->fuelComputer;

	Sensor::setMockValue(SensorType::Clt, 300);
	Sensor::setMockValue(SensorType::Iat, 350);
	ASSERT_FLOAT_EQ(312.5, fuelComputer->getTCharge(1000, 0));
	ASSERT_FLOAT_EQ(313.5833, fuelComputer->getTCharge(1000, 50));
	ASSERT_FLOAT_EQ(314.6667, fuelComputer->getTCharge(1000, 100));


	ASSERT_FLOAT_EQ(312.5, fuelComputer->getTCharge(4000, 0));
	ASSERT_FLOAT_EQ(320.0833, fuelComputer->getTCharge(4000, 50));
	ASSERT_FLOAT_EQ(327.6667, fuelComputer->getTCharge(4000, 100));

	// test Air Interpolation mode
	engineConfiguration->tChargeMode = TCHARGE_MODE_AIR_INTERP;
	engineConfiguration->tChargeAirCoefMin = 0.098f;
	engineConfiguration->tChargeAirCoefMax = 0.902f;
	engineConfiguration->tChargeAirFlowMax = 153.6f;
	// calc. some airMass given the engine displacement=1.839 and 4 cylinders (FORD_ESCORT_GT)
	fuelComputer->sdAirMassInOneCylinder = SpeedDensityBase::getAirmassImpl(/*VE*/1.0f, /*MAP*/100.0f, /*tChargeK*/273.15f + 20.0f);
	ASSERT_NEAR(0.59418f, fuelComputer->sdAirMassInOneCylinder, EPS4D);

	Sensor::setMockValue(SensorType::Clt, 90);
	Sensor::setMockValue(SensorType::Iat, 20);
	Sensor::setMockValue(SensorType::Map, 100);
	Sensor::setMockValue(SensorType::Tps1, 0);
	Sensor::setMockValue(SensorType::Rpm, 1000);

	// calc. airFlow using airMass, and find tCharge
	engine->periodicFastCallback();
	ASSERT_NEAR(57.0099f, engine->engineState.sd.tCharge, EPS3D);
	ASSERT_NEAR(50.6476f/*kg/h*/, engine->engineState.airflowEstimate, EPS4D);
}

class ChargeTemperatureStartup : public testing::Test {
protected:
	EngineTestHelper eth{engine_type_e::TEST_ENGINE};

	void SetUp() override {
		engineConfiguration->fuelAlgorithm = engine_load_mode_e::LM_SPEED_DENSITY;
		setTable(config->lambdaTable, 1.0f);
		setTable(config->veTable, 80);
		engineConfiguration->tChargeMode = TCHARGE_MODE_RPM_TPS;
		engineConfiguration->tChargeMinRpmMinTps = 0.25f;
		engineConfiguration->tChargeMaxRpmMinTps = 0.25f;
		engineConfiguration->tChargeAirIncrLimit = 1.0f;
		engineConfiguration->tChargeAirDecrLimit = 12.5f;
		Sensor::setMockValue(SensorType::Rpm, 1000);
		Sensor::setMockValue(SensorType::Tps1, 0);
		Sensor::setMockValue(SensorType::Map, 100);
		Sensor::setMockValue(SensorType::Clt, 90);
		Sensor::setMockValue(SensorType::Iat, 30);
	}
};

TEST_F(ChargeTemperatureStartup, ValidSensorsOnFirstUpdateBypassRateLimit) {
	ASSERT_FLOAT_EQ(0, engine->engineState.sd.tChargeK);
	ASSERT_FLOAT_EQ(75, engine->fuelComputer.getTCharge(1000, 0)); // 90 + 0.25 * (30 - 90)

	eth.moveTimeForwardMs(100);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ(75, engine->engineState.sd.tCharge);
	EXPECT_FLOAT_EQ(348.15f, engine->engineState.sd.tChargeK);

	// After initialization, ordinary temperature changes still use the rate limiter.
	Sensor::setMockValue(SensorType::Clt, 100);
	eth.moveTimeForwardSec(1);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ(76, engine->engineState.sd.tCharge);
	EXPECT_FLOAT_EQ(349.15f, engine->engineState.sd.tChargeK);
}

TEST_F(ChargeTemperatureStartup, MissingInitialReadingsInitializeFromHotSensors) {
	// [tag:charge_temperature_startup] Regression for https://github.com/rusefi/rusefi/issues/10400.
	// The fallback before the first ADC readings must not consume initialization.
	Sensor::setInvalidMockValue(SensorType::Clt);
	Sensor::setInvalidMockValue(SensorType::Iat);
	ASSERT_FLOAT_EQ(0, engine->engineState.sd.tChargeK);
	engine->periodicFastCallback();
	ASSERT_FLOAT_EQ(0, engine->engineState.sd.tCharge);
	ASSERT_FLOAT_EQ(273.15f, engine->engineState.sd.tChargeK);

	// First real readings must immediately replace the 0 C fallback with 75 C.
	Sensor::setMockValue(SensorType::Clt, 90);
	Sensor::setMockValue(SensorType::Iat, 30);
	ASSERT_FLOAT_EQ(75, engine->fuelComputer.getTCharge(1000, 0));
	eth.moveTimeForwardMs(100);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ(75, engine->engineState.sd.tCharge);
	EXPECT_FLOAT_EQ(348.15f, engine->engineState.sd.tChargeK);

	eth.moveTimeForwardSec(1);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ(75, engine->engineState.sd.tCharge);
	EXPECT_FLOAT_EQ(348.15f, engine->engineState.sd.tChargeK);

	// Speed density must use the hot estimate immediately, without excess air mass.
	const float hotAirmass = SpeedDensityBase::getAirmassImpl(0.8f, 100, 348.15f);
	const float actualAirmass = engine->fuelComputer.sdAirMassInOneCylinder;
	ASSERT_TRUE(std::isfinite(actualAirmass));
	ASSERT_GT(hotAirmass, 0);
	EXPECT_NEAR(hotAirmass, actualAirmass, EPS4D);
}

TEST_F(ChargeTemperatureStartup, CoolantArrivesBeforeIntakeTemperature) {
	Sensor::setInvalidMockValue(SensorType::Iat);
	engine->periodicFastCallback();
	ASSERT_FLOAT_EQ(67.5f, engine->engineState.sd.tCharge); // IAT fallback is 0 C.

	// An installation with only CLT must still rate-limit subsequent changes.
	Sensor::setMockValue(SensorType::Clt, 100);
	eth.moveTimeForwardSec(1);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ(68.5f, engine->engineState.sd.tCharge);

	Sensor::setMockValue(SensorType::Iat, 30);
	eth.moveTimeForwardMs(100);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ(82.5f, engine->engineState.sd.tCharge);
	EXPECT_FLOAT_EQ(355.65f, engine->engineState.sd.tChargeK);
}

TEST_F(ChargeTemperatureStartup, IntakeTemperatureArrivesBeforeCoolant) {
	Sensor::setInvalidMockValue(SensorType::Clt);
	engine->periodicFastCallback();
	ASSERT_FLOAT_EQ(30, engine->engineState.sd.tCharge); // Only IAT is available.

	Sensor::setMockValue(SensorType::Iat, 40);
	eth.moveTimeForwardSec(1);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ(31, engine->engineState.sd.tCharge);

	Sensor::setMockValue(SensorType::Clt, 90);
	eth.moveTimeForwardMs(100);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ(77.5f, engine->engineState.sd.tCharge);
	EXPECT_FLOAT_EQ(350.65f, engine->engineState.sd.tChargeK);
}

TEST_F(ChargeTemperatureStartup, ValidZeroDegreesStillInitializesRateLimiter) {
	Sensor::setMockValue(SensorType::Clt, 0);
	Sensor::setMockValue(SensorType::Iat, 0);
	engine->periodicFastCallback();
	ASSERT_FLOAT_EQ(0, engine->engineState.sd.tCharge);
	ASSERT_FLOAT_EQ(273.15f, engine->engineState.sd.tChargeK);

	Sensor::setMockValue(SensorType::Clt, 90);
	Sensor::setMockValue(SensorType::Iat, 30);
	eth.moveTimeForwardSec(1);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ(1, engine->engineState.sd.tCharge);
	EXPECT_FLOAT_EQ(274.15f, engine->engineState.sd.tChargeK);
}

TEST_F(ChargeTemperatureStartup, LaterSensorLossAndRecoveryKeepRateLimits) {
	engine->periodicFastCallback();
	ASSERT_FLOAT_EQ(75, engine->engineState.sd.tCharge);

	Sensor::setInvalidMockValue(SensorType::Clt);
	Sensor::setInvalidMockValue(SensorType::Iat);
	eth.moveTimeForwardSec(1);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ(62.5f, engine->engineState.sd.tCharge);

	Sensor::setMockValue(SensorType::Clt, 90);
	Sensor::setMockValue(SensorType::Iat, 30);
	eth.moveTimeForwardSec(1);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ(63.5f, engine->engineState.sd.tCharge);
	EXPECT_FLOAT_EQ(336.65f, engine->engineState.sd.tChargeK);
}

TEST(misc, testIgnitionMapGenerator) {
	float rpmBin[16];
	setRpmBin(rpmBin, 16, 800, 7000);
	ASSERT_EQ(650, rpmBin[0]);
	ASSERT_EQ( 800,  rpmBin[1]) << "@1";
	ASSERT_EQ( 1100,  rpmBin[2]) << "@2";
	ASSERT_EQ( 1400,  rpmBin[3]) << "rpm@3";
	ASSERT_EQ( 4700,  rpmBin[14]) << "rpm@14";
	ASSERT_EQ(7000, rpmBin[15]);

	ASSERT_NEAR(36.0, getInitialAdvance(6000, 100, 36), 0.1);
	ASSERT_NEAR(9.9, getInitialAdvance(600, 100, 36), 0.2);

	ASSERT_NEAR(34.2, getInitialAdvance(2400, 40, 36), 0.1);
	ASSERT_NEAR(41.9, getInitialAdvance(4400, 40, 36), 0.2);
	ASSERT_NEAR(14.2, getInitialAdvance(800, 20, 36), 0.2);
}
