/*
 * @file test_idle_controller.cpp
 *
 * @date Oct 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "advance_map.h"
#include "pid.h"
#include "fsio_impl.h"
#include "idle_thread.h"
#include "electronic_throttle.h"

using ::testing::StrictMock;
using ::testing::_;

using ICP = IIdleController::Phase;

TEST(idle_v2, timingPid) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	IdleController dut;
	dut.inject();

	engineConfiguration->useIdleTimingPidControl = true;

	pid_s pidCfg{};
	pidCfg.pFactor = 0.1;
	pidCfg.minValue = -10;
	pidCfg.maxValue = 10;
	dut.init(&pidCfg);

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

	// Now check that the deadzone works
	engineConfiguration->idleTimingPidDeadZone = 50;
	EXPECT_FLOAT_EQ(5.1, dut.getIdleTimingAdjustment(949, 1000, ICP::Idling));
	EXPECT_EQ(0, dut.getIdleTimingAdjustment(951, 1000, ICP::Idling));
	EXPECT_EQ(0, dut.getIdleTimingAdjustment(1000, 1000, ICP::Idling));
	EXPECT_EQ(0, dut.getIdleTimingAdjustment(1049, 1000, ICP::Idling));
	EXPECT_FLOAT_EQ(-5.1, dut.getIdleTimingAdjustment(1051, 1000, ICP::Idling));
}

TEST(idle_v2, testTargetRpm) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	IdleController dut;
	dut.inject();

	for (size_t i = 0; i < efi::size(engineConfiguration->cltIdleRpmBins); i++) {
		CONFIG(cltIdleRpmBins)[i] = i * 10;
		CONFIG(cltIdleRpm)[i] = i * 100;
	}

	EXPECT_FLOAT_EQ(100, dut.getTargetRpm(10));
	EXPECT_FLOAT_EQ(500, dut.getTargetRpm(50));
}

TEST(idle_v2, testDeterminePhase) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	IdleController dut;
	dut.inject();

	// TPS threshold 5% for easy test
	CONFIG(idlePidDeactivationTpsThreshold) = 5;
	// RPM window is 100 RPM above target
	CONFIG(idlePidRpmUpperLimit) = 100;
	// Max VSS for idle is 10kph
	CONFIG(maxIdleVss) = 10;

	// First test stopped engine
	engine->rpmCalculator.setRpmValue(0);
	EXPECT_EQ(ICP::Cranking, dut.determinePhase(0, 1000, unexpected, 0, 10));

	// Now engine is running!
	// Controller doesn't need this other than for isCranking()
	engine->rpmCalculator.setRpmValue(1000);

	// Test invalid TPS, but inside the idle window
	EXPECT_EQ(ICP::Running, dut.determinePhase(1000, 1000, unexpected, 0, 10));

	// Valid TPS should now be inside the zone
	EXPECT_EQ(ICP::Idling, dut.determinePhase(1000, 1000, 0, 0, 10));

	// Inside the zone, but vehicle speed too fast
	EXPECT_EQ(ICP::Running, dut.determinePhase(1000, 1000, 0, 25, 10));

	// Check that shortly after cranking, the cranking taper inhibits closed loop idle
	EXPECT_EQ(ICP::CrankToIdleTaper, dut.determinePhase(1000, 1000, 0, 0, 0.5f));

	// Above TPS threshold should be outside the zone
	EXPECT_EQ(ICP::Running, dut.determinePhase(1000, 1000, 10, 0, 10));

	// Above target, below (target + upperLimit) should be in idle zone
	EXPECT_EQ(ICP::Idling, dut.determinePhase(1099, 1000, 0, 0, 10));

	// above upper limit and on throttle should be out of idle zone
	EXPECT_EQ(ICP::Running, dut.determinePhase(1101, 1000, 10, 0, 10));

	// Below TPS but above RPM should be outside the zone
	EXPECT_EQ(ICP::Coasting, dut.determinePhase(1101, 1000, 0, 0, 10));
	EXPECT_EQ(ICP::Coasting, dut.determinePhase(5000, 1000, 0, 0, 10));
}

TEST(idle_v2, crankingOpenLoop) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	IdleController dut;
	dut.inject();

	engineConfiguration->crankingIACposition = 50;

	for (size_t i = 0; i < efi::size(config->cltCrankingCorrBins); i++) {
		config->cltCrankingCorrBins[i] = i * 10;
		config->cltCrankingCorr[i] = i * 0.1f;

		// different values in running so we can tell which one is used
		config->cltIdleCorrBins[i] = i * 10;
		config->cltIdleCorr[i] = i * 0.2f;
	}

	// First test without override (ie, normal running CLT corr table)
	EXPECT_FLOAT_EQ(10, dut.getCrankingOpenLoop(10));
	EXPECT_FLOAT_EQ(50, dut.getCrankingOpenLoop(50));

	// Test with override (use separate table)
	engineConfiguration->overrideCrankingIacSetting = true;
	EXPECT_FLOAT_EQ(5, dut.getCrankingOpenLoop(10));
	EXPECT_FLOAT_EQ(25, dut.getCrankingOpenLoop(50));
}

TEST(idle_v2, runningOpenLoopBasic) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	IdleController dut;
	dut.inject();

	engineConfiguration->manIdlePosition = 50;

	for (size_t i = 0; i < efi::size(config->cltIdleCorrBins); i++) {
		config->cltIdleCorrBins[i] = i * 10;
		config->cltIdleCorr[i] = i * 0.1f;
	}

	EXPECT_FLOAT_EQ(5, dut.getRunningOpenLoop(10, 0));
	EXPECT_FLOAT_EQ(25, dut.getRunningOpenLoop(50, 0));
}

TEST(idle_v2, runningFanAcBump) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	IdleController dut;
	dut.inject();

	engineConfiguration->manIdlePosition = 50;
	engineConfiguration->acIdleExtraOffset = 9;
	engineConfiguration->fan1ExtraIdle = 7;
	engineConfiguration->fan2ExtraIdle = 3;

	setArrayValues(config->cltIdleCorr, 1.0f);

	// Start with fan off
	enginePins.fanRelay.setValue(0);

	// Should be base position
	EXPECT_FLOAT_EQ(50, dut.getRunningOpenLoop(10, 0));

	// Turn on AC!
	engine->acSwitchState = true;
	EXPECT_FLOAT_EQ(50 + 9, dut.getRunningOpenLoop(10, 0));
	engine->acSwitchState = false;

	// Turn the fan on!
	enginePins.fanRelay.setValue(1);
	EXPECT_FLOAT_EQ(50 + 7, dut.getRunningOpenLoop(10, 0));
	enginePins.fanRelay.setValue(0);

	// Turn on the other fan!
	enginePins.fanRelay2.setValue(1);
	EXPECT_FLOAT_EQ(50 + 3, dut.getRunningOpenLoop(10, 0));

	// Turn on everything!
	engine->acSwitchState = true;
	enginePins.fanRelay.setValue(1);
	enginePins.fanRelay2.setValue(1);
	EXPECT_FLOAT_EQ(50 + 9 + 7 + 3, dut.getRunningOpenLoop(10, 0));
}

TEST(idle_v2, runningOpenLoopTpsTaper) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	IdleController dut;
	dut.inject();

	// Zero out base tempco table
	setArrayValues(config->cltIdleCorr, 0.0f);

	// Add 50% idle position
	CONFIG(iacByTpsTaper) = 50;
	// At 10% TPS
	CONFIG(idlePidDeactivationTpsThreshold) = 10;

	// Check in-bounds points
	EXPECT_FLOAT_EQ(0, dut.getRunningOpenLoop(0, 0));
	EXPECT_FLOAT_EQ(25, dut.getRunningOpenLoop(0, 5));
	EXPECT_FLOAT_EQ(50, dut.getRunningOpenLoop(0, 10));

	// Check out of bounds - shouldn't leave the interval [0, 10]
	EXPECT_FLOAT_EQ(0, dut.getRunningOpenLoop(0, -5));
	EXPECT_FLOAT_EQ(50, dut.getRunningOpenLoop(0, 20));
}

struct MockOpenLoopIdler : public IdleController {
	MOCK_METHOD(float, getCrankingOpenLoop, (float clt), (const, override));
	MOCK_METHOD(float, getRunningOpenLoop, (float clt, SensorResult tps), (const, override));
};

TEST(idle_v2, testOpenLoopCranking) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	StrictMock<MockOpenLoopIdler> dut;
	dut.inject();

	CONFIG(overrideCrankingIacSetting) = true;

	EXPECT_CALL(dut, getCrankingOpenLoop(30)).WillOnce(Return(44));

	// Should return the value from getCrankingOpenLoop, and ignore running numbers
	EXPECT_FLOAT_EQ(44, dut.getOpenLoop(ICP::Cranking, 30, 0, 0));
}

TEST(idle_v2, openLoopRunningTaper) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	StrictMock<MockOpenLoopIdler> dut;
	dut.inject();

	EXPECT_CALL(dut, getRunningOpenLoop(30, SensorResult(0))).WillRepeatedly(Return(25));
	EXPECT_CALL(dut, getCrankingOpenLoop(30)).WillRepeatedly(Return(75));

	// 0 cycles - no taper yet, pure cranking value
	EXPECT_FLOAT_EQ(75, dut.getOpenLoop(ICP::Running, 30, 0, 0));
	EXPECT_FLOAT_EQ(75, dut.getOpenLoop(ICP::CrankToIdleTaper, 30, 0, 0));

	// 1/2 taper - half way, 50% each value -> outputs 50
	EXPECT_FLOAT_EQ(50, dut.getOpenLoop(ICP::Running, 30, 0, 0.5f));
	EXPECT_FLOAT_EQ(50, dut.getOpenLoop(ICP::CrankToIdleTaper, 30, 0, 0.5f));

	// 1x taper - fully tapered, should be running value
	EXPECT_FLOAT_EQ(25, dut.getOpenLoop(ICP::Running, 30, 0, 1.0f));
	EXPECT_FLOAT_EQ(25, dut.getOpenLoop(ICP::CrankToIdleTaper, 30, 0, 1.0f));

	// 2x taper - still fully tapered, should be running value
	EXPECT_FLOAT_EQ(25, dut.getOpenLoop(ICP::Running, 30, 0, 2.0f));
	EXPECT_FLOAT_EQ(25, dut.getOpenLoop(ICP::CrankToIdleTaper, 30, 0, 2.0f));
}

TEST(idle_v2, getCrankingTaperFraction) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	StrictMock<MockOpenLoopIdler> dut;
	dut.inject();

	CONFIG(afterCrankingIACtaperDuration) = 500;

	// 0 cycles - no taper yet, pure cranking value
	EXPECT_FLOAT_EQ(0, dut.getCrankingTaperFraction());

	// 250 cycles - half way, 50% each value -> outputs 50
	for (size_t i = 0; i < 250; i++) {
		engine->rpmCalculator.onNewEngineCycle();
	}
	EXPECT_FLOAT_EQ(0.5f, dut.getCrankingTaperFraction());

	// 500 cycles - fully tapered, should be running value
	for (size_t i = 0; i < 250; i++) {
		engine->rpmCalculator.onNewEngineCycle();
	}
	EXPECT_FLOAT_EQ(1, dut.getCrankingTaperFraction());

	// 1000 cycles - still fully tapered, should be running value
	for (size_t i = 0; i < 500; i++) {
		engine->rpmCalculator.onNewEngineCycle();
	}
	EXPECT_FLOAT_EQ(2, dut.getCrankingTaperFraction());
}

TEST(idle_v2, openLoopCoastingTable) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	IdleController dut;
	dut.inject();

	// enable & configure feature
	CONFIG(useIacTableForCoasting) = true;
	for (size_t i = 0; i < CLT_CURVE_SIZE; i++) {
		CONFIG(iacCoastingBins)[i] = 10 * i;
		CONFIG(iacCoasting)[i] = 5 * i;
	}

	EXPECT_FLOAT_EQ(10, dut.getOpenLoop(ICP::Coasting, 20, 0, 2));
	EXPECT_FLOAT_EQ(20, dut.getOpenLoop(ICP::Coasting, 40, 0, 2));
}

extern int timeNowUs;

TEST(idle_v2, closedLoopBasic) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	IdleController dut;
	dut.inject();

	// Not testing PID here, so we can set very simple PID gains
	CONFIG(idleRpmPid).pFactor = 0.5;	// 0.5 output per 1 RPM error = 50% per 100 rpm
	CONFIG(idleRpmPid).iFactor = 0;
	CONFIG(idleRpmPid).dFactor = 0;
	CONFIG(idleRpmPid).offset = 0;
	CONFIG(idleRpmPid).iFactor = 0;
	CONFIG(idleRpmPid).periodMs = 0;
	CONFIG(idleRpmPid).minValue = -50;
	CONFIG(idleRpmPid).maxValue = 50;

	CONFIG(idlePidRpmDeadZone) = 0;

	// burn one update then advance time 5 seconds to avoid difficulty from wasResetPid
	dut.getClosedLoop(ICP::Idling, 0, 900, 900);
	timeNowUs += 5'000'000;

	// Test above target, should return negative
	EXPECT_FLOAT_EQ(-25, dut.getClosedLoop(ICP::Idling, 0, /*rpm*/ 950, /*tgt*/ 900));

	// Below target, should return positive
	EXPECT_FLOAT_EQ(25, dut.getClosedLoop(ICP::Idling, 0, /*rpm*/ 850, /*tgt*/ 900));
}

