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
#include "vvt.h"

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

	// Boards may provide extra counters, but the bench protocol only defines commands 1..10.
	constexpr int benchButtonCount = LUA_COMMAND_10 - LUA_COMMAND_1 + 1;
	static_assert(LUA_BUTTON_COUNT >= benchButtonCount);

	// Distinct counts check every button's routing and catch accidental updates of adjacent slots.
	for (int button = 0; button < benchButtonCount; button++) {
		for (int press = 0; press <= button; press++) {
			executeTSCommand(TS_BENCH_CATEGORY, LUA_COMMAND_1 + button);
		}
	}
	for (int button = 0; button < LUA_BUTTON_COUNT; button++) {
		const int expectedPresses = button < benchButtonCount ? button + 1 : 0;
		EXPECT_EQ(expectedPresses, luaCommandCounters[button]) << "Lua button " << button + 1;
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

	// every value of bench_mode_e, including the ones compiled out of this build (HD ACR):
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

class VvtBenchTest : public ::testing::Test {
protected:
	EngineTestHelper eth{engine_type_e::TEST_ENGINE};

	// Bank 1 intake/exhaust, then bank 2 intake/exhaust, as wired by the TS buttons.
	static constexpr bench_mode_e commands[] = {
		BENCH_VVT0_VALVE, BENCH_VVT1_VALVE, BENCH_VVT2_VALVE, BENCH_VVT3_VALVE
	};
	static constexpr brain_pin_e pins[] = {Gpio::D0, Gpio::D1, Gpio::D2, Gpio::D3};
	static constexpr int pwmPeriodUs = 10000;

	void SetUp() override {
		setOutputOnTheBenchTestForUnitTest(nullptr);
		takePendingBenchRequestForUnitTest();
		engine->scheduler.clear();
		setTimeNowUs(0);
		stopVvtControlPins();
		for (size_t i = 0; i < efi::size(pins); i++) {
			engineConfiguration->vvtPins[i] = pins[i];
		}
		engineConfiguration->vvtOutputFrequency = 100;
		startVvtControlPins();
	}

	void TearDown() override {
		// The VVT outputs/PWMs are file-scope statics. Never leave bench ownership,
		// scheduled callbacks, or registered pins behind, even after an ASSERT failure.
		setOutputOnTheBenchTestForUnitTest(nullptr);
		takePendingBenchRequestForUnitTest();
		engine->scheduler.clear();
		stopVvtControlPins();
	}
};

TEST_F(VvtBenchTest, allFourTsButtonsQueueTheCorrectPinAndFixedPulse) {
	// VVT deliberately ignores the general-purpose pulse train settings.
	engineConfiguration->benchTestOnTime = 5;
	engineConfiguration->benchTestOffTime = 10;
	engineConfiguration->benchTestCount = 4000;

	for (size_t i = 0; i < efi::size(commands); i++) {
		SCOPED_TRACE(i);
		auto request = dispatch(commands[i]);
		ASSERT_NE(nullptr, request.pin);
		EXPECT_EQ(pins[i], request.pin->brainPin);
		EXPECT_EQ(getVvtOutputPin(i), request.pin);
		// The simulator/CAN QC lookup must agree with TS dispatch.
		EXPECT_EQ(request.pin, enginePins.getOutputPinForBenchMode(commands[i]));
		EXPECT_FLOAT_EQ(300, request.onTimeMs);
		EXPECT_FLOAT_EQ(100, request.offTimeMs);
		EXPECT_EQ(1, request.count);
		EXPECT_FALSE(request.swapOnOff);
	}
	EXPECT_EQ(0, eth.getWarningCounter());
}

TEST_F(VvtBenchTest, eachBenchPulseOwnsOnlyItsPinAndPwmResumesAfterRelease) {
	// Prove all four real PWM callbacks are active before taking ownership.
	for (size_t i = 0; i < efi::size(pins); i++) {
		EXPECT_TRUE(getVvtOutputPin(i)->getLogicValue());
	}
	eth.moveTimeForwardAndInvokeEventsUs(1000); // initial duty = 10% at 100 Hz
	for (size_t i = 0; i < efi::size(pins); i++) {
		EXPECT_FALSE(getVvtOutputPin(i)->getLogicValue());
	}
	eth.moveTimeForwardAndInvokeEventsUs(9000);
	for (size_t i = 0; i < efi::size(pins); i++) {
		EXPECT_TRUE(getVvtOutputPin(i)->getLogicValue());
	}

	for (size_t selected = 0; selected < efi::size(commands); selected++) {
		SCOPED_TRACE(selected);
		auto request = dispatch(commands[selected]);
		ASSERT_NE(nullptr, request.pin);
		auto* output = request.pin;
		int before[4];
		for (size_t i = 0; i < efi::size(pins); i++) {
			before[i] = getVvtOutputPin(i)->pinToggleCounter;
		}

		// Model the bench worker's ownership and forced edges. PWM events themselves
		// run through the production scheduler callbacks, not direct setValue substitutes.
		setOutputOnTheBenchTestForUnitTest(output);
		output->setValue(1, /*isForce*/ true);
		const int highCount = output->pinToggleCounter;
		eth.moveTimeForwardAndInvokeEventsUs(300000);
		EXPECT_TRUE(output->getLogicValue());
		EXPECT_EQ(highCount, output->pinToggleCounter); // no intervening PWM glitches

		output->setValue(0, /*isForce*/ true);
		const int lowCount = output->pinToggleCounter;
		eth.moveTimeForwardAndInvokeEventsUs(100000);
		EXPECT_FALSE(output->getLogicValue());
		EXPECT_EQ(lowCount, output->pinToggleCounter);

		for (size_t i = 0; i < efi::size(pins); i++) {
			if (i != selected) {
				EXPECT_EQ(80, getVvtOutputPin(i)->pinToggleCounter - before[i]) << "output " << i;
			}
		}

		setOutputOnTheBenchTestForUnitTest(nullptr);
		eth.moveTimeForwardAndInvokeEventsUs(pwmPeriodUs);
		EXPECT_TRUE(output->getLogicValue());
		const int resumedCount = output->pinToggleCounter;
		eth.moveTimeForwardAndInvokeEventsUs(pwmPeriodUs);
		EXPECT_EQ(resumedCount + 2, output->pinToggleCounter);
	}
	EXPECT_EQ(0, eth.getWarningCounter());
}
