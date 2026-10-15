#include "pch.h"

#ifdef MODULE_CHECK_ENGINE_LIGHT

#include "check_engine_light.h"
#include "functional_sensor.h"
#include "identity_func.h"
#include "malfunction_central.h"
#include "board_overrides.h"
#ifdef MODULE_MIL
#include "bench_test.h"
#include "malfunction_indicator.h"
#endif
#include "stored_value_sensor.h"

#ifdef MODULE_MIL
// Board hooks live for the process lifetime. Keep upstream MIL tests isolated
// from custom-board setup performed by other suites in the same binary.
class MilCodeFilterGuard {
public:
	explicit MilCodeFilterGuard(board_mil_code_filter_type filter = nullptr)
		: m_previous(custom_board_milCodeFilter) {
		custom_board_milCodeFilter = filter ? std::optional<board_mil_code_filter_type>(filter) : std::nullopt;
	}
	~MilCodeFilterGuard() {
		custom_board_milCodeFilter = m_previous;
	}
private:
	std::optional<board_mil_code_filter_type> m_previous;
};
#endif

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

#ifdef MODULE_MIL
	// A started blink sequence is finished even if the code is already gone,
	// let it run out before checking the plain (non-diagnostic) lamp state.
	void finishBlinking() {
		auto& mil = engine->module<MILController>();
		for (int i = 0; i < 200 && mil->isBlinking(); i++) {
			advanceTimeUs(MS2US(100));
			mil->onSlowCallback();
		}
		mil->onSlowCallback();
	}
#endif

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
#ifdef MODULE_MIL
	MilCodeFilterGuard filterGuard;
#endif
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
	finishBlinking();
	// no codes left: the lamp is simply on because the engine is not spinning
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
	MilCodeFilterGuard filterGuard;
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& mil = engine->module<MILController>();
	addError(ObdCode::OBD_Map_Low);
	mil->onSlowCallback();
	EXPECT_FALSE(enginePins.checkEnginePin.getLogicValue());

	mil->onIgnitionStateChanged(true);

	// 107: two short pulses, one long pulse, eight short pulses
	mil->onSlowCallback();

	// the lamp is steady on for five seconds, then pauses before the first digit
	EXPECT_TRUE(enginePins.checkEnginePin.getLogicValue());
	advanceTimeUs(MS2US(4999));
	mil->onSlowCallback();
	EXPECT_TRUE(enginePins.checkEnginePin.getLogicValue());
	advanceTimeUs(MS2US(2));
	mil->onSlowCallback();
	EXPECT_FALSE(enginePins.checkEnginePin.getLogicValue());
	advanceTimeUs(MS2US(2001));
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

	// The sequence is over: the lamp stays off for the pause which follows the last code.
	EXPECT_FALSE(enginePins.checkEnginePin.getLogicValue());

	clearWarnings();
	advanceTimeUs(MS2US(2001));
	mil->onSlowCallback();
	EXPECT_FALSE(enginePins.checkEnginePin.getLogicValue());
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
	MilCodeFilterGuard filterGuard;
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
	EXPECT_FALSE(mil->isLampOn());
	EXPECT_FALSE(mil->isDiagnosticLampOn());

	setOutputOnTheBenchTestForUnitTest(nullptr);
	mil->onSlowCallback();
	EXPECT_TRUE(enginePins.checkEnginePin.getLogicValue());
	EXPECT_TRUE(mil->isDiagnosticLampOn());
}

