/*
 * @file test_idle_controller.cpp
 *
 * @date Oct 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "efi_pid.h"
#include "idle_thread.h"
#include "electronic_throttle.h"

using ::testing::StrictMock;
using ::testing::_;

using ICP = IIdleController::Phase;
using TgtInfo = IIdleController::TargetInfo;

TEST(idle_v2, timingPid) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;

	engineConfiguration->useIdleTimingPidControl = true;

	engineConfiguration->idleTimingPid.pFactor = 0.1;
	engineConfiguration->idleTimingPid.minValue = -10;
	engineConfiguration->idleTimingPid.maxValue = 10;
	engineConfiguration->idleTimingSoftEntryTime = 0.0f;
	dut.init();

	// Check that out of idle mode it doesn't do anything
	EXPECT_EQ(0, dut.getIdleTimingAdjustment(1050, 1000, ICP::Cranking));
	EXPECT_EQ(0, dut.getIdleTimingAdjustment(1050, 1000, ICP::Coasting));
	EXPECT_EQ(0, dut.getIdleTimingAdjustment(1050, 1000, ICP::Running));

	// Check that it works in idle mode
	EXPECT_FLOAT_EQ(-5, dut.getIdleTimingAdjustment(1050, 1000, ICP::Idling));

	// ...but not when disabled
	engineConfiguration->useIdleTimingPidControl = false;
	EXPECT_EQ(0, dut.getIdleTimingAdjustment(1050, 1000, ICP::Idling));

	engineConfiguration->useIdleTimingPidControl = true;

	EXPECT_FLOAT_EQ(5,    dut.getIdleTimingAdjustment(950,  1000, ICP::Idling));
	EXPECT_FLOAT_EQ(2.5,  dut.getIdleTimingAdjustment(975,  1000, ICP::Idling));
	EXPECT_FLOAT_EQ(0,    dut.getIdleTimingAdjustment(1000, 1000, ICP::Idling));
	EXPECT_FLOAT_EQ(-2.5, dut.getIdleTimingAdjustment(1025, 1000, ICP::Idling));
	EXPECT_FLOAT_EQ(-5,   dut.getIdleTimingAdjustment(1050, 1000, ICP::Idling));
}

TEST(idle_v2, testTargetRpm) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;

	for (size_t i = 0; i < efi::size(config->cltIdleRpmBins); i++) {
		config->cltIdleRpmBins[i] = i * 10;
		config->cltIdleRpm[i] = i * 100;
	}

	engineConfiguration->idlePidRpmUpperLimit = 50;
	EXPECT_EQ((TgtInfo{100, 150, 175}), dut.getTargetRpm(10));
 	EXPECT_EQ((TgtInfo{500, 550, 575}), dut.getTargetRpm(50));
 
 	engineConfiguration->idlePidRpmUpperLimit = 73;
	EXPECT_EQ((TgtInfo{100, 173, 209.5}), dut.getTargetRpm(10));
 	EXPECT_EQ((TgtInfo{500, 573, 609.5}), dut.getTargetRpm(50));
}

TEST(idle_v2, testDeterminePhase) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;

	// TPS threshold 5% for easy test
	engineConfiguration->idlePidDeactivationTpsThreshold = 5;

	// Max VSS for idle is 10kph
	engineConfiguration->maxIdleVss = 10;

  TgtInfo targetInfo;
 	// Phase determination should ignore this!
 	targetInfo.ClosedLoopTarget = 9999;
 
 	// Idling threshold is 1000 + 100 rpm
 	targetInfo.IdleEntryRpm = 1000 + 100;
  targetInfo.IdleExitRpm = 1000 + 100;

	// First test stopped engine
	engine->rpmCalculator.setRpmValue(0);
	EXPECT_EQ(ICP::Cranking, dut.determinePhase(0, targetInfo, unexpected, 0, 10));

	// Now engine is running!
	// Controller doesn't need this other than for isCranking()
	engine->rpmCalculator.setRpmValue(1000);

	// Test invalid TPS, but inside the idle window
	EXPECT_EQ(ICP::Running, dut.determinePhase(1000, targetInfo, unexpected, 0, 10));

	// Valid TPS should now be inside the zone
	EXPECT_EQ(ICP::Idling, dut.determinePhase(1000, targetInfo, 0, 0, 10));

	// Inside the zone, but vehicle speed too fast
	EXPECT_EQ(ICP::Running, dut.determinePhase(1000, targetInfo, 0, 25, 10));

	// Check that shortly after cranking, the cranking taper inhibits closed loop idle
	EXPECT_EQ(ICP::CrankToIdleTaper, dut.determinePhase(1000, targetInfo, 0, 0, 0.5f));

	// Above TPS threshold should be outside the zone
	EXPECT_EQ(ICP::Running, dut.determinePhase(1000, targetInfo, 10, 0, 10));

	// Above target, below (target + upperLimit) should be in idle zone
	EXPECT_EQ(ICP::Idling, dut.determinePhase(1099, targetInfo, 0, 0, 10));

	// above upper limit and on throttle should be out of idle zone
	EXPECT_EQ(ICP::Running, dut.determinePhase(1101, targetInfo, 10, 0, 10));

	// Below TPS but above RPM should be outside the zone
	EXPECT_EQ(ICP::Coasting, dut.determinePhase(1101, targetInfo, 0, 0, 10));
	EXPECT_EQ(ICP::Coasting, dut.determinePhase(5000, targetInfo, 0, 0, 10));
}

TEST(idle_v2, crankingOpenLoop) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;


	for (size_t i = 0; i < efi::size(config->cltCrankingCorrBins); i++) {
		config->cltCrankingCorrBins[i] = i * 10;
		// 50 as base idle value
		config->cltCrankingCorr[i] = 50 * i * 0.1f;

		// different values in running so we can tell which one is used
		config->cltIdleCorrBins[i] = i * 10;
		config->cltIdleCorrTable[0][i] = i * 0.2f;
		config->cltIdleCorrTable[1][i] = i * 0.2f;
	}

	EXPECT_FLOAT_EQ(5, dut.getCrankingOpenLoop(10));
	EXPECT_FLOAT_EQ(25, dut.getCrankingOpenLoop(50));
}

TEST(idle_v2, runningOpenLoopBasic) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;


	for (size_t i = 0; i < efi::size(config->cltIdleCorrBins); i++) {
		config->cltIdleCorrBins[i] = i * 10;
		config->cltIdleCorrTable[0][i] = 50 * (i * 0.1f);
		config->cltIdleCorrTable[1][i] = 50 * (i * 0.1f);
	}

	EXPECT_FLOAT_EQ(5, dut.getRunningOpenLoop(IIdleController::Phase::Cranking, 0, 10, 0));
	EXPECT_FLOAT_EQ(25, dut.getRunningOpenLoop(IIdleController::Phase::Cranking, 0, 50, 0));
}

TEST(idle_v2, runningFanAcBump) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;

	setArrayValues(config->acIdleAdderByPressure, 9);
	engineConfiguration->fan1ExtraIdle = 7;
	engineConfiguration->fan2ExtraIdle = 3;

	setTable(config->cltIdleCorrTable, 50.0f);

	// Start with fan off
	enginePins.fanRelay.setValue(0);

	// Should be base position
	EXPECT_FLOAT_EQ(50, dut.getRunningOpenLoop(IIdleController::Phase::Cranking, 0, 10, 0));

	// Turn on AC!
	engine->module<AcController>()->acButtonState = true;
	EXPECT_FLOAT_EQ(50 + 9, dut.getRunningOpenLoop(IIdleController::Phase::Idling, 0, 10, 0));
	engine->module<AcController>()->acButtonState = false;

	// Turn the fan on!
	enginePins.fanRelay.setValue(1);
	EXPECT_FLOAT_EQ(50 + 7, dut.getRunningOpenLoop(IIdleController::Phase::Idling, 0, 10, 0));
	enginePins.fanRelay.setValue(0);

	// Turn on the other fan!
	enginePins.fanRelay2.setValue(1);
	EXPECT_FLOAT_EQ(50 + 3, dut.getRunningOpenLoop(IIdleController::Phase::Idling, 0, 10, 0));

	// Turn on everything!
	engine->module<AcController>()->acButtonState = true;
	enginePins.fanRelay.setValue(1);
	enginePins.fanRelay2.setValue(1);
	EXPECT_FLOAT_EQ(50 + 9 + 7 + 3, dut.getRunningOpenLoop(IIdleController::Phase::Idling, 0, 10, 0));
}

TEST(idle_v2, parkNeutralOffset) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& dut = engine->module<IdleController>().unmock();
	setTable(config->cltIdleCorrTable, 50.0f);
	setArrayValues(config->cltCrankingCorr, 30.0f);

	// Older tunes and scripts that don't report P/N retain their idle position.
	dut.setParkNeutral(true);
	EXPECT_FLOAT_EQ(50, dut.getOpenLoop(ICP::Idling, 800, 80, 0, 1));
	engineConfiguration->idleParkNeutralOffset = -12.5f;
	dut.setParkNeutral(false);
	EXPECT_FLOAT_EQ(50, dut.getOpenLoop(ICP::Idling, 800, 80, 0, 1));

	dut.setParkNeutral(true);
	EXPECT_FLOAT_EQ(37.5f, dut.getOpenLoop(ICP::Idling, 800, 80, 0, 1));
	// The modifier leaves cranking alone and blends in during the cranking taper.
	EXPECT_FLOAT_EQ(30, dut.getOpenLoop(ICP::Cranking, 200, 80, 0, 0));
	EXPECT_FLOAT_EQ(33.75f, dut.getOpenLoop(ICP::CrankToIdleTaper, 800, 80, 0, 0.5f));

	// Calibration edits take effect even without a gear transition.
	engineConfiguration->idleParkNeutralOffset = 7.5f;
	dut.luaAdd = 2;
	EXPECT_FLOAT_EQ(59.5f, dut.getOpenLoop(ICP::Idling, 800, 80, 0, 1));
	engineConfiguration->idleParkNeutralOffset = -100;
	EXPECT_FLOAT_EQ(0, dut.getOpenLoop(ICP::Idling, 800, 80, 0, 1));
	engineConfiguration->idleParkNeutralOffset = 100;
	EXPECT_FLOAT_EQ(100, dut.getOpenLoop(ICP::Idling, 800, 80, 0, 1));

	dut.setParkNeutral(false);
	EXPECT_FLOAT_EQ(52, dut.getOpenLoop(ICP::Idling, 800, 80, 0, 1));
	dut.setParkNeutral(true);
	engine->resetLua();
	EXPECT_FLOAT_EQ(50, dut.getOpenLoop(ICP::Idling, 800, 80, 0, 1));
}

// This can be seen as a kind of some close-loop logic, please read:
// https://github.com/rusefi/rusefi/issues/6977
TEST(idle_v2, idleAdderShouldNotAffectNonIdleAreas) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;

	setArrayValues(config->acIdleAdderByPressure, 9);

	setTable(config->cltIdleCorrTable, 50.0f);

	// Should be base position
	EXPECT_FLOAT_EQ(50, dut.getRunningOpenLoop(IIdleController::Phase::Cranking, 0, 10, 0));

	// [A/C ON && Phase::Cranking] => should be equal to base time
	engine->module<AcController>()->acButtonState = true;
	EXPECT_FLOAT_EQ(50, dut.getRunningOpenLoop(IIdleController::Phase::Cranking, 0, 10, 0));

	// [A/C ON && Phase::Running] => should be equal to base time plus a/c extra offset
	EXPECT_FLOAT_EQ(50 + 9, dut.getRunningOpenLoop(IIdleController::Phase::Idling, 0, 10, 0));
}

TEST(idle_v2, runningOpenLoopTpsTaper) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;

	// Zero out base tempco table
	setTable(config->cltIdleCorrTable, 0.0f);

	// Add 50% idle position
	engineConfiguration->iacByTpsTaper = 50;
	// At 10% TPS
	engineConfiguration->idlePidDeactivationTpsThreshold = 10;

	// Check in-bounds points
	EXPECT_FLOAT_EQ(0, dut.getRunningOpenLoop(IIdleController::Phase::Cranking, 0, 0, 0));
	EXPECT_FLOAT_EQ(25, dut.getRunningOpenLoop(IIdleController::Phase::Cranking, 0, 0, 5));
	EXPECT_FLOAT_EQ(50, dut.getRunningOpenLoop(IIdleController::Phase::Cranking, 0, 0, 10));

	// Check out of bounds - shouldn't leave the interval [0, 10]
	EXPECT_FLOAT_EQ(0, dut.getRunningOpenLoop(IIdleController::Phase::Cranking, 0, 0, -5));
	EXPECT_FLOAT_EQ(50, dut.getRunningOpenLoop(IIdleController::Phase::Cranking, 0, 0, 20));
}

TEST(idle_v2, runningOpenLoopTpsTaperWithDashpot) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;

	// Zero out base tempco table
	setTable(config->cltIdleCorrTable, 0.0f);

	// Add 50% idle position
	engineConfiguration->iacByTpsTaper = 50;
	// At 10% TPS
	engineConfiguration->idlePidDeactivationTpsThreshold = 10;

	// set hold and decay time
	engineConfiguration->iacByTpsHoldTime = 10;	// 10 secs
	engineConfiguration->iacByTpsDecayTime = 10;	// 10 secs

	// save the lastTimeRunningUs time - let it be the start of the hold phase
	advanceTimeUs(5'000'000);
	// full throttle = max.iac
	EXPECT_FLOAT_EQ(50, dut.getRunningOpenLoop(ICP::Running, 0, 0, 100));

	// jump to the end of the 'hold' phase of dashpot
	advanceTimeUs(10'000'000);

	// change the state to idle (release the pedal) - but still 100% max.iac!
    EXPECT_FLOAT_EQ(50, dut.getRunningOpenLoop(ICP::Idling, 0, 0, 0));
    // now we're in the middle of decay
    advanceTimeUs(5'000'000);
    // 50% decay (50% of 50 is 25)
    EXPECT_FLOAT_EQ(25, dut.getRunningOpenLoop(ICP::Idling, 0, 0, 0));
    // now the decay is finished
    advanceTimeUs(5'000'000);
    // no correction
    EXPECT_FLOAT_EQ(0, dut.getRunningOpenLoop(ICP::Idling, 0, 0, 0));
    // still react to the pedal
    EXPECT_FLOAT_EQ(50, dut.getRunningOpenLoop(ICP::Idling, 0, 0, 10));
}

struct MockOpenLoopIdler : public IdleController {
	MOCK_METHOD(float, getCrankingOpenLoop, (float clt), (const, override));
	MOCK_METHOD(float, getRunningOpenLoop, (IIdleController::Phase phase, float rpm, float clt, SensorResult tps), (override));
};

TEST(idle_v2, testOpenLoopCranking) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	StrictMock<MockOpenLoopIdler> dut;

	EXPECT_CALL(dut, getCrankingOpenLoop(30)).WillOnce(Return(44));

	// Should return the value from getCrankingOpenLoop, and ignore running numbers
	EXPECT_FLOAT_EQ(44, dut.getOpenLoop(ICP::Cranking, 0, 30, 0, 0));
}

TEST(idle_v2, openLoopRunningTaper) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	StrictMock<MockOpenLoopIdler> dut;

	EXPECT_CALL(dut, getRunningOpenLoop(ICP::CrankToIdleTaper, 0, 30, SensorResult(0))).WillRepeatedly(Return(25));
	EXPECT_CALL(dut, getRunningOpenLoop(ICP::Running, 0, 30, SensorResult(0))).WillRepeatedly(Return(25));
	EXPECT_CALL(dut, getCrankingOpenLoop(30)).WillRepeatedly(Return(75));

	// 0 cycles - no taper yet, pure cranking value
	EXPECT_FLOAT_EQ(75, dut.getOpenLoop(ICP::Running, 0, 30, 0, 0));
	EXPECT_FLOAT_EQ(75, dut.getOpenLoop(ICP::CrankToIdleTaper, 0, 30, 0, 0));

	// 1/2 taper - half way, 50% each value -> outputs 50
	EXPECT_FLOAT_EQ(50, dut.getOpenLoop(ICP::Running, 0, 30, 0, 0.5f));
	EXPECT_FLOAT_EQ(50, dut.getOpenLoop(ICP::CrankToIdleTaper, 0, 30, 0, 0.5f));

	// 1x taper - fully tapered, should be running value
	EXPECT_FLOAT_EQ(25, dut.getOpenLoop(ICP::Running, 0, 30, 0, 1.0f));
	EXPECT_FLOAT_EQ(25, dut.getOpenLoop(ICP::CrankToIdleTaper, 0, 30, 0, 1.0f));

	// 2x taper - still fully tapered, should be running value
	EXPECT_FLOAT_EQ(25, dut.getOpenLoop(ICP::Running, 0, 30, 0, 2.0f));
	EXPECT_FLOAT_EQ(25, dut.getOpenLoop(ICP::CrankToIdleTaper, 0, 30, 0, 2.0f));
}

TEST(idle_v2, getCrankingTaperFraction) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	StrictMock<MockOpenLoopIdler> dut;
	const float mockedTemperature = 50;

	setArrayValues(config->afterCrankingIACtaperDuration, 500);

	// 0 cycles - no taper yet, pure cranking value
	EXPECT_FLOAT_EQ(0, dut.getCrankingTaperFraction(mockedTemperature));

	// 250 cycles - half way, 50% each value -> outputs 50
	for (size_t i = 0; i < 250; i++) {
		engine->rpmCalculator.onNewEngineCycle();
	}
	EXPECT_FLOAT_EQ(0.5f, dut.getCrankingTaperFraction(mockedTemperature));

	// 500 cycles - fully tapered, should be running value
	for (size_t i = 0; i < 250; i++) {
		engine->rpmCalculator.onNewEngineCycle();
	}
	EXPECT_FLOAT_EQ(1, dut.getCrankingTaperFraction(mockedTemperature));

	// 1000 cycles - still fully tapered, should be running value
	for (size_t i = 0; i < 500; i++) {
		engine->rpmCalculator.onNewEngineCycle();
	}
	EXPECT_FLOAT_EQ(2, dut.getCrankingTaperFraction(mockedTemperature));
}

TEST(idle_v2, openLoopCoastingTable) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;

	// enable & configure feature
	engineConfiguration->useIacTableForCoasting = true;
	for (size_t i = 0; i < CLT_CURVE_SIZE; i++) {
		config->iacCoastingRpmBins[i] = 100 * i;
		config->iacCoasting[i] = 5 * i;
	}

	EXPECT_FLOAT_EQ(40, dut.getOpenLoop(ICP::Coasting, 800, 0, 0, 2));
	EXPECT_FLOAT_EQ(75, dut.getOpenLoop(ICP::Coasting, 1500, 0, 0, 2));
}

TEST(idle_v2, closedLoopBasic) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;
	dut.init();

	// Not testing PID here, so we can set very simple PID gains
	engineConfiguration->idleRpmPid.pFactor = 0.5;	// 0.5 output per 1 RPM error = 50% per 100 rpm
	engineConfiguration->idleRpmPid.iFactor = 0;
	engineConfiguration->idleRpmPid.dFactor = 0;
	engineConfiguration->idleRpmPid.iFactor = 0;
	engineConfiguration->idleRpmPid.periodMs = 0;
	engineConfiguration->idleRpmPid.minValue = -50;
	engineConfiguration->idleRpmPid.maxValue = 50;

	engineConfiguration->idlePidRpmDeadZone = 0;

	// burn one update then advance time 5 seconds to avoid difficulty from wasResetPid
	dut.getClosedLoop(ICP::Idling, 0, 900, 900);
	advanceTimeUs(5'000'000);

	// Test above target, should return negative
	EXPECT_FLOAT_EQ(-25, dut.getClosedLoop(ICP::Idling, 0, /*rpm*/ 950, /*tgt*/ 900));

	// Below target, should return positive
	EXPECT_FLOAT_EQ(25, dut.getClosedLoop(ICP::Idling, 0, /*rpm*/ 850, /*tgt*/ 900));
}