TEST(idle_v2, closedLoopDeadzone) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	IdleController dut;
	dut.inject();

	// Not testing PID here, so we can set very simple PID gains
	CONFIG(idleRpmPid).pFactor = 0.5;	// 0.5 output per 1 RPM error = 50% per 100 rpm
	CONFIG(idleRpmPid).iFactor = 0;
	CONFIG(idleRpmPid).dFactor = 0;
	CONFIG(idleRpmPid).offset = 0;
	CONFIG(idleRpmPid).iFactor = 0;
	CONFIG(idleRpmPid).periodMs = 0;
	CONFIG(idleRpmPid).minValue = -50;
	CONFIG(idleRpmPid).maxValue = 50;

	CONFIG(idlePidRpmDeadZone) = 25;

	// burn one then advance time 5 seconds to avoid difficulty from wasResetPid
	dut.getClosedLoop(ICP::Idling, 0, 900, 900);
	timeNowUs += 5'000'000;

	// Test above target, should return negative
	EXPECT_FLOAT_EQ(-25, dut.getClosedLoop(ICP::Idling, 0, /*rpm*/ 950, /*tgt*/ 900));

	// Inside deadzone, should return same as last time
	EXPECT_FLOAT_EQ(-25, dut.getClosedLoop(ICP::Idling, 0, /*rpm*/ 900, /*tgt*/ 900));
}

