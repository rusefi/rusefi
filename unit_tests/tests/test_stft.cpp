#include "pch.h"

#include "closed_loop_fuel_cell.h"
#include "closed_loop_fuel.h"
#include "tunerstudio.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

class MockClCell : public ClosedLoopFuelCellBase {
public:
	MOCK_METHOD(float, getLambdaError, (), (const));
	MOCK_METHOD(float, getMaxAdjustment, (), (const));
	MOCK_METHOD(float, getMinAdjustment, (), (const));
	MOCK_METHOD(float, getIntegratorGain, (), (const));
};

TEST(ClosedLoopCell, TestDeadband) {
	StrictMock<MockClCell> cl;

	// Error is more than deadtime, so nothing else should be called
	EXPECT_CALL(cl, getLambdaError())
		.WillOnce(Return(0.05f));

	cl.update(0.1f, true);

	// Should be zero adjustment
	EXPECT_FLOAT_EQ(cl.getAdjustment(), 1.0f);
}

TEST(ClosedLoopFuelCell, AdjustRate) {
	StrictMock<MockClCell> cl;

	EXPECT_CALL(cl, getLambdaError())
		.WillOnce(Return(0.1f));
	EXPECT_CALL(cl, getMinAdjustment())
		.WillOnce(Return(-0.2f));
	EXPECT_CALL(cl, getMaxAdjustment())
		.WillOnce(Return(0.2f));
	EXPECT_CALL(cl, getIntegratorGain())
		.WillOnce(Return(2.0f));

	cl.update(0.0f, false);

	// Should have integrated 0.2 * dt
	// dt = 1000.0f / FAST_CALLBACK_PERIOD_MS
	EXPECT_FLOAT_EQ(cl.getAdjustment(), 1 + (0.2f / (1000.0f / FAST_CALLBACK_PERIOD_MS)));
}

TEST(ClosedLoopFuel, CellSelection) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	ShortTermFuelTrim stft;
	stft_s cfg;

	// Sensible region config
	cfg.maxIdleRegionRpm = 1500;
	cfg.minPowerLoad = 80;
	cfg.maxOverrunLoad = 30;

	stft.init(&cfg);

	// Test idle
	EXPECT_EQ(0u, stft.computeStftBin(1000, 10, cfg));
	EXPECT_EQ(0u, stft.computeStftBin(1000, 50, cfg));
	EXPECT_EQ(0u, stft.computeStftBin(1000, 90, cfg));

	// Test overrun
	EXPECT_EQ(1u, stft.computeStftBin(2000, 10, cfg));
	EXPECT_EQ(1u, stft.computeStftBin(4000, 10, cfg));
	EXPECT_EQ(1u, stft.computeStftBin(10000, 10, cfg));

	// Test load
	EXPECT_EQ(2u, stft.computeStftBin(2000, 90, cfg));
	EXPECT_EQ(2u, stft.computeStftBin(4000, 90, cfg));
	EXPECT_EQ(2u, stft.computeStftBin(10000, 90, cfg));

	// Main cell
	EXPECT_EQ(3u, stft.computeStftBin(2000, 50, cfg));
	EXPECT_EQ(3u, stft.computeStftBin(4000, 50, cfg));
	EXPECT_EQ(3u, stft.computeStftBin(10000, 50, cfg));
}

TEST(ClosedLoopFuel, pausedDuringTwoStepLaunch) {
	ShortTermFuelTrim stft;
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->stft.minAfr = 10;  // 10.0 AFR
	engineConfiguration->stft.maxAfr = 18;  // 18.0 AFR

	stft.init(&engineConfiguration->stft);

	Sensor::setMockValue(SensorType::Lambda1, 1.0f);
	EXPECT_EQ(stft.getLearningState(SensorType::Lambda1), stftEnabled);

	engineConfiguration->launchControlEnabled = true;

	// Pre-launch spark skip window pauses learning
	engine->launchController.isPreLaunchCondition = true;
	EXPECT_EQ(stft.getLearningState(SensorType::Lambda1), stftDisabledLaunch);

	// Full launch pauses learning
	engine->launchController.isPreLaunchCondition = false;
	engine->launchController.isLaunchCondition = true;
	EXPECT_EQ(stft.getLearningState(SensorType::Lambda1), stftDisabledLaunch);

	// Launch over - learning resumes
	engine->launchController.isLaunchCondition = false;
	EXPECT_EQ(stft.getLearningState(SensorType::Lambda1), stftEnabled);
}

