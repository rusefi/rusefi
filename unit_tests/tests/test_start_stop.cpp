/*
 * @file	test_start_stop.cpp
 *
 * @date Nov 18, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
*/

#include "pch.h"
#include "rusefi_lua.h"
#include "start_stop.h"

class StartStopTest : public testing::Test {
protected:
	void SetUp() override {
		enginePins.starterControl.setValue(false);
	}

	void TearDown() override {
		// EngineTestHelper resets injector/coil outputs, but not the starter's logical state.
		enginePins.starterControl.setValue(false);
	}
};

TEST_F(StartStopTest, luaStartAfterStopRequest) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->engineShutDownPeriod = 3;
	auto* limp = getLimpManager();

	ASSERT_FALSE(limp->shutdownController.isEngineStop(getTimeNowNt()));
	limp->updateState(0, getTimeNowNt());
	ASSERT_TRUE(limp->allowInjection());

	doScheduleStopEngine(StopRequestedReason::Lua);
	eth.moveTimeForwardMs(100);
	limp->updateState(0, getTimeNowNt());
	ASSERT_TRUE(limp->shutdownController.isEngineStop(getTimeNowNt()));
	ASSERT_EQ(ClearReason::StopRequested, limp->allowInjection().reason);
	engine->rpmCalculator.lastTdcTimer.reset();
	engine->triggerCentral.m_lastEventTimer.reset();
	ASSERT_FALSE(engine->rpmCalculator.checkIfSpinning(getTimeNowNt()));

	testLuaExecString("startCrankingEngine()");
	EXPECT_TRUE(enginePins.starterControl.getLogicValue());
	limp->updateState(0, getTimeNowNt());
	// A fresh start cancels the old stop window for both fuel and RPM detection.
	EXPECT_FALSE(limp->shutdownController.isEngineStop(getTimeNowNt()));
	EXPECT_TRUE(limp->allowInjection());
	EXPECT_TRUE(engine->rpmCalculator.checkIfSpinning(getTimeNowNt()));

	// Cancellation is not a lasting override: a subsequent stop takes effect immediately.
	doScheduleStopEngine(StopRequestedReason::Lua);
	limp->updateState(0, getTimeNowNt());
	EXPECT_TRUE(limp->shutdownController.isEngineStop(getTimeNowNt()));
	EXPECT_EQ(ClearReason::StopRequested, limp->allowInjection().reason);
}

TEST_F(StartStopTest, stopWindowExpiresAndRestarts) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->engineShutDownPeriod = 3;
	auto& shutdown = getLimpManager()->shutdownController;
	ASSERT_FALSE(shutdown.isEngineStop(getTimeNowNt()));

	shutdown.stopEngine(StopRequestedReason::Console);
	eth.moveTimeForwardMs(2999);
	EXPECT_TRUE(shutdown.isEngineStop(getTimeNowNt()));
	eth.moveTimeForwardMs(1);
	EXPECT_FALSE(shutdown.isEngineStop(getTimeNowNt()));

	shutdown.stopEngine(StopRequestedReason::TsCommand);
	eth.moveTimeForwardSec(2);
	shutdown.stopEngine(StopRequestedReason::Lua);
	eth.moveTimeForwardMs(2999);
	EXPECT_TRUE(shutdown.isEngineStop(getTimeNowNt()));
	eth.moveTimeForwardMs(1);
	EXPECT_FALSE(shutdown.isEngineStop(getTimeNowNt()));
}

TEST_F(StartStopTest, repeatedLuaStartPreservesTimeoutAndNewStop) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->engineShutDownPeriod = 3;
	engineConfiguration->startCrankingDuration = 3;
	engineConfiguration->startButtonSuppressOnStartUpMs = 0;
	Sensor::setMockValue(SensorType::BatteryVoltage, 15);
	slowStartStopButtonCallback();

	testLuaExecString("startCrankingEngine()");
	ASSERT_TRUE(enginePins.starterControl.getLogicValue());
	auto initialStart = engine->startStopState.startStopStateLastPush.get();
	eth.moveTimeForwardSec(1);
	doScheduleStopEngine(StopRequestedReason::Lua);
	testLuaExecString("startCrankingEngine()");
	EXPECT_EQ(initialStart, engine->startStopState.startStopStateLastPush.get());
	EXPECT_TRUE(getLimpManager()->shutdownController.isEngineStop(getTimeNowNt()));

	eth.moveTimeForwardMs(2001);
	slowStartStopButtonCallback();
	EXPECT_FALSE(enginePins.starterControl.getLogicValue());
	ASSERT_TRUE(getLimpManager()->shutdownController.isEngineStop(getTimeNowNt()));

	// Once the starter disengages, a fresh request can cancel the remaining window.
	testLuaExecString("startCrankingEngine()");
	EXPECT_TRUE(enginePins.starterControl.getLogicValue());
	EXPECT_FALSE(getLimpManager()->shutdownController.isEngineStop(getTimeNowNt()));
}

