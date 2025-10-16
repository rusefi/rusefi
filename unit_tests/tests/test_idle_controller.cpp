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

	engineConfiguration->acIdleExtraOffset = 9;
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

// This can be seen as a kind of some close-loop logic, please read:
// https://github.com/rusefi/rusefi/issues/6977
TEST(idle_v2, idleAdderShouldNotAffectNonIdleAreas) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;

	engineConfiguration->acIdleExtraOffset = 9;

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