TEST(ClosedLoopFuel, afrLimits) {
	ShortTermFuelTrim stft;
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->stft.minAfr = 10;  // 10.0 AFR
	engineConfiguration->stft.maxAfr = 18;  // 18.0 AFR

	stft.init(&engineConfiguration->stft);

	Sensor::setMockValue(SensorType::Lambda1, 0.1f);
	EXPECT_NE(stft.getLearningState(SensorType::Lambda1), stftEnabled);

	Sensor::setMockValue(SensorType::Lambda1, 1.0f);
	EXPECT_EQ(stft.getLearningState(SensorType::Lambda1), stftEnabled);

	Sensor::setMockValue(SensorType::Lambda1, 2.0f);
	EXPECT_NE(stft.getLearningState(SensorType::Lambda1), stftEnabled);
}

TEST(ClosedLoopFuel, StateBasedSelectionAndFallback) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& sm = engine->module<EngineStateMachine>().unmock();
	auto& stft = engine->module<ShortTermFuelTrim>().unmock();
	auto& cfg = engineConfiguration->stft;
	cfg.maxIdleRegionRpm = 1500;
	cfg.minPowerLoad = 80;
	cfg.maxOverrunLoad = 30;
	engineConfiguration->useEngineStateMachine = true;
	engineConfiguration->stateBasedStft = true;
	engine->rpmCalculator.setRpmValue(3000);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 95);
	sm.onSlowCallback();
	ASSERT_EQ(sm.getCurrentState(), EngineStateMachineState::WOT);
	EXPECT_EQ(stft.computeStftBin(1000, 10, cfg), ftRegionPower);
	engineConfiguration->stateBasedStft = false;
	EXPECT_EQ(stft.computeStftBin(1000, 10, cfg), ftRegionIdle);
	engineConfiguration->stateBasedStft = true;
	engineConfiguration->useEngineStateMachine = false;
	EXPECT_EQ(stft.computeStftBin(1000, 10, cfg), ftRegionIdle);
}

TEST(ClosedLoopFuel, StateBasedRegionMapping) {
	using S = EngineStateMachineState;
	EXPECT_EQ(ShortTermFuelTrim::regionForSmState(S::Idle), ftRegionIdle);
	EXPECT_EQ(ShortTermFuelTrim::regionForSmState(S::Afterstart), ftRegionIdle);
	EXPECT_EQ(ShortTermFuelTrim::regionForSmState(S::Coasting), ftRegionIdle);
	EXPECT_EQ(ShortTermFuelTrim::regionForSmState(S::Overrun), ftRegionOverrun);
	EXPECT_EQ(ShortTermFuelTrim::regionForSmState(S::WOT), ftRegionPower);
	for (auto state : {S::Off, S::Cranking, S::Transient, S::Cruising, S::Accelerating,
		S::Decelerating, S::LaunchControl, S::Upshifting, S::Downshifting}) {
		EXPECT_EQ(ShortTermFuelTrim::regionForSmState(state), ftRegionCruise);
	}
}

namespace {

// #4258 / #4254: the startup timer must advance even when post-cranking
// enrichment is disabled. Exercise its producer and STFT together through
// the engine callback, without assigning timeSinceCrankingInSecs in the test.
void checkClosedLoopStartup(float postCrankingFactor, float expectedCorrection) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& stft = engine->module<ShortTermFuelTrim>().unmock();
	engineConfiguration->cranking.rpm = 500;
	engineConfiguration->fuelClosedLoopCorrectionEnabled = true;
	engineConfiguration->isTuningDetectorEnabled = false;
	engineConfiguration->useEngineStateMachine = false;
	engineConfiguration->coastingFuelCutEnabled = false;
	engineConfiguration->noFuelTrimAfterAccelTime = 0;
	engineConfiguration->stftIgnoreErrorMagnitude = false;
	auto& cfg = engineConfiguration->stft;
	cfg.startupDelay = 10;
	cfg.minClt = 60;
	cfg.minAfr = 10;
	cfg.maxAfr = 18;
	cfg.deadband = 1;
	for (auto& cell : cfg.cellCfgs) {
		cell.maxAdd = 10;
		cell.maxRemove = 10;
		cell.timeConstant = 0.1f;
	}
	stft.init(&cfg);
	setTable(config->lambdaTable, 1);
	setTable(config->postCrankingFactor, postCrankingFactor);
	Sensor::setMockValue(SensorType::Clt, 80);
	Sensor::setMockValue(SensorType::Lambda1, 1.1f);
	Sensor::setMockValue(SensorType::Lambda2, 0.9f);
	EXPECT_CALL(*eth.mockAirmass, getAirmass(_, _))
		.WillRepeatedly(Return(AirmassResult{0.6f, 50}));

