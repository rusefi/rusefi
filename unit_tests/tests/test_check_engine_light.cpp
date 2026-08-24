#include "pch.h"

#ifdef MODULE_CHECK_ENGINE_LIGHT

#include "check_engine_light.h"
#include "functional_sensor.h"
#include "identity_func.h"
#include "malfunction_central.h"
#ifdef MODULE_MIL
#include "bench_test.h"
#include "malfunction_indicator.h"
#endif
#include "stored_value_sensor.h"

class CheckEngineLightTest : public ::testing::Test {
protected:
	CheckEngineLightTest()
		: eth(engine_type_e::TEST_ENGINE)
		, battery(SensorType::BatteryVoltage, MS2NT(60'000))
		, map(SensorType::MapSlow, MS2NT(60'000))
		, iat(SensorType::Iat, MS2NT(60'000))
		, tps(SensorType::Tps1Primary, MS2NT(60'000)) {
	}

	void SetUp() override {
		registerSensor(battery, 12.0f);
		registerSensor(map, 2.5f);
		registerSensor(iat, 2.5f);
		registerSensor(tps, 2.5f);

		auto& sensorChecker = engine->module<SensorChecker>();
		sensorChecker->onIgnitionStateChanged(true);
		sensorChecker->onSlowCallback();
		advanceTimeUs(6'000'000);
		sensorChecker->onSlowCallback();
		ASSERT_TRUE(sensorChecker->analogSensorsShouldWork());
#ifdef MODULE_MIL
		engine->module<MILController>()->onIgnitionStateChanged(true);
#endif
	}

	void registerSensor(FunctionalSensor& sensor, float value) {
		sensor.setFunction(identityFunction);
		ASSERT_TRUE(sensor.Register());
		sensor.postRawValue(value, getTimeNowNt());
	}

	void evaluateAfterDebounce() {
		auto& cel = engine->module<CheckEngineLight>();
		cel->onSlowCallback();
		advanceTimeUs(1'100'000);
		cel->onSlowCallback();
#ifdef MODULE_MIL
		engine->module<MILController>()->onSlowCallback();
#endif
	}

	bool hasCode(ObdCode code) {
		error_codes_set_s errors;
		getErrorCodes(&errors);

		for (int i = 0; i < errors.count; i++) {
			if (errors.error_codes[i] == code) {
				return true;
			}
		}

		return false;
	}

	EngineTestHelper eth;
	FunctionalSensor battery;
	FunctionalSensor map;
	FunctionalSensor iat;
	FunctionalSensor tps;
};

TEST_F(CheckEngineLightTest, freshTuneDefaults) {
	EXPECT_FLOAT_EQ(6.0f, config->cel_battery_min_v);
	EXPECT_FLOAT_EQ(18.0f, config->cel_battery_max_v);
	EXPECT_FLOAT_EQ(0.2f, config->cel_map_min_v);
	EXPECT_FLOAT_EQ(4.8f, config->cel_map_max_v);
	EXPECT_FLOAT_EQ(0.2f, config->cel_iat_min_v);
	EXPECT_FLOAT_EQ(4.8f, config->cel_iat_max_v);
	EXPECT_FLOAT_EQ(0.2f, config->cel_tps_min_v);
	EXPECT_FLOAT_EQ(4.8f, config->cel_tps_max_v);
}

TEST_F(CheckEngineLightTest, configuredVoltageFaults) {
	map.postRawValue(0.1f, getTimeNowNt());
	evaluateAfterDebounce();
	EXPECT_TRUE(hasCode(ObdCode::OBD_Map_Low));
#ifdef MODULE_MIL
	EXPECT_TRUE(enginePins.checkEnginePin.getLogicValue());
#endif

	clearWarnings();
	engine->module<CheckEngineLight>()->onSlowCallback();
#ifdef MODULE_MIL
	engine->module<MILController>()->onSlowCallback();
#endif
	EXPECT_TRUE(hasCode(ObdCode::OBD_Map_Low));

	iat.postRawValue(4.9f, getTimeNowNt());
	evaluateAfterDebounce();
	EXPECT_TRUE(hasCode(ObdCode::OBD_Map_Low));
	EXPECT_TRUE(hasCode(ObdCode::OBD_Iat_High));

	map.postRawValue(2.5f, getTimeNowNt());
	evaluateAfterDebounce();
	EXPECT_FALSE(hasCode(ObdCode::OBD_Map_Low));
#ifdef MODULE_MIL
	EXPECT_TRUE(enginePins.checkEnginePin.getLogicValue());
#endif

	iat.postRawValue(2.5f, getTimeNowNt());
	evaluateAfterDebounce();
	EXPECT_FALSE(hasErrorCodes());
#ifdef MODULE_MIL
	EXPECT_TRUE(enginePins.checkEnginePin.getLogicValue());
#endif

	tps.postRawValue(0.1f, getTimeNowNt());
	evaluateAfterDebounce();
	EXPECT_TRUE(hasCode(ObdCode::OBD_TPS1_Primary_Low));

	tps.postRawValue(2.5f, getTimeNowNt());
	battery.postRawValue(20.0f, getTimeNowNt());
	evaluateAfterDebounce();
	EXPECT_FALSE(hasCode(ObdCode::OBD_TPS1_Primary_Low));
	EXPECT_TRUE(hasCode(ObdCode::OBD_System_Voltage_Malfunction));
}

TEST_F(CheckEngineLightTest, disabledRangesAndIgnitionGate) {
	config->cel_map_min_v = 0;
	config->cel_map_max_v = 0;
	map.postRawValue(0.1f, getTimeNowNt());
	evaluateAfterDebounce();
	EXPECT_FALSE(hasErrorCodes());

	config->cel_map_min_v = 4.8f;
	config->cel_map_max_v = 0.2f;
	evaluateAfterDebounce();
	EXPECT_FALSE(hasErrorCodes());

	config->cel_map_min_v = 0.2f;
	config->cel_map_max_v = 4.8f;
	evaluateAfterDebounce();
	ASSERT_TRUE(hasErrorCodes());

	auto& sensorChecker = engine->module<SensorChecker>();
	sensorChecker->onIgnitionStateChanged(false);
#ifdef MODULE_MIL
	engine->module<MILController>()->onIgnitionStateChanged(false);
#endif
	sensorChecker->onSlowCallback();
	engine->module<CheckEngineLight>()->onSlowCallback();
#ifdef MODULE_MIL
	engine->module<MILController>()->onSlowCallback();
#endif
	EXPECT_FALSE(hasErrorCodes());
#ifdef MODULE_MIL
	EXPECT_FALSE(enginePins.checkEnginePin.getLogicValue());
#endif
}

TEST_F(CheckEngineLightTest, interruptedRecoveryRestartsDebounce) {
	map.postRawValue(0.1f, getTimeNowNt());
	evaluateAfterDebounce();
	ASSERT_TRUE(hasCode(ObdCode::OBD_Map_Low));

	map.postRawValue(2.5f, getTimeNowNt());
	engine->module<CheckEngineLight>()->onSlowCallback();
	advanceTimeUs(500'000);
	map.postRawValue(0.1f, getTimeNowNt());
	engine->module<CheckEngineLight>()->onSlowCallback();

	advanceTimeUs(600'000);
	map.postRawValue(2.5f, getTimeNowNt());
	engine->module<CheckEngineLight>()->onSlowCallback();
	advanceTimeUs(500'000);
	engine->module<CheckEngineLight>()->onSlowCallback();
	EXPECT_TRUE(hasCode(ObdCode::OBD_Map_Low));

	advanceTimeUs(600'000);
	engine->module<CheckEngineLight>()->onSlowCallback();
	EXPECT_FALSE(hasErrorCodes());
}

TEST(checkEngineLight, sensorWithoutRawVoltageIsIgnored) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	StoredValueSensor iat(SensorType::Iat, MS2NT(60'000));
	ASSERT_TRUE(iat.Register());
	iat.setValidValue(30.0f, getTimeNowNt());

	auto& sensorChecker = engine->module<SensorChecker>();
	sensorChecker->onIgnitionStateChanged(true);
	sensorChecker->onSlowCallback();
	advanceTimeUs(6'000'000);
	sensorChecker->onSlowCallback();
	ASSERT_TRUE(sensorChecker->analogSensorsShouldWork());

	auto& cel = engine->module<CheckEngineLight>();
	cel->onSlowCallback();
	advanceTimeUs(1'100'000);
	cel->onSlowCallback();

	EXPECT_FALSE(hasErrorCodes());
}

#ifdef MODULE_MIL
TEST(checkEngineLight, milControllerBlinksDiagnosticCode) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& mil = engine->module<MILController>();
	addError(ObdCode::OBD_Map_Low);
	mil->onSlowCallback();
	EXPECT_FALSE(enginePins.checkEnginePin.getLogicValue());

	mil->onIgnitionStateChanged(true);

	// 107: two short pulses, one long pulse, eight short pulses
	mil->onSlowCallback();

	auto expectPulse = [&](int durationMs) {
		EXPECT_TRUE(enginePins.checkEnginePin.getLogicValue());
		advanceTimeUs(MS2US(durationMs - 1));
		mil->onSlowCallback();
		EXPECT_TRUE(enginePins.checkEnginePin.getLogicValue());
		advanceTimeUs(MS2US(2));
		mil->onSlowCallback();
		EXPECT_FALSE(enginePins.checkEnginePin.getLogicValue());
		advanceTimeUs(401'000);
		mil->onSlowCallback();
	};

	expectPulse(400);
	expectPulse(400);
	expectPulse(1500);
	for (int i = 0; i < 8; i++) {
		expectPulse(400);
	}

	// The single code repeats after its final gap.
	EXPECT_TRUE(enginePins.checkEnginePin.getLogicValue());

	clearWarnings();
	mil->onSlowCallback();
	EXPECT_TRUE(enginePins.checkEnginePin.getLogicValue());

	engine->rpmCalculator.setRpmValue(100);
	mil->onSlowCallback();
	EXPECT_FALSE(enginePins.checkEnginePin.getLogicValue());

	engine->rpmCalculator.setStopSpinning();
	mil->onSlowCallback();
	EXPECT_TRUE(enginePins.checkEnginePin.getLogicValue());
}

TEST(checkEngineLight, milRestartsAfterBenchTest) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& mil = engine->module<MILController>();
	mil->onIgnitionStateChanged(true);

	addError(ObdCode::OBD_Map_Low);
	mil->onSlowCallback();
	ASSERT_TRUE(enginePins.checkEnginePin.getLogicValue());

	setOutputOnTheBenchTestForUnitTest(&enginePins.checkEnginePin);
	enginePins.checkEnginePin.setValue("bench", false, true);
	advanceTimeUs(2'000'000);
	mil->onSlowCallback();
	EXPECT_FALSE(enginePins.checkEnginePin.getLogicValue());

	setOutputOnTheBenchTestForUnitTest(nullptr);
	mil->onSlowCallback();
	EXPECT_TRUE(enginePins.checkEnginePin.getLogicValue());
}
#endif

#endif