struct IntegrationIdleMock : public IdleController {
	MOCK_METHOD(int, getTargetRpm, (float clt), (const, override));
	MOCK_METHOD(ICP, determinePhase, (int rpm, int targetRpm, SensorResult tps, float vss, float crankingTaperFraction), (const, override));
	MOCK_METHOD(float, getOpenLoop, (ICP phase, float clt, SensorResult tps, float crankingTaperFraction), (const, override));
	MOCK_METHOD(float, getClosedLoop, (ICP phase, float tps, int rpm, int target), (override));
	MOCK_METHOD(float, getCrankingTaperFraction, (), (const, override));
};

TEST(idle_v2, IntegrationManual) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	StrictMock<IntegrationIdleMock> dut;
	dut.inject();

	SensorResult expectedTps = 1;
	float expectedClt = 37;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, expectedTps.Value);
	Sensor::setMockValue(SensorType::Clt, expectedClt);
	Sensor::setMockValue(SensorType::VehicleSpeed, 15.0);
	ENGINE(rpmCalculator.mockRpm) = 950;

	// Target of 1000 rpm
	EXPECT_CALL(dut, getTargetRpm(expectedClt))
		.WillOnce(Return(1000));

	// 30% of the way through cranking taper
	EXPECT_CALL(dut, getCrankingTaperFraction())
		.WillOnce(Return(0.3f));

	// Determine phase will claim we're idling
	EXPECT_CALL(dut, determinePhase(950, 1000, expectedTps, 15, 0.3f))
		.WillOnce(Return(ICP::Idling));

	// Open loop should be asked for an open loop position
	EXPECT_CALL(dut, getOpenLoop(ICP::Idling, expectedClt, expectedTps, 0.3f))
		.WillOnce(Return(13));

	// getClosedLoop() should not be called!

	EXPECT_EQ(13, dut.getIdlePosition());
}