	auto advance = [&](float milliseconds) {
		eth.moveTimeForwardMs(milliseconds);
		engine->periodicFastCallback();
		EXPECT_FLOAT_EQ(expectedCorrection, engine->fuelComputer.running.postCrankingFuelCorrection);
	};
	auto expectDisabled = [&](stft_state_e reason) {
		EXPECT_EQ(reason, stft.stftCorrectionState);
		for (size_t bank = 0; bank < FT_BANK_COUNT; bank++) {
			EXPECT_EQ(reason, stft.stftLearningState[bank]);
			EXPECT_FLOAT_EQ(1, engine->engineState.stftCorrection[bank]);
		}
	};

	// Repeat after a stop: a fresh cranking attempt must reset the delay,
	// regardless of the elapsed time and learned trim from the previous run.
	for (int attempt = 0; attempt < 2; attempt++) {
		SCOPED_TRACE(attempt);
		engine->rpmCalculator.setRpmValue(0);
		advance(0);
		expectDisabled(stftDisabledRPM);
		engine->rpmCalculator.setRpmValue(200);
		ASSERT_TRUE(engine->rpmCalculator.isCranking());
		advance(5000);
		EXPECT_FLOAT_EQ(0, engine->fuelComputer.running.timeSinceCrankingInSecs);
		expectDisabled(stftDisabledRPM);
		advance(5000);
		EXPECT_FLOAT_EQ(0, engine->fuelComputer.running.timeSinceCrankingInSecs);

		engine->rpmCalculator.setRpmValue(3000);
		ASSERT_TRUE(engine->rpmCalculator.isRunning());
		advance(0);
		expectDisabled(stftDisabledCrankingDelay);
		advance(9999);
		EXPECT_NEAR(9.999f, engine->fuelComputer.running.timeSinceCrankingInSecs, 1e-5f);
		expectDisabled(stftDisabledCrankingDelay);

		advance(1);
		EXPECT_FLOAT_EQ(10, engine->fuelComputer.running.timeSinceCrankingInSecs);
		EXPECT_EQ(stftEnabled, stft.stftCorrectionState);
		EXPECT_EQ(stftEnabled, stft.stftLearningState[0]);
		EXPECT_EQ(stftEnabled, stft.stftLearningState[1]);
		// Lambda errors must produce real corrections, not just an enabled flag.
		EXPECT_GT(engine->engineState.stftCorrection[0], 1);
		EXPECT_LT(engine->engineState.stftCorrection[1], 1);
	}
}

TEST(ClosedLoopFuel, StartupDelayWithNeutralPostCrankingEnrichment) {
	checkClosedLoopStartup(1, 1);
}

TEST(ClosedLoopFuel, StartupDelayWithZeroPostCrankingEnrichment) {
	// Values below unity are treated as no enrichment for compatibility.
	checkClosedLoopStartup(0, 1);
}

TEST(ClosedLoopFuel, StartupDelayWithActivePostCrankingEnrichment) {
	checkClosedLoopStartup(1.2f, 1.2f);
}

// Exercise the production cells through the public correction API, without
// simulating trigger events or writing the private integrator state.
class ShortTermFuelTrimBehavior : public ::testing::Test {
protected:
	EngineTestHelper eth{engine_type_e::TEST_ENGINE};
	ShortTermFuelTrim stft;