TEST(idle_v2, closedLoopDeadzone) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;
	dut.init();


	// Not testing PID here, so we can set very simple PID gains
	engineConfiguration->idleRpmPid.pFactor = 0.5;	// 0.5 output per 1 RPM error = 50% per 100 rpm
	engineConfiguration->idleRpmPid.iFactor = 0;
	engineConfiguration->idleRpmPid.dFactor = 0;
	engineConfiguration->idleRpmPid.iFactor = 0;
	engineConfiguration->idleRpmPid.periodMs = 0;
	engineConfiguration->idleRpmPid.minValue = -50;
	engineConfiguration->idleRpmPid.maxValue = 50;

	engineConfiguration->idlePidRpmDeadZone = 25;

	// burn one then advance time 5 seconds to avoid difficulty from wasResetPid
	dut.getClosedLoop(ICP::Idling, 0, 900, 900);
	advanceTimeUs(5'000'000);

	// Test above target, should return negative
	EXPECT_FLOAT_EQ(-25, dut.getClosedLoop(ICP::Idling, 0, /*rpm*/ 950, /*tgt*/ 900));

	// Inside deadzone, should return same as last time
	EXPECT_FLOAT_EQ(-25, dut.getClosedLoop(ICP::Idling, 0, /*rpm*/ 900, /*tgt*/ 900));
}

