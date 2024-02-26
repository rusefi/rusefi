/*
 * @file test_idle_controller.cpp
 *
 * @date Oct 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "advance_map.h"
#include "efi_pid.h"
#include "idle_thread.h"
#include "electronic_throttle.h"

using ::testing::StrictMock;
using ::testing::_;

using ICP = IIdleController::Phase;

TEST(idle_v2, timingPid) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;

	engineConfiguration->useIdleTimingPidControl = true;

	engineConfiguration->idleTimingPid.pFactor = 0.1;
	engineConfiguration->idleTimingPid.minValue = -10;
	engineConfiguration->idleTimingPid.maxValue = 10;
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

	EXPECT_FLOAT_EQ(100, dut.getTargetRpm(10));
	EXPECT_FLOAT_EQ(500, dut.getTargetRpm(50));
}

TEST(idle_v2, testDeterminePhase) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;

	// TPS threshold 5% for easy test
	engineConfiguration->idlePidDeactivationTpsThreshold = 5;
	// RPM window is 100 RPM above target
	engineConfiguration->idlePidRpmUpperLimit = 100;
	// Max VSS for idle is 10kph
	engineConfiguration->maxIdleVss = 10;

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
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;

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
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;

	engineConfiguration->manIdlePosition = 50;

	for (size_t i = 0; i < efi::size(config->cltIdleCorrBins); i++) {
		config->cltIdleCorrBins[i] = i * 10;
		config->cltIdleCorr[i] = i * 0.1f;
	}

	EXPECT_FLOAT_EQ(5, dut.getRunningOpenLoop(0, 10, 0));
	EXPECT_FLOAT_EQ(25, dut.getRunningOpenLoop(0, 50, 0));
}

TEST(idle_v2, runningFanAcBump) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;

	engineConfiguration->manIdlePosition = 50;
	engineConfiguration->acIdleExtraOffset = 9;
	engineConfiguration->fan1ExtraIdle = 7;
	engineConfiguration->fan2ExtraIdle = 3;

	setArrayValues(config->cltIdleCorr, 1.0f);

	// Start with fan off
	enginePins.fanRelay.setValue(0);

	// Should be base position
	EXPECT_FLOAT_EQ(50, dut.getRunningOpenLoop(0, 10, 0));

	// Turn on AC!
	engine->module<AcController>()->acButtonState = true;
	EXPECT_FLOAT_EQ(50 + 9, dut.getRunningOpenLoop(0, 10, 0));
	engine->module<AcController>()->acButtonState = false;

	// Turn the fan on!
	enginePins.fanRelay.setValue(1);
	EXPECT_FLOAT_EQ(50 + 7, dut.getRunningOpenLoop(0, 10, 0));
	enginePins.fanRelay.setValue(0);

	// Turn on the other fan!
	enginePins.fanRelay2.setValue(1);
	EXPECT_FLOAT_EQ(50 + 3, dut.getRunningOpenLoop(0, 10, 0));

	// Turn on everything!
	engine->module<AcController>()->acButtonState = true;
	enginePins.fanRelay.setValue(1);
	enginePins.fanRelay2.setValue(1);
	EXPECT_FLOAT_EQ(50 + 9 + 7 + 3, dut.getRunningOpenLoop(0, 10, 0));
}

TEST(idle_v2, runningOpenLoopTpsTaper) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;

	// Zero out base tempco table
	setArrayValues(config->cltIdleCorr, 0.0f);

	// Add 50% idle position
	engineConfiguration->iacByTpsTaper = 50;
	// At 10% TPS
	engineConfiguration->idlePidDeactivationTpsThreshold = 10;

	// Check in-bounds points
	EXPECT_FLOAT_EQ(0, dut.getRunningOpenLoop(0, 0, 0));
	EXPECT_FLOAT_EQ(25, dut.getRunningOpenLoop(0, 0, 5));
	EXPECT_FLOAT_EQ(50, dut.getRunningOpenLoop(0, 0, 10));

	// Check out of bounds - shouldn't leave the interval [0, 10]
	EXPECT_FLOAT_EQ(0, dut.getRunningOpenLoop(0, 0, -5));
	EXPECT_FLOAT_EQ(50, dut.getRunningOpenLoop(0, 0, 20));
}

TEST(idle_v2, runningOpenLoopRpmTaper) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	IdleController dut;

	// Zero out base tempco table
	setArrayValues(config->cltIdleCorr, 0.0f);

	// Add 50% idle position
	engineConfiguration->airByRpmTaper = 50;
	// At 2000 RPM
	engineConfiguration->airTaperRpmRange = 500;
	engineConfiguration->idlePidRpmUpperLimit = 1500;

	// Check in-bounds points
	EXPECT_FLOAT_EQ(0, dut.getRunningOpenLoop(1500, 0, 0));
	EXPECT_FLOAT_EQ(25, dut.getRunningOpenLoop(1750, 0, 0));
	EXPECT_FLOAT_EQ(50, dut.getRunningOpenLoop(2000, 0, 0));

	// Check out of bounds - shouldn't leave the interval [1500, 2000]
	EXPECT_FLOAT_EQ(0, dut.getRunningOpenLoop(200, 0, 0));
	EXPECT_FLOAT_EQ(50, dut.getRunningOpenLoop(3000, 0, 0));
}

struct MockOpenLoopIdler : public IdleController {
	MOCK_METHOD(float, getCrankingOpenLoop, (float clt), (const, override));
	MOCK_METHOD(float, getRunningOpenLoop, (float rpm, float clt, SensorResult tps), (override));
};

TEST(idle_v2, testOpenLoopCranking) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	StrictMock<MockOpenLoopIdler> dut;

	engineConfiguration->overrideCrankingIacSetting = true;

	EXPECT_CALL(dut, getCrankingOpenLoop(30)).WillOnce(Return(44));

	// Should return the value from getCrankingOpenLoop, and ignore running numbers
	EXPECT_FLOAT_EQ(44, dut.getOpenLoop(ICP::Cranking, 0, 30, 0, 0));
}

TEST(idle_v2, openLoopRunningTaper) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	StrictMock<MockOpenLoopIdler> dut;

	EXPECT_CALL(dut, getRunningOpenLoop(0, 30, SensorResult(0))).WillRepeatedly(Return(25));
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

    float expectedClt = 37;
	engineConfiguration->afterCrankingIACtaperDuration = 500;
    engineConfiguration->useCrankingIdleTaperTableSetting = false;

	// 0 cycles - no taper yet, pure cranking value
	EXPECT_FLOAT_EQ(0, dut.getCrankingTaperFraction(expectedClt));

	// 250 cycles - half way, 50% each value -> outputs 50
	for (size_t i = 0; i < 250; i++) {
		engine->rpmCalculator.onNewEngineCycle();
	}
	EXPECT_FLOAT_EQ(0.5f, dut.getCrankingTaperFraction(expectedClt));

	// 500 cycles - fully tapered, should be running value
	for (size_t i = 0; i < 250; i++) {
		engine->rpmCalculator.onNewEngineCycle();
	}
	EXPECT_FLOAT_EQ(1, dut.getCrankingTaperFraction(expectedClt));

	// 1000 cycles - still fully tapered, should be running value
	for (size_t i = 0; i < 500; i++) {
		engine->rpmCalculator.onNewEngineCycle();
	}
	EXPECT_FLOAT_EQ(2, dut.getCrankingTaperFraction(expectedClt));
}

TEST(idle_v2, getCrankingTaperFractionWithMultiplier) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    StrictMock<MockOpenLoopIdler> dut;

    float expectedClt = 40;

    engineConfiguration->afterCrankingIACtaperDuration = 200;
    engineConfiguration->useCrankingIdleTaperTableSetting = true;
	
	float curve[CLT_CRANKING_CURVE_SIZE] = {
		1.0, // 0C
		1.0, // 10C
		1.0, // 20C
		0.5, // 30C
		0.4, // 40C
		0.3, // 50C
		0.2, // 60C
		0.1  // 70C
		};

    for (int i = 0; i < CLT_CRANKING_CURVE_SIZE; i++) {
        config->cltCrankingTaperCorrBins[i] = i * 10;
        config->cltCrankingTaperCorr[i] = curve[i];
    }

    // 0 cycles - no taper yet, pure cranking value
    EXPECT_FLOAT_EQ(0, dut.getCrankingTaperFraction(expectedClt));

    // 50 cycles in - total taper duration should be 200 * 0.4 (40C) = 80 cyclyes instead of 200
    for (size_t i = 0; i < 50; i++) {
        engine->rpmCalculator.onNewEngineCycle();
    }
	// So 80*0.625 = 50 cycles
    EXPECT_FLOAT_EQ(0.625f, dut.getCrankingTaperFraction(expectedClt));
	// testing 20C colder, should use 1.0f so 200*0.25 = 50 cycles
    EXPECT_FLOAT_EQ(0.25f, dut.getCrankingTaperFraction(expectedClt - 20));

	// 200 cycles in - total taper duration should be done by 40C
    for (size_t i = 0; i < 150; i++) {
        engine->rpmCalculator.onNewEngineCycle();
    }
	// Taper last only 80 cycles, so 2.5*80 = 200 cycles
	EXPECT_FLOAT_EQ(2.5f, dut.getCrankingTaperFraction(expectedClt));
	// Taper last full length
	EXPECT_FLOAT_EQ(1.0f, dut.getCrankingTaperFraction(expectedClt - 20));

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

extern int timeNowUs;

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
	timeNowUs += 5'000'000;

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
	timeNowUs += 5'000'000;

	// Test above target, should return negative
	EXPECT_FLOAT_EQ(-25, dut.getClosedLoop(ICP::Idling, 0, /*rpm*/ 950, /*tgt*/ 900));

	// Inside deadzone, should return same as last time
	EXPECT_FLOAT_EQ(-25, dut.getClosedLoop(ICP::Idling, 0, /*rpm*/ 900, /*tgt*/ 900));
}

