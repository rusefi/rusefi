/*
 * @file	test_long_term_fuel_trim.cpp
 *
 * @date Jun 6, 2025
 * @author Andrey Gusakov, 2025
 * @author Andrey Belomutskiy, (c) 2012-2025
 */

#include "pch.h"
#include "limp_manager.h"

static void checkLtftLearnsWhileStftIsFrozen(bool fuelCut) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& stft = engine->module<ShortTermFuelTrim>().unmock();
	stft.init(&engineConfiguration->stft);
	engineConfiguration->fuelClosedLoopCorrectionEnabled = true;
	engineConfiguration->stft.startupDelay = 0;
	engineConfiguration->stft.minClt = 0;
	engineConfiguration->stft.minAfr = 10;
	engineConfiguration->stft.maxAfr = 18;
	engineConfiguration->stft.deadband = 0;
	engineConfiguration->stftIgnoreErrorMagnitude = false;
	engineConfiguration->noFuelTrimAfterDfcoTime = 0;
	engineConfiguration->noFuelTrimAfterAccelTime = 0;
	engineConfiguration->launchControlEnabled = false;
	for (auto& cell : engineConfiguration->stft.cellCfgs) {
		cell.timeConstant = 0.1f;
		cell.maxAdd = 25;
		cell.maxRemove = 25;
	}
	for (size_t i = 0; i < VE_RPM_COUNT; i++) config->veRpmBins[i] = 500 + 500 * i;
	for (size_t i = 0; i < VE_LOAD_COUNT; i++) config->veLoadBins[i] = 10 + 10 * i;
	engine->rpmCalculator.setRpmValue(1000);
	engine->fuelComputer.targetLambda = 1;
	Sensor::setMockValue(SensorType::Clt, 80);
	Sensor::setMockValue(SensorType::Lambda1, 1.2f);
	Sensor::setMockValue(SensorType::Lambda2, 1);
	eth.moveTimeForwardUs(3000000);
	getLimpManager()->updateState(1000, getTimeNowNt());
	ClosedLoopFuelResult correction;
	for (int i = 0; i < 10; i++) {
		correction = stft.getCorrection(1000, 50);
		eth.moveTimeForwardUs(5000);
	}
	ASSERT_EQ(stftEnabled, stft.stftCorrectionState);
	ASSERT_EQ(stftEnabled, stft.stftLearningState[0]);
	ASSERT_NEAR(1.1f, correction.banks[0], 0.0001f);
	ASSERT_FLOAT_EQ(1, correction.banks[1]);

	LtftState state;
	LongTermFuelTrim ltft;
	ltft.init(&state);
	engineConfiguration->ltft.enabled = true;
	engineConfiguration->ltft.correctionEnabled = true;
	engineConfiguration->ltft.deadband = 0;
	engineConfiguration->ltft.maxAdd = 25;
	engineConfiguration->ltft.maxRemove = 25;
	setArrayValues(engineConfiguration->ltft.timeConstant, 1);
	if (fuelCut) {
		engine->engineState.lua.luaFuelCut = true;
	} else {
		Sensor::setInvalidMockValue(SensorType::Lambda1);
	}

	// Use the production STFT -> LTFT handoff for one second at 200 Hz.
	for (int i = 0; i < 200; i++) {
		getLimpManager()->updateState(1000, getTimeNowNt());
		correction = stft.getCorrection(1000, 50);
		ltft.learn(correction, 1000, 50);
		eth.moveTimeForwardUs(5000);
	}
	EXPECT_EQ(stftEnabled, stft.stftCorrectionState);
	EXPECT_EQ(fuelCut ? stftDisabledFuelCut : stftDisabledAfrOurOfRange, stft.stftLearningState[0]);
	EXPECT_NEAR(1.1f, correction.banks[0], 0.0001f); // frozen short-term value
	EXPECT_TRUE(ltft.ltftLearning);
	auto trims = ltft.getTrims(1000, 50);
	EXPECT_GT(trims.banks[0], 1.06f); // persisted-learning output grows without new feedback
	EXPECT_FLOAT_EQ(1, trims.banks[1]);

	// A global correction disable does stop LTFT; the per-bank gate is missing.
	engineConfiguration->fuelClosedLoopCorrectionEnabled = false;
	ltft.learn(stft.getCorrection(1000, 50), 1000, 50);
	EXPECT_FALSE(ltft.ltftLearning);
	EXPECT_FLOAT_EQ(trims.banks[0], ltft.getTrims(1000, 50).banks[0]);
}

TEST(LTFT, InvalidLambdaStillLearnsFrozenShortTermCorrection) {
	checkLtftLearnsWhileStftIsFrozen(false);
}

TEST(LTFT, FuelCutStillLearnsFrozenShortTermCorrection) {
	checkLtftLearnsWhileStftIsFrozen(true);
}

#define ITERATE_TIME(time, action)	\
	for (size_t i = 0; i < (time * 1000 / FAST_CALLBACK_PERIOD_MS); i++) { \
		(action); \
	}

#define ASSERT_LTFT_RESULT(action, val0, val1)	\
	{ \
		ClosedLoopFuelResult ltftClResult = action;	\
		ASSERT_FLOAT_EQ(ltftClResult.banks[0], val0);	\
		ASSERT_FLOAT_EQ(ltftClResult.banks[1], val1);	\
	}

