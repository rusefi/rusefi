#include "pch.h"

#include "boost_control.h"

using ::testing::_;
using ::testing::StrictMock;

static Map2D<BOOST_CURVE_SIZE, float, float> testBoostCltCorr { "clt" };
static Map2D<BOOST_CURVE_SIZE, float, float> testBoostIatCorr { "iat" };
static Map2D<BOOST_CURVE_SIZE, float, float> testBoostCltAdder { "clt (adder)" };
static Map2D<BOOST_CURVE_SIZE, float, float> testBoostIatAdder { "iat (adder)" };

TEST(BoostControl, Setpoint) {
	MockVp3d targetMap;

	// Just pass TPS input to output
	EXPECT_CALL(targetMap, getValue(_, _))
		.WillRepeatedly([](float xRpm, float tps) { return tps; });

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->boostType = CLOSED_LOOP;

	BoostController bc;

	// Should return unexpected without a pedal map cfg'd
	EXPECT_EQ(bc.getSetpoint(), unexpected);

    testBoostCltCorr.initTable(config->cltBoostCorr, config->cltBoostCorrBins);
    testBoostIatCorr.initTable(config->iatBoostCorr, config->iatBoostCorrBins);
    testBoostCltAdder.initTable(config->cltBoostAdder, config->cltBoostAdderBins);
    testBoostIatAdder.initTable(config->iatBoostAdder, config->iatBoostAdderBins);

    // Now init with mock target map
	bc.init(nullptr, nullptr, &targetMap, testBoostCltCorr, testBoostIatCorr, testBoostCltAdder, testBoostIatAdder, nullptr);

	// Should still return unxepected since TPS is invalid
	EXPECT_EQ(bc.getSetpoint(), unexpected);

	// Configure TPS, should get passthru of tps value
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 35.0f);
	EXPECT_FLOAT_EQ(bc.getSetpoint().value_or(-1), 35.0f);
}

TEST(BoostControl, ObservePlant) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->boostType = CLOSED_LOOP;

	BoostController bc;

	Sensor::resetMockValue(SensorType::Map);
	// Check that invalid MAP returns unexpected
	EXPECT_EQ(bc.observePlant(), unexpected);

	// Test valid MAP value
	Sensor::setMockValue(SensorType::Map, 150);

	EXPECT_FLOAT_EQ(bc.observePlant().value_or(0), 150.0f);
}

TEST(BoostControl, OpenLoop) {
	MockVp3d openMap;

	// Just pass MAP input to output
	EXPECT_CALL(openMap, getValue(_, _))
		.WillRepeatedly([](float xRpm, float tps) { return tps; });

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	BoostController bc;

	// Without table set, should return unexpected
	EXPECT_EQ(bc.getOpenLoop(0), unexpected);


    testBoostCltCorr.initTable(config->cltBoostCorr, config->cltBoostCorrBins);
    testBoostIatCorr.initTable(config->iatBoostCorr, config->iatBoostCorrBins);
    testBoostCltAdder.initTable(config->cltBoostAdder, config->cltBoostAdderBins);
    testBoostIatAdder.initTable(config->iatBoostAdder, config->iatBoostAdderBins);

    bc.init(
        nullptr,
        &openMap,
        nullptr,
        testBoostCltCorr,
        testBoostIatCorr,
        testBoostCltAdder,
        testBoostIatAdder,
        nullptr
    );

	// Should pass TPS value thru
	Sensor::setMockValue(SensorType::Tps1, 47.0f);
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), 47.0f);
}