	void SetUp() override {
		resetCalibrationTimerForTest();
		engineConfiguration->fuelClosedLoopCorrectionEnabled = true;
		engineConfiguration->isTuningDetectorEnabled = false;
		engineConfiguration->useEngineStateMachine = false;
		engineConfiguration->stftIgnoreErrorMagnitude = false;
		engineConfiguration->noFuelTrimAfterDfcoTime = 0.5f;
		engineConfiguration->noFuelTrimAfterAccelTime = 0;
		auto& cfg = engineConfiguration->stft;
		cfg.startupDelay = 10;
		cfg.minClt = 60;
		cfg.minAfr = 10;
		cfg.maxAfr = 18;
		cfg.deadband = 1;
		cfg.maxIdleRegionRpm = 1500;
		cfg.maxOverrunLoad = 30;
		cfg.minPowerLoad = 80;
		for (auto& cell : cfg.cellCfgs) {
			cell.maxAdd = 10;
			cell.maxRemove = 10;
			cell.timeConstant = 0.1f;
		}
		stft.init(&cfg);
		engine->rpmCalculator.setRpmValue(3000);
		engine->fuelComputer.running.timeSinceCrankingInSecs = 10;
		engine->fuelComputer.targetLambda = 1;
		Sensor::setMockValue(SensorType::Clt, 60);
		setLambda(1.1f, 0.9f);
	}

	void TearDown() override {
		resetCalibrationTimerForTest();
	}

	void setLambda(float bank1, float bank2) {
		Sensor::setMockValue(SensorType::Lambda1, bank1);
		Sensor::setMockValue(SensorType::Lambda2, bank2);
	}

	ClosedLoopFuelResult learn(int ticks) {
		ClosedLoopFuelResult result;
		for (int i = 0; i < ticks; i++) {
			eth.moveTimeForwardMs(FAST_CALLBACK_PERIOD_MS);
			result = stft.getCorrection(3000, 50);
		}
		return result;
	}

	void expectBanks(const ClosedLoopFuelResult& result, float bank1, float bank2) {
		EXPECT_NEAR(bank1, result.banks[0], 1e-6f);
		EXPECT_NEAR(bank2, result.banks[1], 1e-6f);
	}

	void expectDisabled(stft_state_e reason) {
		expectBanks(stft.getCorrection(3000, 50), 1, 1);
		EXPECT_EQ(reason, stft.stftCorrectionState);
		EXPECT_EQ(reason, stft.stftLearningState[0]);
		EXPECT_EQ(reason, stft.stftLearningState[1]);
	}
};

TEST_F(ShortTermFuelTrimBehavior, BanksAndOperatingRegionsKeepIndependentCorrections) {
	// 50 ms at a 100 ms time constant and +/-0.1 lambda error -> +/-5%.
	expectBanks(learn(50 / FAST_CALLBACK_PERIOD_MS), 1.05f, 0.95f);
	EXPECT_EQ(ftRegionCruise, stft.stftCorrectionBinIdx);
	EXPECT_EQ(ftRegionCruise, stft.stftLearningBinIdx);
	EXPECT_NEAR(0.1f, stft.stftInputError[0], 1e-6f);
	EXPECT_NEAR(-0.1f, stft.stftInputError[1], 1e-6f);

	setLambda(1, 1);
	for (const auto& point : {std::pair<float, float>{1000, 50}, {3000, 10}, {3000, 90}}) {
		expectBanks(stft.getCorrection(point.first, point.second), 1, 1);
	}
	auto result = stft.getCorrection(3000, 50);
	EXPECT_EQ(ftRegionCruise, result.region);
	expectBanks(result, 1.05f, 0.95f);
}

TEST_F(ShortTermFuelTrimBehavior, FailedLambdaPausesOnlyItsBankAndRecovers) {
	expectBanks(learn(50 / FAST_CALLBACK_PERIOD_MS), 1.05f, 0.95f);
	Sensor::setInvalidMockValue(SensorType::Lambda1);
	expectBanks(learn(50 / FAST_CALLBACK_PERIOD_MS), 1.05f, 0.90f);
	EXPECT_EQ(stftEnabled, stft.stftCorrectionState);
	EXPECT_EQ(stftDisabledAfrOurOfRange, stft.stftLearningState[0]);
	EXPECT_EQ(stftEnabled, stft.stftLearningState[1]);

	setLambda(0.9f, 1);
	expectBanks(learn(50 / FAST_CALLBACK_PERIOD_MS), 1, 0.90f);
	EXPECT_EQ(stftEnabled, stft.stftLearningState[0]);
}