TEST(idle_v2, RunningToIdleTransition) {
  EngineTestHelper eth(engine_type_e::TEST_ENGINE);
  IdleController dut;
  dut.init();

  engineConfiguration->idleRpmPid.pFactor = 0.0040;	// 0.5 output per 1 RPM error = 50% per 100 rpm
  engineConfiguration->idleRpmPid.iFactor = 0.0040;
  engineConfiguration->idleRpmPid.dFactor = 0.0001;
  engineConfiguration->idleRpmPid.periodMs = 0;
  engineConfiguration->idleRpmPid.minValue = -50;
  engineConfiguration->idleRpmPid.maxValue = 50;

  SensorResult expectedTps = 0;
  float expectedClt = 37;
  Sensor::setMockValue(SensorType::DriverThrottleIntent, expectedTps.Value);
  Sensor::setMockValue(SensorType::Clt, expectedClt);
  Sensor::setMockValue(SensorType::VehicleSpeed, 15.0);

  // we are on running state still, so 0 idle position
  EXPECT_EQ(0, dut.getClosedLoop(ICP::Running, expectedTps.Value, 950, 1100));
  dut.getIdlePid()->postState(engine->outputChannels.idleStatus);

  EXPECT_EQ(0, engine->outputChannels.idleStatus.dTerm);
  EXPECT_EQ(0, engine->outputChannels.idleStatus.iTerm);
  EXPECT_EQ(0, engine->outputChannels.idleStatus.pTerm);

  // now we are idling
  dut.getClosedLoop(ICP::Idling, expectedTps.Value, 950, 1100);
  advanceTimeUs(5'000'000);

  EXPECT_NEAR(3.603, dut.getClosedLoop(ICP::Idling, expectedTps.Value, 950, 1100), EPS2D);
  dut.getIdlePid()->postState(engine->outputChannels.idleStatus);

  EXPECT_NEAR(3, engine->outputChannels.idleStatus.dTerm, EPS2D);
  EXPECT_NEAR(0, engine->outputChannels.idleStatus.iTerm, EPS2D);
  EXPECT_NEAR(0.6, engine->outputChannels.idleStatus.pTerm, EPS2D);

  // still idle, add some error:
  EXPECT_NEAR(1.086, dut.getClosedLoop(ICP::Idling, expectedTps.Value, 950, 1120), EPS2D);
  dut.getIdlePid()->postState(engine->outputChannels.idleStatus);

  EXPECT_NEAR(0.4, engine->outputChannels.idleStatus.dTerm, EPS2D);
  EXPECT_NEAR(0.01, engine->outputChannels.idleStatus.iTerm, EPS2D);
  EXPECT_NEAR(0.68, engine->outputChannels.idleStatus.pTerm, EPS2D);

  // back to running mode, should reset all:
  EXPECT_EQ(0, dut.getClosedLoop(ICP::Running, expectedTps.Value, 950, 1100));
  dut.getIdlePid()->postState(engine->outputChannels.idleStatus);

  // first cycle we set shouldResetPid, now we test the reset:
  EXPECT_EQ(0, dut.getClosedLoop(ICP::Running, expectedTps.Value, 400, 1100));
  dut.getIdlePid()->postState(engine->outputChannels.idleStatus);

  EXPECT_NEAR(0, engine->outputChannels.idleStatus.dTerm, EPS2D);
  EXPECT_NEAR(0, engine->outputChannels.idleStatus.iTerm, EPS2D);
  EXPECT_NEAR(0, engine->outputChannels.idleStatus.pTerm, EPS2D);
}