struct IntegrationIdleMock : public IdleController {
	MOCK_METHOD(int, getTargetRpm, (float clt), (override));
	MOCK_METHOD(ICP, determinePhase, (int rpm, int targetRpm, SensorResult tps, float vss, float crankingTaperFraction), (override));
	MOCK_METHOD(float, getOpenLoop, (ICP phase, float rpm, float clt, SensorResult tps, float crankingTaperFraction), (override));
	MOCK_METHOD(float, getClosedLoop, (ICP phase, float tps, int rpm, int target), (override));
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

	// Target of 1000 rpm
	EXPECT_CALL(dut, getTargetRpm(expectedClt))
		.WillOnce(Return(1000));

	// 30% of the way through cranking taper
	EXPECT_CALL(dut, getCrankingTaperFraction(expectedClt))
		.WillOnce(Return(0.3f));

	// Determine phase will claim we're idling
	EXPECT_CALL(dut, determinePhase(950, 1000, expectedTps, 15, 0.3f))
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

	engineConfiguration->idleMode = IM_AUTO;

	SensorResult expectedTps = 1;
	float expectedClt = 37;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, expectedTps.Value);
	Sensor::setMockValue(SensorType::Clt, expectedClt);
	Sensor::setMockValue(SensorType::VehicleSpeed, 15.0);

	// Target of 1000 rpm
	EXPECT_CALL(dut, getTargetRpm(expectedClt))
		.WillOnce(Return(1000));

	// 40% of the way through cranking taper
	EXPECT_CALL(dut, getCrankingTaperFraction(expectedClt))
		.WillOnce(Return(0.4f));

	// Determine phase will claim we're idling
	EXPECT_CALL(dut, determinePhase(950, 1000, expectedTps, 15, 0.4f))
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

	engineConfiguration->idleMode = IM_AUTO;

	SensorResult expectedTps = 1;
	float expectedClt = 37;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, expectedTps.Value);
	Sensor::setMockValue(SensorType::Clt, expectedClt);
	Sensor::setMockValue(SensorType::VehicleSpeed, 15.0);

	// Target of 1000 rpm
	EXPECT_CALL(dut, getTargetRpm(expectedClt))
		.WillOnce(Return(1000));

	// 50% of the way through cranking taper
	EXPECT_CALL(dut, getCrankingTaperFraction(expectedClt))
		.WillOnce(Return(0.5f));

	// Determine phase will claim we're idling
	EXPECT_CALL(dut, determinePhase(950, 1000, expectedTps, 15, 0.5f))
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