TEST_F(ShortTermFuelTrimBehavior, DisabledCorrectionReturnsUnityWithoutErasingLearnedTrim) {
	expectBanks(learn(50 / FAST_CALLBACK_PERIOD_MS), 1.05f, 0.95f);
	engineConfiguration->fuelClosedLoopCorrectionEnabled = false;
	expectDisabled(stftDisabledSettings);
	engineConfiguration->fuelClosedLoopCorrectionEnabled = true;

	engine->rpmCalculator.setRpmValue(0);
	expectDisabled(stftDisabledRPM);
	engine->rpmCalculator.setRpmValue(3000);
	engine->fuelComputer.running.timeSinceCrankingInSecs = 9;
	expectDisabled(stftDisabledCrankingDelay);
	engine->fuelComputer.running.timeSinceCrankingInSecs = 10;

	Sensor::setInvalidMockValue(SensorType::Clt);
	expectDisabled(stftDisabledClt);
	Sensor::setMockValue(SensorType::Clt, 59);
	expectDisabled(stftDisabledClt);
	Sensor::setMockValue(SensorType::Clt, 60);

	// Both warmup thresholds are inclusive. Hold lambda on target to observe
	// the saved correction without learning another increment. Restarting RPM
	// invokes a fast callback which recalculates target lambda from the tune.
	setLambda(engine->fuelComputer.targetLambda, engine->fuelComputer.targetLambda);
	expectBanks(stft.getCorrection(3000, 50), 1.05f, 0.95f);
	EXPECT_EQ(stftEnabled, stft.stftCorrectionState);
}

TEST_F(ShortTermFuelTrimBehavior, VeWriteSuspendsCorrectionUntilTuningTimeout) {
	expectBanks(learn(50 / FAST_CALLBACK_PERIOD_MS), 1.05f, 0.95f);
	engineConfiguration->isTuningDetectorEnabled = true;
	::testing::NiceMock<MockTsChannel> channel;
	TunerStudio ts;
	const uint16_t offset = static_cast<uint16_t>(offsetof(persistent_config_s, veTable));
	uint8_t dummy = 0;
	ts.handleWriteChunkCommand(&channel, TS_PAGE_SETTINGS, offset, 1, &dummy);
	ASSERT_TRUE(isTuningVeNow());
	expectDisabled(stftDisabledTuning);
	eth.moveTimeForwardMs(19999);
	expectDisabled(stftDisabledTuning);
	eth.moveTimeForwardMs(2);
	setLambda(1, 1);
	expectBanks(stft.getCorrection(3000, 50), 1.05f, 0.95f);
	EXPECT_EQ(stftEnabled, stft.stftCorrectionState);
	EXPECT_FALSE(engine->outputChannels.isTuningNow);
}

TEST_F(ShortTermFuelTrimBehavior, DfcoPausesLearningAndResumesAtHoldoffBoundary) {
	expectBanks(learn(50 / FAST_CALLBACK_PERIOD_MS), 1.05f, 0.95f);
	engineConfiguration->coastingFuelCutEnabled = true;
	engineConfiguration->coastingFuelCutClt = 30;
	engineConfiguration->coastingFuelCutMap = 100;
	engineConfiguration->coastingFuelCutTps = 2;
	engineConfiguration->coastingFuelCutRpmHigh = 1500;
	engineConfiguration->coastingFuelCutRpmLow = 1300;
	engineConfiguration->coastingFuelCutVssHigh = 0;
	engineConfiguration->coastingFuelCutVssLow = 0;
	engineConfiguration->dfcoDelay = 0;
	Sensor::setMockValue(SensorType::Map, 20);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);
	auto& dfco = engine->module<DfcoController>().unmock();
	dfco.update();
	ASSERT_TRUE(dfco.cutFuel());
	expectBanks(stft.getCorrection(3000, 50), 1.05f, 0.95f);
	EXPECT_EQ(stftDisabledDFCO, stft.stftLearningState[0]);

	Sensor::setMockValue(SensorType::DriverThrottleIntent, 20);
	dfco.update();
	ASSERT_FALSE(dfco.cutFuel());
	eth.moveTimeForwardMs(499);
	expectBanks(stft.getCorrection(3000, 50), 1.05f, 0.95f);
	EXPECT_EQ(stftDisabledDFCO, stft.stftLearningState[0]);
	eth.moveTimeForwardMs(1);
	EXPECT_GT(stft.getCorrection(3000, 50).banks[0], 1.05f);
	EXPECT_EQ(stftEnabled, stft.stftLearningState[0]);
}