struct IntegrationIdleMock : public IdleController {
	MOCK_METHOD(TargetInfo, getTargetRpm, (float clt), (override));
 	MOCK_METHOD(ICP, determinePhase, (float rpm, TargetInfo targetRpm, SensorResult tps, float vss, float crankingTaperFraction), (override));
	MOCK_METHOD(float, getOpenLoop, (ICP phase, float rpm, float clt, SensorResult tps, float crankingTaperFraction), (override));
	MOCK_METHOD(float, getClosedLoop, (ICP phase, float tps, float rpm, float target), (override));
	MOCK_METHOD(float, getCrankingTaperFraction, (float clt), (const, override));
};

TEST(idle_v2, IntegrationManual) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	StrictMock<IntegrationIdleMock> dut;

	SensorResult expectedTps = 1;
	float expectedClt = 37;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, expectedTps.Value);
	Sensor::setMockValue(SensorType::Clt, expectedClt);
	Sensor::setMockValue(SensorType::VehicleSpeed, 15.0);

  TgtInfo target{1000, 1100, 1100};

	// Target of 1000 rpm
	EXPECT_CALL(dut, getTargetRpm(expectedClt))
		.WillOnce(Return(target));

	// 30% of the way through cranking taper
	EXPECT_CALL(dut, getCrankingTaperFraction(expectedClt))
		.WillOnce(Return(0.3f));

	// Determine phase will claim we're idling
	EXPECT_CALL(dut, determinePhase(950, target, expectedTps, 15, 0.3f))
		.WillOnce(Return(ICP::Idling));

	// Open loop should be asked for an open loop position
	EXPECT_CALL(dut, getOpenLoop(ICP::Idling, 950, expectedClt, expectedTps, 0.3f))
		.WillOnce(Return(13));

	// getClosedLoop() should not be called!

	EXPECT_EQ(13, dut.getIdlePosition(950));
}