TEST(LTFT, testLearning)
{
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	LtftState ltftState;
	LongTermFuelTrim ltft;

	// reset to zero
	ltftState.load();
	ltft.init(&ltftState);

	engineConfiguration->ltft.enabled = false;
	engineConfiguration->ltft.correctionEnabled = false;
	engineConfiguration->ltft.deadband = 0.5; // %
	engineConfiguration->ltft.maxAdd = 15.0; // %
	engineConfiguration->ltft.maxRemove = 5; // %
	engineConfiguration->ltft.timeConstant[ftRegionIdle] = 30; // seconds
	engineConfiguration->ltft.timeConstant[ftRegionOverrun] = 30; // seconds
	engineConfiguration->ltft.timeConstant[ftRegionPower] = 30; // seconds
	engineConfiguration->ltft.timeConstant[ftRegionCruise] = 30; // seconds

	float rpm = config->veRpmBins[0];
	float load = config->veLoadBins[0];
	ClosedLoopFuelResult clInput;
	ClosedLoopFuelResult clResult;

	clInput.region = ftRegionIdle;
	clInput.banks[0] = clInput.banks[1] = 1.5;
	// Run for 100 sec
	ITERATE_TIME(100, ltft.learn(clInput, rpm, load))

	// Disabled should not affect
	ASSERT_LTFT_RESULT(ltft.getTrims(rpm, load), 1.0, 1.0);

	// Enabling
	engineConfiguration->ltft.enabled = true;
	engineConfiguration->ltft.correctionEnabled = true;

	// Run for 100 sec with positive correction
	clInput.banks[0] = clInput.banks[1] = 1.5;
	ITERATE_TIME(100, ltft.learn(clInput, rpm, load))

	// test hiting maxAdd limit
	ASSERT_LTFT_RESULT(ltft.getTrims(rpm, load), 1.15, 1.15);

	// Other cells should not be affected
	ASSERT_LTFT_RESULT(ltft.getTrims(5000, 50), 1.0, 1.0);

	// Run for 100 sec with negative correction
	clInput.banks[0] = clInput.banks[1] = 0.5;
	ITERATE_TIME(100, ltft.learn(clInput, rpm, load))

	// test hiting maxRemove limit
	ASSERT_LTFT_RESULT(ltft.getTrims(rpm, load), 0.95, 0.95);

	// Other cells should not be affected
	ASSERT_LTFT_RESULT(ltft.getTrims(5000, 50), 1.0, 1.0);

#if 0
	rpm = config->veRpmBins[0] - (config->veRpmBins[1] - config->veRpmBins[0]) - 1;
	load = config->veLoadBins[0] - (config->veLoadBins[1] - config->veLoadBins[0]) - 1;
	// we are not learning, buts but still calculating correction if far outside table
	// test far outside
	ASSERT_LTFT_RESULT(ltft.getTrims(rpm, load), 1.0, 1.0);
	ASSERT_LTFT_RESULT(ltft.getTrims(10000, 1000), 1.0, 1.0);
#endif
}

TEST(LTFT, pausedDuringTwoStepLaunch)
{
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	LtftState ltftState;
	LongTermFuelTrim ltft;

	// reset to zero
	ltftState.load();
	ltft.init(&ltftState);

	engineConfiguration->ltft.enabled = true;
	engineConfiguration->ltft.correctionEnabled = true;
	engineConfiguration->ltft.deadband = 0.5; // %
	engineConfiguration->ltft.maxAdd = 15.0; // %
	engineConfiguration->ltft.maxRemove = 5; // %
	engineConfiguration->ltft.timeConstant[ftRegionIdle] = 30; // seconds

	float rpm = config->veRpmBins[0];
	float load = config->veLoadBins[0];
	ClosedLoopFuelResult clInput;

	clInput.region = ftRegionIdle;
	clInput.banks[0] = clInput.banks[1] = 1.5;

	// Two-step launch activity in progress: learning is paused
	engineConfiguration->launchControlEnabled = true;
	engine->launchController.isLaunchCondition = true;
	ITERATE_TIME(100, ltft.learn(clInput, rpm, load))
	EXPECT_FALSE(ltft.ltftLearning);
	ASSERT_LTFT_RESULT(ltft.getTrims(rpm, load), 1.0, 1.0);

	// Same for the pre-launch window
	engine->launchController.isLaunchCondition = false;
	engine->launchController.isPreLaunchCondition = true;
	ITERATE_TIME(100, ltft.learn(clInput, rpm, load))
	EXPECT_FALSE(ltft.ltftLearning);
	ASSERT_LTFT_RESULT(ltft.getTrims(rpm, load), 1.0, 1.0);

	// Launch over - learning resumes
	engine->launchController.isPreLaunchCondition = false;
	ITERATE_TIME(100, ltft.learn(clInput, rpm, load))
	EXPECT_TRUE(ltft.ltftLearning);
	ASSERT_LTFT_RESULT(ltft.getTrims(rpm, load), 1.15, 1.15);
}

TEST(LTFT, testSlowCallbackLoadError) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	LtftState ltftState;
	LongTermFuelTrim ltft;

	// reset to zero
	ltftState.load();
	ltft.init(&ltftState);

	// mock loading state
	ltft.ltftLearning = false;
	ltft.ltftLoadPending = true;
	engine->rpmCalculator.setRpmValue(1000);

	ltft.onSlowCallback();
	// still in loading time:
	EXPECT_TRUE(ltft.ltftLoadPending);
	EXPECT_FALSE(ltft.ltftLoadError);

	ltft.ltftLoadPending = true;

	// too much time loading!
	advanceTimeUs(8'000'000);

	ltft.onSlowCallback();
	EXPECT_FALSE(ltft.ltftLoadPending);
	EXPECT_TRUE(ltft.ltftLoadError);
}
