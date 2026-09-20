/**
 * @file test_bench_test.cpp
 *
 * Coverage for the TunerStudio bench command dispatch, handleBenchCategory(): every bench_mode_e
 * value that tunerstudio.template.ini binds to a button must be routed somewhere, because the
 * dispatcher's fallback used to be a criticalError() - a reboot on a button click. Issue #10285: the
 * "Idle Second Air Valve" button (BENCH_SECOND_IDLE_VALVE) had been in the .ini since 2021 with
 * no matching case; "Test Starter Disable" and Lua buttons 5..10 had the same hole.
 *
 * The bench thread does not exist in the unit test build, so a routed command is observed as the
 * request it queued for that thread (takePendingBenchRequestForUnitTest()).
 *
 * Reproduction stage of this coverage (before the fix) asserted EXPECT_THROW(..., std::logic_error)
 * for the three buttons below: criticalError() throws in the unit test build.
 */

#include "pch.h"

#include "bench_test.h"

static BenchRequestForUnitTest dispatch(bench_mode_e command) {
	// nothing left over from a previous command
	EXPECT_EQ(nullptr, takePendingBenchRequestForUnitTest().pin);
	executeTSCommand(TS_BENCH_CATEGORY, command);
	return takePendingBenchRequestForUnitTest();
}

TEST(BenchTest, tsButtonsRouteToOutputs) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->benchTestOnTime = 5;
	engineConfiguration->benchTestOffTime = 10;
	engineConfiguration->benchTestCount = 4000;

	auto request = dispatch(BENCH_HPFP_VALVE);
	EXPECT_EQ(&enginePins.hpfpValve, request.pin);
	EXPECT_FLOAT_EQ(5, request.onTimeMs);
	EXPECT_FLOAT_EQ(10, request.offTimeMs);
	EXPECT_EQ(4000, request.count);
	EXPECT_FALSE(request.swapOnOff);

	request = dispatch(BENCH_STARTER_ENABLE_RELAY);
	EXPECT_EQ(&enginePins.starterControl, request.pin);
	EXPECT_FLOAT_EQ(BENCH_STARTER_DURATION, request.onTimeMs);
	EXPECT_EQ(1, request.count);

	// #10285 sibling: "Test Starter Disable" button
	request = dispatch(BENCH_STARTER_DISABLE_RELAY);
	EXPECT_EQ(&enginePins.starterRelayDisable, request.pin);
	EXPECT_FLOAT_EQ(BENCH_STARTER_DURATION, request.onTimeMs);
	EXPECT_EQ(1, request.count);
	EXPECT_FALSE(request.swapOnOff);

	// main relay is normally-on, its bench test pulses it *off*
	request = dispatch(BENCH_MAIN_RELAY);
	EXPECT_EQ(&enginePins.mainRelay, request.pin);
	EXPECT_TRUE(request.swapOnOff);

	EXPECT_EQ(&enginePins.fuelPumpRelay, dispatch(BENCH_FUEL_PUMP).pin);
	EXPECT_EQ(&enginePins.fanRelay, dispatch(BENCH_FAN_RELAY).pin);
	EXPECT_EQ(&enginePins.fanRelay2, dispatch(BENCH_FAN_RELAY_2).pin);
	EXPECT_EQ(&enginePins.acRelay, dispatch(BENCH_AC_COMPRESSOR_RELAY).pin);
	EXPECT_EQ(&enginePins.checkEnginePin, dispatch(BENCH_CHECK_ENGINE_LIGHT).pin);
	EXPECT_EQ(&enginePins.boostPin, dispatch(BENCH_BOOST_VALVE).pin);
}

TEST(BenchTest, idleValveButtonRunsIdleControllerForThreeSeconds) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	ASSERT_EQ(0, engine->timeToStopIdleTest);

	// the idle valve bench is not a pin pulse: it lets the idle controller drive the valve(s)
	// (both coils in double solenoid mode) while the engine is stopped
	EXPECT_EQ(nullptr, dispatch(BENCH_IDLE_VALVE).pin);
	EXPECT_EQ(getTimeNowUs() + MS2US(3000), engine->timeToStopIdleTest);
}

TEST(BenchTest, luaButtonsCountPresses) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	memset(luaCommandCounters, 0, sizeof(luaCommandCounters));

	// Distinct counts check every button's routing and catch accidental updates of adjacent slots.
	for (int button = 0; button < LUA_BUTTON_COUNT; button++) {
		for (int press = 0; press <= button; press++) {
			executeTSCommand(TS_BENCH_CATEGORY, LUA_COMMAND_1 + button);
		}
	}
	for (int button = 0; button < LUA_BUTTON_COUNT; button++) {
		EXPECT_EQ(button + 1, luaCommandCounters[button]) << "Lua button " << button + 1;
	}
	EXPECT_EQ(0, eth.getWarningCounter());
	EXPECT_EQ(nullptr, takePendingBenchRequestForUnitTest().pin);
}