TEST(idle_v2, IntegrationAutomatic) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	StrictMock<IntegrationIdleMock> dut;

	engineConfiguration->idleMode = idle_mode_e::IM_AUTO;

	SensorResult expectedTps = 1;
	float expectedClt = 37;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, expectedTps.Value);
	Sensor::setMockValue(SensorType::Clt, expectedClt);
	Sensor::setMockValue(SensorType::VehicleSpeed, 15.0);
  
  TgtInfo target{1000, 1100, 1100};

	// Target of 1000 rpm
	EXPECT_CALL(dut, getTargetRpm(expectedClt))
		.WillOnce(Return(target));

	// 40% of the way through cranking taper
	EXPECT_CALL(dut, getCrankingTaperFraction(expectedClt))
		.WillOnce(Return(0.4f));

	// Determine phase will claim we're idling
	EXPECT_CALL(dut, determinePhase(950, target, expectedTps, 15, 0.4f))
		.WillOnce(Return(ICP::Idling));

	// Open loop should be asked for an open loop position
	EXPECT_CALL(dut, getOpenLoop(ICP::Idling, 950, expectedClt, expectedTps, 0.4f))
		.WillOnce(Return(13));

	// Closed loop should get called
	EXPECT_CALL(dut, getClosedLoop(ICP::Idling, expectedTps.Value, 950, 1000))
		.WillOnce(Return(7));

	// Result should be open + closed
	EXPECT_EQ(13 + 7, dut.getIdlePosition(950));
}

TEST(idle_v2, IntegrationClamping) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	StrictMock<IntegrationIdleMock> dut;

	engineConfiguration->idleMode = idle_mode_e::IM_AUTO;

	SensorResult expectedTps = 1;
	float expectedClt = 37;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, expectedTps.Value);
	Sensor::setMockValue(SensorType::Clt, expectedClt);
	Sensor::setMockValue(SensorType::VehicleSpeed, 15.0);
  TgtInfo target{1000, 1100, 1100};

	// Target of 1000 rpm
	EXPECT_CALL(dut, getTargetRpm(expectedClt))
		.WillOnce(Return(target));

	// 50% of the way through cranking taper
	EXPECT_CALL(dut, getCrankingTaperFraction(expectedClt))
		.WillOnce(Return(0.5f));

	// Determine phase will claim we're idling
	EXPECT_CALL(dut, determinePhase(950, target, expectedTps, 15, 0.5f))
		.WillOnce(Return(ICP::Idling));

	// Open loop should be asked for an open loop position
	EXPECT_CALL(dut, getOpenLoop(ICP::Idling, 950, expectedClt, expectedTps, 0.5f))
		.WillOnce(Return(75));

	// Closed loop should get called
	EXPECT_CALL(dut, getClosedLoop(ICP::Idling, expectedTps.Value, 950, 1000))
		.WillOnce(Return(75));

	// Result would be 75 + 75 = 150, but it should clamp to 100
	EXPECT_EQ(100, dut.getIdlePosition(950));
}


TEST(idle_v2, clutchOrNeutralOverridesVssGate) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;

	engineConfiguration->idlePidDeactivationTpsThreshold = 5;
	engineConfiguration->maxIdleVss = 10;

	TgtInfo targetInfo{};
	targetInfo.IdleEntryRpm = 1000 + 100;
	targetInfo.IdleExitRpm = 1000 + 100;

	engine->rpmCalculator.setRpmValue(1000);

	// Baseline: above maxIdleVss forces Running, override disabled (default).
	EXPECT_EQ(ICP::Running, dut.determinePhase(1000, targetInfo, 0, 25, 10));

	engineConfiguration->idleVssGateClutchOverride = true;

	// Override enabled, but still in gear with clutch engaged -- VSS gate still applies.
	Sensor::setMockValue(SensorType::DetectedGear, 3);
	EXPECT_EQ(ICP::Running, dut.determinePhase(1000, targetInfo, 0, 25, 10));

	// Neutral detected (Detected Gear == 0) -- override allows closed-loop idle despite high VSS.
	Sensor::setMockValue(SensorType::DetectedGear, 0);
	EXPECT_EQ(ICP::Idling, dut.determinePhase(1000, targetInfo, 0, 25, 10));

	// No gear signal, but the clutch switch itself reports disengaged -- override still applies.
	Sensor::resetMockValue(SensorType::DetectedGear);
	engineConfiguration->smUpshiftClutchSwitch = sm_clutch_switch_e::ClutchDown;
	engine->engineState.clutchDownState = true;
	EXPECT_EQ(ICP::Idling, dut.determinePhase(1000, targetInfo, 0, 25, 10));

	// Clutch released again (pedal at rest) -- VSS gate re-applies.
	engine->engineState.clutchDownState = false;
	EXPECT_EQ(ICP::Running, dut.determinePhase(1000, targetInfo, 0, 25, 10));
}