TEST(BoostControl, BoostOpenLoopYAxis)
{
	MockVp3d openMap;
	// Just pass MAP input to output
	EXPECT_CALL(openMap, getValue(_, _))
		.WillRepeatedly([](float xRpm, float tps) { return tps; });

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	BoostController bc;

    testBoostCltCorr.initTable(config->cltBoostCorr, config->cltBoostCorrBins);
    testBoostIatCorr.initTable(config->iatBoostCorr, config->iatBoostCorrBins);
    testBoostCltAdder.initTable(config->cltBoostAdder, config->cltBoostAdderBins);
    testBoostIatAdder.initTable(config->iatBoostAdder, config->iatBoostAdderBins);

	bc.init(
        nullptr,
        &openMap,
        nullptr,
        testBoostCltCorr,
        testBoostIatCorr,
        testBoostCltAdder,
        testBoostIatAdder,
        nullptr
    );

	constexpr float RPM_TEST_VALUE = 42.0f;
	Sensor::setMockValue(SensorType::Rpm, RPM_TEST_VALUE);
	constexpr float TPS1_TEST_VALUE = 42.1f;
	Sensor::setMockValue(SensorType::Tps1, TPS1_TEST_VALUE);
	constexpr float MAP_TEST_VALUE = 42.3f;
	Sensor::setMockValue(SensorType::Map, MAP_TEST_VALUE);
	constexpr float CLT_TEST_VALUE = 42.4f;
	Sensor::setMockValue(SensorType::Clt, CLT_TEST_VALUE);
	constexpr float IAT_TEST_VALUE = 42.5f;
	Sensor::setMockValue(SensorType::Iat, IAT_TEST_VALUE);
	constexpr float LUA_GAUGE1_TEST_VALUE = 42.6f;
	Sensor::setMockValue(SensorType::LuaGauge1, LUA_GAUGE1_TEST_VALUE);
	constexpr float LUA_GAUGE2_TEST_VALUE = 42.7f;
	Sensor::setMockValue(SensorType::LuaGauge2, LUA_GAUGE2_TEST_VALUE);
	constexpr float AUX_TEMP1_TEST_VALUE = 42.7f;
	Sensor::setMockValue(SensorType::AuxTemp1, AUX_TEMP1_TEST_VALUE);
	constexpr float AUX_TEMP2_TEST_VALUE = 42.8f;
	Sensor::setMockValue(SensorType::AuxTemp2, AUX_TEMP2_TEST_VALUE);
	constexpr float ACCELERATOR_PEDAL_TEST_VALUE = 42.9f;
	Sensor::setMockValue(SensorType::AcceleratorPedal, ACCELERATOR_PEDAL_TEST_VALUE);
	constexpr float BATTERY_VOLTAGE_TEST_VALUE = 43.0f;
	Sensor::setMockValue(SensorType::BatteryVoltage, BATTERY_VOLTAGE_TEST_VALUE);
	constexpr float FUEL_ETANOL_PERCENT_TEST_VALUE = 43.1f;
	Sensor::setMockValue(SensorType::FuelEthanolPercent, FUEL_ETANOL_PERCENT_TEST_VALUE);
	constexpr float AUX_LINEAR1_TEST_VALUE = 43.2f;
	Sensor::setMockValue(SensorType::AuxLinear1, AUX_LINEAR1_TEST_VALUE);
	constexpr float AUX_LINEAR2_TEST_VALUE = 43.3f;
	Sensor::setMockValue(SensorType::AuxLinear2, AUX_LINEAR2_TEST_VALUE);
	// we cannot use value 43.4f for the DetectedGear sensor  because gear should be less then TCU_GEAR_COUNT
	constexpr float DETECTED_GEAR_TEST_VALUE = TCU_GEAR_COUNT - 1;
	Sensor::setMockValue(SensorType::DetectedGear, DETECTED_GEAR_TEST_VALUE);
	constexpr float BAROMETRIC_PRESSURE_TEST_VALUE = 43.5f;
	Sensor::setMockValue(SensorType::BarometricPressure, BAROMETRIC_PRESSURE_TEST_VALUE);
	constexpr float EGT1_TEST_VALUE = 43.6f;
	Sensor::setMockValue(SensorType::EGT1, EGT1_TEST_VALUE);
	constexpr float EGT2_TEST_VALUE = 43.7f;
	Sensor::setMockValue(SensorType::EGT2, EGT2_TEST_VALUE);

	EXPECT_EQ(engineConfiguration->boostOpenLoopYAxis, gppwm_channel_e::GPPWM_Tps); // default value
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), TPS1_TEST_VALUE);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_Zero;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), 0.0f);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_Rpm;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), RPM_TEST_VALUE);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_Tps;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), TPS1_TEST_VALUE);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_Map;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), MAP_TEST_VALUE);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_Clt;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), CLT_TEST_VALUE);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_Iat;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), IAT_TEST_VALUE);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_LuaGauge1;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), LUA_GAUGE1_TEST_VALUE);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_LuaGauge2;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), LUA_GAUGE2_TEST_VALUE);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_FuelLoad;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), engine->engineState.fuelingLoad);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_IgnLoad;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), engine->engineState.ignitionLoad);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_AuxTemp1;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), AUX_TEMP1_TEST_VALUE);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_AuxTemp2;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), AUX_TEMP2_TEST_VALUE);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_AccelPedal;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), ACCELERATOR_PEDAL_TEST_VALUE);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_Vbatt;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), BATTERY_VOLTAGE_TEST_VALUE);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_VVT_1I;
	EXPECT_FLOAT_EQ(
		bc.getOpenLoop(0).value_or(-1),
		engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/0)
	);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_VVT_1E;
	EXPECT_FLOAT_EQ(
		bc.getOpenLoop(0).value_or(-1),
		engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/1)
	);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_VVT_2I;
	EXPECT_FLOAT_EQ(
		bc.getOpenLoop(0).value_or(-1),
		engine->triggerCentral.getVVTPosition(/*bankIndex*/1, /*camIndex*/0)
	);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_VVT_2E;
	EXPECT_FLOAT_EQ(
		bc.getOpenLoop(0).value_or(-1),
		engine->triggerCentral.getVVTPosition(/*bankIndex*/1, /*camIndex*/1)
	);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_EthanolPercent;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), FUEL_ETANOL_PERCENT_TEST_VALUE);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_AuxLinear1;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), AUX_LINEAR1_TEST_VALUE);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_AuxLinear2;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), AUX_LINEAR2_TEST_VALUE);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_GppwmOutput1;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), (float)engine->outputChannels.gppwmOutput[0]);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_GppwmOutput2;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), (float)engine->outputChannels.gppwmOutput[1]);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_GppwmOutput3;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), (float)engine->outputChannels.gppwmOutput[2]);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_GppwmOutput4;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), (float)engine->outputChannels.gppwmOutput[3]);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_DetectedGear;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), DETECTED_GEAR_TEST_VALUE);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_BaroPressure;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), BAROMETRIC_PRESSURE_TEST_VALUE);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_Egt1;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), EGT1_TEST_VALUE);

	engineConfiguration->boostOpenLoopYAxis = GPPWM_Egt2;
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), EGT2_TEST_VALUE);
}