TEST(idle_v2, IntegrationAutomatic) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	StrictMock<IntegrationIdleMock> dut;
	dut.inject();

	CONFIG(idleMode) = IM_AUTO;

	SensorResult expectedTps = 1;
	float expectedClt = 37;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, expectedTps.Value);
	Sensor::setMockValue(SensorType::Clt, expectedClt);
	Sensor::setMockValue(SensorType::VehicleSpeed, 15.0);
	ENGINE(rpmCalculator.mockRpm) = 950;

	// Target of 1000 rpm
	EXPECT_CALL(dut, getTargetRpm(expectedClt))
		.WillOnce(Return(1000));

	// 40% of the way through cranking taper
	EXPECT_CALL(dut, getCrankingTaperFraction())
		.WillOnce(Return(0.4f));

	// Determine phase will claim we're idling
	EXPECT_CALL(dut, determinePhase(950, 1000, expectedTps, 15, 0.4f))
		.WillOnce(Return(ICP::Idling));

	// Open loop should be asked for an open loop position
	EXPECT_CALL(dut, getOpenLoop(ICP::Idling, expectedClt, expectedTps, 0.4f))
		.WillOnce(Return(13));

	// Closed loop should get called
	EXPECT_CALL(dut, getClosedLoop(ICP::Idling, expectedTps.Value, 950, 1000))
		.WillOnce(Return(7));

	// Result should be open + closed
	EXPECT_EQ(13 + 7, dut.getIdlePosition());
}