TEST(idle_v2, neutralOverrideStaysLatchedThroughTransientGearRematch) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;

	engineConfiguration->idlePidDeactivationTpsThreshold = 5;
	engineConfiguration->maxIdleVss = 10;
	engineConfiguration->idleVssGateClutchOverride = true;

	TgtInfo targetInfo{};
	targetInfo.IdleEntryRpm = 1000 + 100;
	targetInfo.IdleExitRpm = 1000 + 100;

	engine->rpmCalculator.setRpmValue(1000);

	// Neutral detected while RPM is sagging below entry -- override grants idle authority.
	Sensor::setMockValue(SensorType::DetectedGear, 0);
	EXPECT_EQ(ICP::Idling, dut.determinePhase(900, targetInfo, 0, 25, 10));

	// RPM sags further under closed-loop idle correction, still neutral.
	EXPECT_EQ(ICP::Idling, dut.determinePhase(700, targetInfo, 0, 25, 10));

	// RPM recovers and GearDetector's ratio briefly re-matches a gear again -- but RPM is
	// still well below IdleExitRpm, so the override must stay latched: this is the exact
	// transient that used to bounce back to Running before reaching a real exit.
	Sensor::setMockValue(SensorType::DetectedGear, 6);
	EXPECT_EQ(ICP::Idling, dut.determinePhase(1000, targetInfo, 0, 25, 10));

	// Gear flickers back to neutral -- still idling either way.
	Sensor::setMockValue(SensorType::DetectedGear, 0);
	EXPECT_EQ(ICP::Idling, dut.determinePhase(1050, targetInfo, 0, 25, 10));

	// Only once RPM genuinely climbs past IdleExitRpm does the override release -- at that point
	// the ordinary Coasting classification (checked before the VSS gate) already takes over.
	Sensor::setMockValue(SensorType::DetectedGear, 6);
	EXPECT_EQ(ICP::Coasting, dut.determinePhase(1101, targetInfo, 0, 25, 10));
	// Returning below entry in gear must not revive the old neutral latch.
	EXPECT_EQ(ICP::Running, dut.determinePhase(900, targetInfo, 0, 25, 10));
}

TEST(idle_v2, getCrankingTaperFractionWithHold) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	StrictMock<MockOpenLoopIdler> dut;
	const float mockedTemperature = 50;

	// Hold for 200 cycles before the 500-cycle taper begins counting down.
	setArrayValues(config->afterCrankingIACtaperHoldDuration, 200);
	setArrayValues(config->afterCrankingIACtaperDuration, 500);

	// 0 cycles - within hold, pinned at (or below) cranking value
	EXPECT_LE(dut.getCrankingTaperFraction(mockedTemperature), 0);

	// 100 cycles - still within hold
	for (size_t i = 0; i < 100; i++) {
		engine->rpmCalculator.onNewEngineCycle();
	}
	EXPECT_LE(dut.getCrankingTaperFraction(mockedTemperature), 0);

	// 200 cycles - hold just elapsed, taper now starts from 0
	for (size_t i = 0; i < 100; i++) {
		engine->rpmCalculator.onNewEngineCycle();
	}
	EXPECT_FLOAT_EQ(0, dut.getCrankingTaperFraction(mockedTemperature));

	// 450 cycles - 250 cycles past the hold, halfway through the 500-cycle taper
	for (size_t i = 0; i < 250; i++) {
		engine->rpmCalculator.onNewEngineCycle();
	}
	EXPECT_FLOAT_EQ(0.5f, dut.getCrankingTaperFraction(mockedTemperature));

	// 700 cycles - 500 cycles past the hold, taper complete
	for (size_t i = 0; i < 250; i++) {
		engine->rpmCalculator.onNewEngineCycle();
	}
	EXPECT_FLOAT_EQ(1, dut.getCrankingTaperFraction(mockedTemperature));
}

static void setupOffIdleAdder(int adderRpm = 200, float stabilityThreshold = 100, float waitTime = 0.5f, float decayTime = 2.0f) {
	engineConfiguration->offIdleRpmAdder = adderRpm;
	engineConfiguration->offIdleRpmStabilityThreshold = stabilityThreshold;
	engineConfiguration->offIdleWaitTime = waitTime;
	engineConfiguration->offIdleRpmAdderDecayTime = decayTime;
}

TEST(idle_v2, offIdleAdder_waitTimerBeforeDecay) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;
	setupOffIdleAdder(200, 100, 0.5f, 2.0f);

	// Arm → Stabilizing → Waiting (stable RPM)
	dut.getOffIdleAdder(ICP::Running, 2000);
	dut.getOffIdleAdder(ICP::Idling, 1000);
	dut.getOffIdleAdder(ICP::Idling, 1000); // transitions to Waiting here

	// Not yet expired - still max
	EXPECT_FLOAT_EQ(200, dut.getOffIdleAdder(ICP::Idling, 1000));

	// Advance past the wait time
	advanceTimeUs(600'000); // 0.6s > 0.5s wait time

	// Should now be Decaying - first call returns max (transitions on this call)
	EXPECT_FLOAT_EQ(200, dut.getOffIdleAdder(ICP::Idling, 1000));
}

TEST(idle_v2, offIdleAdder_linearDecay) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;
	setupOffIdleAdder(200, 100, 0.0f, 2.0f); // no wait time

	// Arm → Stabilizing → Waiting (stable RPM, 0 wait) → Decaying
	dut.getOffIdleAdder(ICP::Running, 2000);
	dut.getOffIdleAdder(ICP::Idling, 1000);
	dut.getOffIdleAdder(ICP::Idling, 1000); // transitions to Waiting
	dut.getOffIdleAdder(ICP::Idling, 1000); // wait=0 → transitions to Decaying, returns max

	// At start of decay: full adder
	EXPECT_NEAR(200, dut.getOffIdleAdder(ICP::Idling, 1000), 5);

	// At ~1s: half adder
	advanceTimeUs(1'000'000);
	EXPECT_NEAR(100, dut.getOffIdleAdder(ICP::Idling, 1000), 5);

	// At 2s: zero, transitions to Inactive
	advanceTimeUs(1'000'000);
	EXPECT_FLOAT_EQ(0, dut.getOffIdleAdder(ICP::Idling, 1000));
}