TEST_F(ShortTermFuelTrimBehavior, FuelAndSparkCutsPauseLearningForTwoSeconds) {
	expectBanks(learn(50 / FAST_CALLBACK_PERIOD_MS), 1.05f, 0.95f);
	auto& limp = engine->module<LimpManager>().unmock();
	for (bool fuelCut : {true, false}) {
		SCOPED_TRACE(fuelCut ? "fuel cut" : "spark cut");
		engine->engineState.lua.luaFuelCut = fuelCut;
		engine->engineState.lua.luaIgnCut = !fuelCut;
		limp.updateState(3000, getTimeNowNt());
		expectBanks(stft.getCorrection(3000, 50), 1.05f, 0.95f);
		EXPECT_EQ(stftDisabledFuelCut, stft.stftLearningState[0]);

		engine->engineState.lua.luaFuelCut = false;
		engine->engineState.lua.luaIgnCut = false;
		limp.updateState(3000, getTimeNowNt());
		ASSERT_TRUE(limp.allowInjection());
		ASSERT_TRUE(limp.allowIgnition());
		eth.moveTimeForwardMs(1999);
		expectBanks(stft.getCorrection(3000, 50), 1.05f, 0.95f);
		EXPECT_EQ(stftDisabledFuelCut, stft.stftLearningState[0]);
		eth.moveTimeForwardMs(1);
		setLambda(1, 1);
		expectBanks(stft.getCorrection(3000, 50), 1.05f, 0.95f);
		EXPECT_EQ(stftEnabled, stft.stftLearningState[0]);
		setLambda(1.1f, 0.9f);
	}
}

TEST_F(ShortTermFuelTrimBehavior, CorrectionSaturatesAndCanMoveAwayFromBothLimits) {
	expectBanks(learn(1000 / FAST_CALLBACK_PERIOD_MS), 1.1f, 0.9f);
	setLambda(0.9f, 1.1f);
	expectBanks(learn(50 / FAST_CALLBACK_PERIOD_MS), 1.05f, 0.95f);

	// Even the largest representable calibration (25.5%) is capped at 25%.
	for (auto& cell : engineConfiguration->stft.cellCfgs) {
		cell.maxAdd = 25.5f;
		cell.maxRemove = 25.5f;
	}
	expectBanks(learn(1000 / FAST_CALLBACK_PERIOD_MS), 0.75f, 1.25f);
}

TEST_F(ShortTermFuelTrimBehavior, ConstantRateUsesErrorDirectionAndHonorsDeadband) {
	engineConfiguration->stftIgnoreErrorMagnitude = true;
	setLambda(1.02f, 0.8f);
	expectBanks(learn(50 / FAST_CALLBACK_PERIOD_MS), 1.05f, 0.95f);
	setLambda(1.005f, 0.995f);
	expectBanks(learn(100 / FAST_CALLBACK_PERIOD_MS), 1.05f, 0.95f);
}

TEST_F(ShortTermFuelTrimBehavior, IntegrationUsesTargetLambdaAndClampsTimeConstant) {
	engine->fuelComputer.targetLambda = 0.9f;
	setLambda(1, 0.8f);
	for (auto& cell : engineConfiguration->stft.cellCfgs) {
		cell.timeConstant = 0;
	}
	// A zero calibration uses the 100 ms minimum instead of dividing by zero.
	expectBanks(learn(50 / FAST_CALLBACK_PERIOD_MS), 1.05f, 0.95f);
	for (auto& cell : engineConfiguration->stft.cellCfgs) {
		cell.timeConstant = 1000;
	}
	// At the 100 second maximum, another 50 ms changes trim by 0.005%.
	expectBanks(learn(50 / FAST_CALLBACK_PERIOD_MS), 1.05005f, 0.94995f);
}

} // namespace