TEST_F(StartStopTest, buttonStartCancelsStopRequest) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->engineShutDownPeriod = 3;
	doScheduleStopEngine(StopRequestedReason::Console);
	ASSERT_TRUE(getLimpManager()->shutdownController.isEngineStop(getTimeNowNt()));
	ASSERT_TRUE(engine->rpmCalculator.isStopped());

	startStopButtonToggle();
	EXPECT_TRUE(enginePins.starterControl.getLogicValue());
	EXPECT_FALSE(getLimpManager()->shutdownController.isEngineStop(getTimeNowNt()));
}

TEST_F(StartStopTest, luaStartPreservesOtherFuelCuts) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->rpmHardLimit = 2500;
	engineConfiguration->cutFuelOnHardLimit = true;
	engineConfiguration->engineShutDownPeriod = 3;
	auto* limp = getLimpManager();
	doScheduleStopEngine(StopRequestedReason::TsCommand);
	ASSERT_TRUE(limp->shutdownController.isEngineStop(getTimeNowNt()));

	testLuaExecString("startCrankingEngine()");
	limp->updateState(3000, getTimeNowNt());
	EXPECT_FALSE(limp->shutdownController.isEngineStop(getTimeNowNt()));
	EXPECT_FALSE(limp->allowInjection());
	EXPECT_EQ(ClearReason::HardLimit, limp->allowInjection().reason);
}

TEST(start, startStop) {
	std::unordered_map<SensorType, float> sensorVals = {{ SensorType::AcceleratorPedal, 0 }};
	EngineTestHelper eth(engine_type_e::PROTEUS_BMW_M73, sensorVals);

	Sensor::setMockValue(SensorType::BatteryVoltage, 15);
	eth.moveTimeForwardAndInvokeEventsSec(1); // '0' time has special meaning for implementation so let's move forward

	// pull-up means inverted value
	ASSERT_TRUE(engine->startStopState.startStopButtonDebounce.readPinState());
	// this is a pull-up, so 'true' on start-up
	setMockState(engineConfiguration->startStopButtonPin, true);
	// remember about debounce?
	ASSERT_TRUE(engine->startStopState.startStopButtonDebounce.readPinState());
	eth.moveTimeForwardAndInvokeEventsSec(10);
	ASSERT_FALSE(engine->startStopState.startStopButtonDebounce.readPinState());

	ASSERT_FALSE(efiReadPin(engineConfiguration->starterControlPin));

	slowStartStopButtonCallback();
	ASSERT_FALSE(efiReadPin(engineConfiguration->starterControlPin));

	// startup 'timeout' duration of time is a special case so let's sleep a bit
	eth.moveTimeForwardAndInvokeEventsSec(10);
	slowStartStopButtonCallback();
	ASSERT_FALSE(efiReadPin(engineConfiguration->starterControlPin));

	eth.moveTimeForwardAndInvokeEventsSec(10);
	// hit 'start' button! inverted since pull-up
	setMockState(engineConfiguration->startStopButtonPin, false);
	slowStartStopButtonCallback();
	ASSERT_TRUE(efiReadPin(engineConfiguration->starterControlPin));

	eth.moveTimeForwardAndInvokeEventsSec(5);
	slowStartStopButtonCallback();

	eth.moveTimeForwardAndInvokeEventsSec(5);
	slowStartStopButtonCallback();

	eth.moveTimeForwardAndInvokeEventsSec(5);
	slowStartStopButtonCallback();
	// starter is now OFF due to timeout
	ASSERT_FALSE(efiReadPin(engineConfiguration->starterControlPin));
}