// #10285: "Idle Second Air Valve" was "Unexpected bench function 14" on the TS/console screen and a reboot
TEST(BenchTest, secondIdleValveButtonPulsesSecondSolenoid) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->benchTestOnTime = 5;
	engineConfiguration->benchTestOffTime = 10;
	engineConfiguration->benchTestCount = 4000;

	auto request = dispatch(BENCH_SECOND_IDLE_VALVE);
	EXPECT_EQ(&enginePins.secondIdleSolenoidPin, request.pin);
	EXPECT_FLOAT_EQ(5, request.onTimeMs);
	EXPECT_FLOAT_EQ(10, request.offTimeMs);
	EXPECT_EQ(4000, request.count);
	EXPECT_FALSE(request.swapOnOff);
	// unlike the "Idle Air Valve" button this is a plain pin pulse, not an idle controller run
	EXPECT_EQ(0, engine->timeToStopIdleTest);
}

TEST(BenchTest, noBenchFunctionRebootsEcu) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	memset(luaCommandCounters, 0, sizeof(luaCommandCounters));

	// a routed command leaves no warning behind, an unrouted one (no .ini button sends GPPWM) says so
	EXPECT_EQ(&enginePins.hpfpValve, dispatch(BENCH_HPFP_VALVE).pin);
	EXPECT_FALSE(engine->engineState.warnings.isWarningNow(ObdCode::CUSTOM_ERR_BENCH_PARAM));
	EXPECT_EQ(nullptr, dispatch(BENCH_GPPWM1_VALVE).pin);
	EXPECT_TRUE(engine->engineState.warnings.isWarningNow(ObdCode::CUSTOM_ERR_BENCH_PARAM));

	// every value of bench_mode_e, including the ones compiled out of this build (VVT, HD ACR):
	// the worst outcome is that warning, never a criticalError()
	for (int index = 0; index <= BENCH_BOOST_VALVE; index++) {
		EXPECT_NO_THROW(handleBenchCategory(index)) << "bench function " << index;
		takePendingBenchRequestForUnitTest();
	}
}

TEST(BenchTest, unknownSubsystemWarnsAndAllowsNextCommand) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	EXPECT_NO_THROW(executeTSCommand(0xffff, 0xffff));
	EXPECT_TRUE(engine->engineState.warnings.isWarningNow(ObdCode::CUSTOM_ERR_BENCH_PARAM));
	EXPECT_FALSE(hasFirmwareError());
	EXPECT_EQ(nullptr, takePendingBenchRequestForUnitTest().pin);
	EXPECT_EQ(&enginePins.secondIdleSolenoidPin, dispatch(BENCH_SECOND_IDLE_VALVE).pin);
}

TEST(BenchTest, unknownX14CommandWarnsAndAllowsNextCommand) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	EXPECT_NO_THROW(executeTSCommand(TS_X14, 0xffff));
	EXPECT_TRUE(engine->engineState.warnings.isWarningNow(ObdCode::CUSTOM_ERR_BENCH_PARAM));
	EXPECT_FALSE(hasFirmwareError());
	EXPECT_EQ(nullptr, takePendingBenchRequestForUnitTest().pin);

	EXPECT_FALSE(engine->etbIgnoreJamProtection);
	executeTSCommand(TS_X14, TS_ETB_DISABLE_JAM_DETECT);
	EXPECT_TRUE(engine->etbIgnoreJamProtection);
}

TEST(BenchTest, unknownBenchCommandWarnsWithoutQueuingOutput) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	EXPECT_NO_THROW(executeTSCommand(TS_BENCH_CATEGORY, 0xffff));
	EXPECT_TRUE(engine->engineState.warnings.isWarningNow(ObdCode::CUSTOM_ERR_BENCH_PARAM));
	EXPECT_FALSE(hasFirmwareError());
	EXPECT_EQ(nullptr, takePendingBenchRequestForUnitTest().pin);
}

TEST(BenchTest, secondIdleSolenoidIgnoresPwmWritesDuringBench) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto request = dispatch(BENCH_SECOND_IDLE_VALVE);
	ASSERT_EQ(&enginePins.secondIdleSolenoidPin, request.pin);

	// runBench owns the output while its scheduled callbacks use forced writes.
	setOutputOnTheBenchTestForUnitTest(request.pin);
	request.pin->setValue(1, /*isForce*/ true);
	request.pin->setValue(0);
	EXPECT_TRUE(request.pin->getLogicValue());
	request.pin->setValue(0, /*isForce*/ true);
	request.pin->setValue(1);
	EXPECT_FALSE(request.pin->getLogicValue());

	// The other coil remains under normal control.
	enginePins.idleSolenoidPin.setValue(1);
	EXPECT_TRUE(enginePins.idleSolenoidPin.getLogicValue());
	enginePins.idleSolenoidPin.setValue(0);

	setOutputOnTheBenchTestForUnitTest(nullptr);
	request.pin->setValue(1);
	EXPECT_TRUE(request.pin->getLogicValue());
	request.pin->setValue(0);
}