TEST(idle_v2, offIdleAdder_rearmDuringDecay) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;
	setupOffIdleAdder(200, 100, 0.0f, 2.0f);

	// Get into Decaying state (same path as linearDecay)
	dut.getOffIdleAdder(ICP::Running, 2000);
	dut.getOffIdleAdder(ICP::Idling, 1000);
	dut.getOffIdleAdder(ICP::Idling, 1000);
	dut.getOffIdleAdder(ICP::Idling, 1000); // now Decaying
	advanceTimeUs(1'000'000); // decay is half-way

	// Mid-decay value should be ~100
	EXPECT_NEAR(100, dut.getOffIdleAdder(ICP::Idling, 1000), 10);

	// Re-arm by going off-idle again
	EXPECT_FLOAT_EQ(200, dut.getOffIdleAdder(ICP::Running, 3000));
	// Back to Armed - full max
	EXPECT_FLOAT_EQ(200, dut.getOffIdleAdder(ICP::Coasting, 1500));
}

TEST(idle_v2, offIdleAdder_abortsOnRecrankFromAnyState) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;
	setupOffIdleAdder(200, 100, 0.0f, 2.0f); // no wait time

	// Armed by Running, then engine stalls and is re-cranked mid-cycle: must not
	// carry the adder into Cranking.
	EXPECT_FLOAT_EQ(200, dut.getOffIdleAdder(ICP::Running, 2000));
	EXPECT_FLOAT_EQ(0, dut.getOffIdleAdder(ICP::Cranking, 300));

	// Get all the way into Decaying, then stall/re-crank: must still abort to zero.
	dut.getOffIdleAdder(ICP::Running, 2000);
	dut.getOffIdleAdder(ICP::Idling, 1000);
	dut.getOffIdleAdder(ICP::Idling, 1000); // transitions to Waiting
	dut.getOffIdleAdder(ICP::Idling, 1000); // wait=0 -> transitions to Decaying
	advanceTimeUs(1'000'000); // decay is half-way
	EXPECT_NEAR(100, dut.getOffIdleAdder(ICP::Idling, 1000), 10);

	EXPECT_FLOAT_EQ(0, dut.getOffIdleAdder(ICP::Cranking, 300));

	// Once cranking has reset the adder, resuming Idling must not resurrect the
	// stale decay - it should stay off until re-armed via Running/Coasting.
	EXPECT_FLOAT_EQ(0, dut.getOffIdleAdder(ICP::Idling, 1000));
}

TEST(idle_v2, acRpmAdditionAndPressureAir) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;
	setArrayValues(config->cltIdleRpm, 800);
	engineConfiguration->acIdleRpmAdder = 100;
	EXPECT_EQ(800, dut.getTargetRpm(80).ClosedLoopTarget);
	engine->module<AcController>().unmock().acButtonState = true;
	EXPECT_EQ(900, dut.getTargetRpm(80).ClosedLoopTarget);
	setArrayValues(config->cltIdleRpm, 1200);
	EXPECT_EQ(1300, dut.getTargetRpm(-20).ClosedLoopTarget);

	setTable(config->cltIdleCorrTable, 20);
	for (size_t i = 0; i < AC_IDLE_PRESSURE_CURVE_SIZE; i++) {
		config->acIdleAdderByPressureBins[i] = 100 + i * 100;
		config->acIdleAdderByPressure[i] = 5 + i * 10;
	}
	EXPECT_FLOAT_EQ(25, dut.getRunningOpenLoop(ICP::Idling, 800, 80, 0));
	Sensor::setMockValue(SensorType::AcPressure, 250);
	EXPECT_FLOAT_EQ(40, dut.getRunningOpenLoop(ICP::Idling, 800, 80, 0));
	EXPECT_FLOAT_EQ(40, dut.getRunningOpenLoop(ICP::CrankToIdleTaper, 800, 80, 0));
	EXPECT_FLOAT_EQ(20, dut.getRunningOpenLoop(ICP::Running, 2000, 80, 0));
	engineConfiguration->useIacTableForCoasting = true;
	setArrayValues(config->iacCoasting, 90);
	EXPECT_FLOAT_EQ(100, dut.getOpenLoop(ICP::Coasting, 2000, 80, 0, 1));
	Sensor::resetMockValue(SensorType::AcPressure);
	EXPECT_FLOAT_EQ(95, dut.getOpenLoop(ICP::Coasting, 2000, 80, 0, 1));
}

TEST(idle_v2, externalAirRequestsStackWithLuaAndParkNeutral) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;
	setTable(config->cltIdleCorrTable, 20);
	engineConfiguration->idleUpSwitchPins[0] = Gpio::A6;
	engineConfiguration->idleUpSwitchPins[1] = Gpio::A7;
	engineConfiguration->idleUpSwitchPins[2] = Gpio::A8;
	for (size_t i = 0; i < IDLE_UP_SWITCH_COUNT; i++) {
		engineConfiguration->idleUpAdder[i] = 10 * (i + 1);
	}
	engineConfiguration->idleUpSwitchMode[1] = PI_INVERTED_DEFAULT;
	setMockState(Gpio::A6, true);
	setMockState(Gpio::A7, true);
	setMockState(Gpio::A8, false);
	EXPECT_FLOAT_EQ(30, dut.getRunningOpenLoop(ICP::Idling, 800, 80, 0));
	setMockState(Gpio::A7, false);
	setMockState(Gpio::A8, true);
	dut.luaAdd = 5;
	dut.setParkNeutral(true);
	engineConfiguration->idleParkNeutralOffset = -10;
	EXPECT_FLOAT_EQ(75, dut.getRunningOpenLoop(ICP::Idling, 800, 80, 0));
	engineConfiguration->idleUpSwitchPins[2] = Gpio::Unassigned;
	EXPECT_FLOAT_EQ(45, dut.getRunningOpenLoop(ICP::Idling, 800, 80, 0));
}