TEST(idle_v2, IntegrationClamping) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	StrictMock<IntegrationIdleMock> dut;
	dut.inject();

	CONFIG(idleMode) = IM_AUTO;

	SensorResult expectedTps = 1;
	float expectedClt = 37;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, expectedTps.Value);
	Sensor::setMockValue(SensorType::Clt, expectedClt);
	Sensor::setMockValue(SensorType::VehicleSpeed, 15.0);
	ENGINE(rpmCalculator.mockRpm) = 950;

	// Target of 1000 rpm
	EXPECT_CALL(dut, getTargetRpm(expectedClt))
		.WillOnce(Return(1000));

	// 50% of the way through cranking taper
	EXPECT_CALL(dut, getCrankingTaperFraction())
		.WillOnce(Return(0.5f));

	// Determine phase will claim we're idling
	EXPECT_CALL(dut, determinePhase(950, 1000, expectedTps, 15, 0.5f))
		.WillOnce(Return(ICP::Idling));

	// Open loop should be asked for an open loop position
	EXPECT_CALL(dut, getOpenLoop(ICP::Idling, expectedClt, expectedTps, 0.5f))
		.WillOnce(Return(75));

	// Closed loop should get called
	EXPECT_CALL(dut, getClosedLoop(ICP::Idling, expectedTps.Value, 950, 1000))
		.WillOnce(Return(75));

	// Result would be 75 + 75 = 150, but it should clamp to 100
	EXPECT_EQ(100, dut.getIdlePosition());
}