TEST(BoostControl, TestClosedLoop) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	BoostController bc;

	pid_s pidCfg = {
		1, 0, 0,	 // P controller, easier to test
		0,	// no offset
		5,	// 5ms period
		-100, 100 // min/max output
	};

	bc.init(
        nullptr,
        nullptr,
        nullptr,
        testBoostCltCorr,
        testBoostIatCorr,
        testBoostCltAdder,
        testBoostIatAdder,
        &pidCfg
    );

	// Enable closed loop
	engineConfiguration->boostType = CLOSED_LOOP;
	// Minimum 75kpa
	engineConfiguration->minimumBoostClosedLoopMap = 75;

	// At 0 RPM, closed loop is disabled
	Sensor::setMockValue(SensorType::Rpm, 0);
	EXPECT_EQ(0, bc.getClosedLoop(150, 100).value_or(-1000));

	// too low MAP, disable closed loop
	Sensor::setMockValue(SensorType::Rpm, 0);
	EXPECT_EQ(0, bc.getClosedLoop(150, 50).value_or(-1000));

	// With RPM, we should get an output
	Sensor::setMockValue(SensorType::Rpm, 1000);
	// Actual is below target -> positive output
	EXPECT_FLOAT_EQ(50, bc.getClosedLoop(150, 100).value_or(-1000));
	// Actual is above target -> negative output
	EXPECT_FLOAT_EQ(-25.0f, bc.getClosedLoop(150, 175).value_or(-1000));

	// Disabling closed loop should return 0
	engineConfiguration->boostType = OPEN_LOOP;
	EXPECT_FLOAT_EQ(0, bc.getClosedLoop(150, 175).value_or(-1000));
}

TEST(BoostControl, SetOutput) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->isBoostControlEnabled = true;

	StrictMock<MockPwm> pwm;
	StrictMock<MockEtb> etb;
	BoostController bc;

	// ETB wastegate position & PWM should both be set
	EXPECT_CALL(etb, setWastegatePosition(25.0f));
	EXPECT_CALL(pwm, setSimplePwmDutyCycle(0.25f));

	// Don't crash if not init'd (don't deref null ptr m_pwm)
	EXPECT_NO_THROW(bc.setOutput(25.0f));

	// Init with mock PWM device and ETB
	bc.init(&pwm, nullptr, nullptr, testBoostCltCorr, testBoostIatCorr, testBoostCltAdder, testBoostIatAdder, nullptr);
	engine->etbControllers[0] = &etb;

	bc.setOutput(25.0f);
}