TEST(checkEngineLight, filteredMilPublishesLogicalPulsesAndSkipsRejectedCodes) {
	MilCodeFilterGuard filterGuard([](ObdCode code) {
		return code == ObdCode::OBD_Map_Low;
	});
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& mil = engine->module<MILController>();
	engine->rpmCalculator.setRpmValue(1000);
	addError(ObdCode::OBD_Iat_High); // rejected code comes first
	addError(ObdCode::OBD_Map_Low);
	addError(ObdCode::OBD_TPS1_Primary_High);
	mil->onSlowCallback();
	EXPECT_FALSE(mil->isLampOn()); // ignition off
	EXPECT_FALSE(mil->isDiagnosticLampOn());
	mil->onIgnitionStateChanged(true);
	mil->onSlowCallback();

	// the steady lamp and the pause which open every round
	auto expectPrelude = [&]() {
		EXPECT_TRUE(mil->isDiagnosticLampOn());
		advanceTimeUs(MS2US(4999));
		mil->onSlowCallback();
		EXPECT_TRUE(mil->isDiagnosticLampOn());
		advanceTimeUs(MS2US(2));
		mil->onSlowCallback();
		EXPECT_FALSE(mil->isLampOn());
		advanceTimeUs(MS2US(2001));
		mil->onSlowCallback();
	};

	auto expectPulse = [&](int durationMs) {
		EXPECT_TRUE(mil->isLampOn());
		EXPECT_TRUE(mil->isDiagnosticLampOn());
		EXPECT_TRUE(mil->isBlinking());
		advanceTimeUs(MS2US(durationMs - 1));
		mil->onSlowCallback();
		EXPECT_TRUE(mil->isDiagnosticLampOn());
		advanceTimeUs(2'000);
		mil->onSlowCallback();
		EXPECT_FALSE(mil->isLampOn());
		EXPECT_FALSE(mil->isDiagnosticLampOn());
		EXPECT_TRUE(mil->isBlinking()); // off gap still belongs to the sequence
		advanceTimeUs(401'000);
		mil->onSlowCallback();
	};
	// Two whole repetitions of 107 prove rejected neighbors are skipped during
	// selection and rotation, and preserve the existing digit/pulse encoding.
	for (int repeat = 0; repeat < 2; repeat++) {
		if (repeat > 0) {
			// the last code is followed by a pause, then idle, then the round starts over
			EXPECT_FALSE(mil->isLampOn());
			advanceTimeUs(MS2US(2001));
			mil->onSlowCallback();
			EXPECT_FALSE(mil->isLampOn());
			mil->onSlowCallback();
		}
		expectPrelude();
		expectPulse(400);
		expectPulse(400);
		expectPulse(1500);
		for (int i = 0; i < 8; i++) {
			expectPulse(400);
		}
	}

	// wait out the pause which follows the last code of the round
	advanceTimeUs(MS2US(2001));
	mil->onSlowCallback();

	// Changing the policy while a pulse is active stops diagnostic blinking.
	custom_board_milCodeFilter = [](ObdCode) { return false; };
	mil->onSlowCallback();
	EXPECT_FALSE(mil->isLampOn());
	EXPECT_FALSE(mil->isDiagnosticLampOn());
	error_codes_set_s codes;
	EXPECT_FALSE(mil->isBlinking());
	getErrorCodes(&codes);
	EXPECT_EQ(3, codes.count); // display filtering never mutates diagnostics

	// The stopped-engine lamp check is separate from diagnostic pulses.
	engine->rpmCalculator.setStopSpinning();
	mil->onSlowCallback();
	EXPECT_TRUE(mil->isLampOn());
	EXPECT_FALSE(mil->isDiagnosticLampOn());
	mil->onIgnitionStateChanged(false);
	mil->onSlowCallback();
	EXPECT_FALSE(mil->isLampOn());
}

TEST(checkEngineLight, milSeparatesCodesWithPauseAndRestartsWithSteadyLamp) {
	MilCodeFilterGuard filterGuard([](ObdCode code) {
		return code == ObdCode::OBD_Map_Low || code == ObdCode::OBD_Iat_High;
	});
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& mil = engine->module<MILController>();
	engine->rpmCalculator.setRpmValue(1000);
	addError(ObdCode::OBD_Map_Low);
	addError(ObdCode::OBD_Iat_High);
	mil->onIgnitionStateChanged(true);

	// measures how long the lamp keeps its current state, in milliseconds
	auto measureState = [&](bool expectedState) {
		EXPECT_EQ(expectedState, mil->isLampOn());
		int ms = 0;
		while (mil->isLampOn() == expectedState && ms < 10000) {
			advanceTimeUs(MS2US(1));
			mil->onSlowCallback();
			ms++;
		}
		return ms;
	};

	mil->onSlowCallback();
	// the round opens with five seconds of steady light and a two second pause
	EXPECT_NEAR(5000, measureState(true), 2);
	EXPECT_NEAR(2000, measureState(false), 2);

	// blink out the first code, its digits are separated by short gaps only
	int offMs = 0;
	do {
		measureState(true);
		offMs = measureState(false);
	} while (offMs < 1000);

	// the codes themselves are separated by a two second pause
	EXPECT_NEAR(2000, offMs, 2);

	// the second code, then the steady lamp again instead of a plain pause
	int onMs = 0;
	do {
		onMs = measureState(true);
		if (onMs < 2000) {
			measureState(false);
		}
	} while (onMs < 2000);

	EXPECT_NEAR(5000, onMs, 2);
	EXPECT_NEAR(2000, measureState(false), 2);
}

TEST(checkEngineLight, filteredMilFinishesSequenceWhenActiveCodeIsRemoved) {
	MilCodeFilterGuard filterGuard([](ObdCode code) {
		return code == ObdCode::OBD_Map_Low;
	});
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& mil = engine->module<MILController>();
	engine->rpmCalculator.setRpmValue(1000);
	mil->onIgnitionStateChanged(true);
	addError(ObdCode::OBD_Iat_High);
	mil->onSlowCallback();
	EXPECT_FALSE(mil->isDiagnosticLampOn());
	addError(ObdCode::OBD_Map_Low);
	mil->onSlowCallback();
	EXPECT_TRUE(mil->isDiagnosticLampOn());
	removeError(ObdCode::OBD_Map_Low);
	// the sequence is not aborted in the middle, that would confuse the user
	mil->onSlowCallback();
	EXPECT_TRUE(mil->isDiagnosticLampOn());
	EXPECT_TRUE(hasErrorCodes()); // rejected IAT code remains

	// let the started code blink out to the end
	for (int i = 0; i < 200 && mil->isBlinking(); i++) {
		advanceTimeUs(MS2US(100));
		mil->onSlowCallback();
	}
	EXPECT_FALSE(mil->isBlinking());
	EXPECT_FALSE(mil->isLampOn()); // engine is running and no code is accepted anymore

	addError(ObdCode::OBD_Map_Low);
	mil->onSlowCallback();
	EXPECT_TRUE(mil->isDiagnosticLampOn());
}
#endif

TEST(checkEngineLight, filteredSnapshotsPreserveStorageAndOrder) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	addError(ObdCode::OBD_Iat_High);
	addError(ObdCode::OBD_Map_Low);
	addError(ObdCode::OBD_TPS1_Primary_High);
	const ObdCodeFilter filter = [](ObdCode code) {
		return code != ObdCode::OBD_Iat_High;
	};
	error_codes_set_s codes;
	getErrorCodes(&codes, filter);
	ASSERT_EQ(2, codes.count);
	EXPECT_EQ(ObdCode::OBD_Map_Low, codes.error_codes[0]);
	EXPECT_EQ(ObdCode::OBD_TPS1_Primary_High, codes.error_codes[1]);
	EXPECT_TRUE(hasErrorCodes(filter));
	getErrorCodes(&codes, [](ObdCode) { return false; });
	EXPECT_EQ(0, codes.count);
	EXPECT_EQ(ObdCode::None, codes.error_codes[0]);
	EXPECT_FALSE(hasErrorCodes([](ObdCode) { return false; }));
	EXPECT_TRUE(hasErrorCodes());
	getErrorCodes(&codes);
	ASSERT_EQ(3, codes.count);
	EXPECT_EQ(ObdCode::OBD_Iat_High, codes.error_codes[0]);
	EXPECT_EQ(ObdCode::OBD_Map_Low, codes.error_codes[1]);
	EXPECT_EQ(ObdCode::OBD_TPS1_Primary_High, codes.error_codes[2]);
	clearWarnings();
	getErrorCodes(&codes, filter);
	EXPECT_EQ(0, codes.count);
	EXPECT_FALSE(hasErrorCodes(filter));
}

#endif