TEST(idle_v2, optionalCrankingAirAndAdditiveFlare) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;
	setTable(config->cltIdleCorrTable, 25);
	setArrayValues(config->cltCrankingCorr, 50);
	ASSERT_TRUE(engineConfiguration->crankingAirAmountEnabled);
	EXPECT_FLOAT_EQ(50, dut.getOpenLoop(ICP::Cranking, 300, 80, 0, 0));
	engineConfiguration->crankingAirAmountEnabled = false;
	EXPECT_FLOAT_EQ(25, dut.getOpenLoop(ICP::Cranking, 300, 80, 0, 0));
	EXPECT_FLOAT_EQ(25, dut.getOpenLoop(ICP::CrankToIdleTaper, 900, 80, 0, 0.5f));
	engineConfiguration->crankingIdleRpmFlareEnabled = true;
	EXPECT_FLOAT_EQ(25, dut.getOpenLoop(ICP::Cranking, 300, 80, 0, 0));
	engineConfiguration->crankingAirAmountEnabled = true;
	EXPECT_FLOAT_EQ(75, dut.getOpenLoop(ICP::Cranking, 300, 80, 0, 0));
	EXPECT_FLOAT_EQ(50, dut.getOpenLoop(ICP::CrankToIdleTaper, 900, 80, 0, 0.5f));
	EXPECT_FLOAT_EQ(25, dut.getOpenLoop(ICP::Idling, 900, 80, 0, 2));
	setTable(config->cltIdleCorrTable, 80);
	EXPECT_FLOAT_EQ(100, dut.getOpenLoop(ICP::Cranking, 300, 80, 0, 0));
}

TEST(idle_v2, flareUsesFreshTargetAndAirTableWithoutClosedLoop) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;
	dut.init();
	engineConfiguration->crankingIdleRpmFlareEnabled = true;
	engineConfiguration->crankingAirAmountEnabled = false;
	engineConfiguration->idleMode = idle_mode_e::IM_AUTO;
	engineConfiguration->idlePidRpmUpperLimit = 300;
	setArrayValues(config->cltIdleRpm, 800);
	setArrayValues(config->cltCrankingRpmAdder, 700);
	setArrayValues(config->afterCrankingIACtaperDuration, 100);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);
	Sensor::setMockValue(SensorType::Clt, 80);
	for (size_t i = 0; i < efi::size(config->rpmIdleCorrBins); i++) {
		config->rpmIdleCorrBins[i] = 800 + 700 * i;
		setArrayValues(config->cltIdleCorrTable[i], 20 + 20 * i);
	}
	engine->rpmCalculator.setRpmValue(300);
	EXPECT_FLOAT_EQ(40, dut.getIdlePosition(300));
	EXPECT_EQ(1500, dut.idleTarget);
	EXPECT_EQ(1100, dut.idleEntryRpm);
	EXPECT_FALSE(dut.isIdleClosedLoop);
	EXPECT_FLOAT_EQ(40, dut.getIdlePosition(300));
	EXPECT_EQ(1500, dut.idleTarget);
	engine->rpmCalculator.setRpmValue(900);
	for (int i = 0; i < 50; i++) {
		engine->rpmCalculator.onNewEngineCycle();
	}
	EXPECT_FLOAT_EQ(30, dut.getIdlePosition(900));
	EXPECT_EQ(1150, dut.idleTarget);
	EXPECT_EQ(ICP::CrankToIdleTaper, dut.getCurrentPhase());
	EXPECT_FALSE(dut.isIdleClosedLoop);
}

TEST(idle_v2, zeroDurationTaperAndOffIdleReset) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;
	dut.init();
	setArrayValues(config->afterCrankingIACtaperHoldDuration, 1);
	setArrayValues(config->afterCrankingIACtaperDuration, 0);
	EXPECT_FLOAT_EQ(0, dut.getCrankingTaperFraction(80));
	engine->rpmCalculator.onNewEngineCycle();
	EXPECT_FLOAT_EQ(1, dut.getCrankingTaperFraction(80));
	setupOffIdleAdder(200, 100, 0, 0);
	EXPECT_FLOAT_EQ(0, dut.getOffIdleAdder(ICP::Idling, 800));
	dut.getOffIdleAdder(ICP::Running, 2000);
	dut.getOffIdleAdder(ICP::Idling, 800);
	dut.getOffIdleAdder(ICP::Idling, 800);
	dut.getOffIdleAdder(ICP::Idling, 800);
	EXPECT_FLOAT_EQ(0, dut.getOffIdleAdder(ICP::Idling, 800));
	dut.getOffIdleAdder(ICP::Running, 2000);
	dut.onEngineStop();
	EXPECT_FLOAT_EQ(0, dut.getOffIdleAdder(ICP::Idling, 800));
	dut.getOffIdleAdder(ICP::Running, 2000);
	dut.onConfigurationChange(nullptr);
	EXPECT_FLOAT_EQ(0, dut.getOffIdleAdder(ICP::Idling, 800));
	dut.getOffIdleAdder(ICP::Running, 2000);
	engineConfiguration->offIdleRpmAdder = 0;
	EXPECT_FLOAT_EQ(0, dut.getOffIdleAdder(ICP::Idling, 800));
}

TEST(idle_v2, neutralLatchClearsOnThrottleInvalidTpsAndRecrank) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;
	engineConfiguration->idlePidDeactivationTpsThreshold = 5;
	engineConfiguration->maxIdleVss = 10;
	engineConfiguration->idleVssGateClutchOverride = true;
	TgtInfo target{800, 1000, 1100};
	engine->rpmCalculator.setRpmValue(900);
	for (int exit = 0; exit < 3; exit++) {
		Sensor::setMockValue(SensorType::DetectedGear, 0);
		EXPECT_EQ(ICP::Idling, dut.determinePhase(900, target, 0, 25, 1));
		Sensor::setMockValue(SensorType::DetectedGear, 3);
		EXPECT_EQ(ICP::Idling, dut.determinePhase(900, target, 0, 25, 1));
		if (exit == 0) {
			EXPECT_EQ(ICP::Running, dut.determinePhase(900, target, 10, 25, 1));
		} else if (exit == 1) {
			EXPECT_EQ(ICP::Running, dut.determinePhase(900, target, unexpected, 25, 1));
		} else {
			engine->rpmCalculator.setRpmValue(0);
			EXPECT_EQ(ICP::Cranking, dut.determinePhase(0, target, 0, 25, 0));
			engine->rpmCalculator.setRpmValue(900);
		}
		EXPECT_EQ(ICP::Running, dut.determinePhase(900, target, 0, 25, 1));
	}
}
