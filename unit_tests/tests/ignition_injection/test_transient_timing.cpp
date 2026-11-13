/**
 * @file test_transient_timing.cpp
 *
 * [tag:transient_timing] Signed ignition correction during TPS events: table and curve,
 * hold and decay, limits, knock interlock, Log only, and how fast it reaches the sparks.
 */

#include "pch.h"
#include "defaults.h"
#include "transient_timing.h"

class TransientTimingTest : public ::testing::Test {
protected:
	EngineTestHelper eth{engine_type_e::TEST_ENGINE};
	efitick_t nowNt = 0;

	void SetUp() override {
		setDefaultTransientTiming();
		config->transientTimingMode = TRANSIENT_TIMING_APPLY;
		// Rows are the TPS change {5, 20, 50, 80} %, columns RPM {1000..4000}.
		for (size_t row = 0; row < TRANSIENT_TIMING_SIZE; row++) {
			for (size_t col = 0; col < TRANSIENT_TIMING_SIZE; col++) {
				config->transientTimingOpeningTable[row][col] = 2.0f * (row + 1);
			}
		}
		setArrayValues(config->transientTimingClosingValues, -6);
		config->transientTimingMaxAdvance = 20;
		config->transientTimingMaxRetard = 30;

		engine->rpmCalculator.setRpmValue(1500);
		Sensor::setMockValue(SensorType::Rpm, 1500);
		ASSERT_TRUE(engine->rpmCalculator.isRunning());
		ASSERT_FALSE(engine->rpmCalculator.isCranking());
		ae().resetAE();
		tt().reset();
		nowNt = getTimeNowNt();
	}

	TpsAccelEnrichment& ae() {
		return engine->module<TpsAccelEnrichment>().unmock();
	}

	TransientTiming& tt() {
		return engine->module<TransientTiming>().unmock();
	}

	// One 5 ms fast-callback step with the given AE detector state.
	void tick(bool opening, bool closing, float deltaTps, int count = 1) {
		for (int i = 0; i < count; i++) {
			nowNt += US2NT(5000);
			ae().isAboveAccelThreshold = opening;
			ae().isBelowDecelThreshold = closing;
			ae().deltaTps = deltaTps;
			tt().update(nowNt);
		}
	}

	void opening(float change, int count = 1) {
		tick(true, false, change, count);
	}

	void closing(float change, int count = 1) {
		tick(false, true, -change, count);
	}

	void quiet(int count = 1) {
		tick(false, false, 0, count);
	}
};

TEST_F(TransientTimingTest, OpeningFollowsTheTableThenHoldsAndDecays) {
	// TPS change 20 %: table row 2, +4 deg.
	opening(20, 2);
	EXPECT_EQ(TransientTiming::State::Opening, tt().m_state);
	EXPECT_NEAR(4, tt().getTimingCorrection(), 1e-4f);
	EXPECT_NEAR(4, engine->ignitionState.transientTimingCorrection, 0.01f);
	quiet(); // event over: the 100 ms hold starts from the last opening sample
	EXPECT_EQ(TransientTiming::State::Hold, tt().m_state);
	EXPECT_NEAR(4, tt().getTimingCorrection(), 1e-4f);

	quiet(8); // 45 ms after the last opening sample
	EXPECT_NEAR(4, tt().getTimingCorrection(), 1e-4f);
	quiet(41); // 250 ms: 150 of the 300 ms decay
	EXPECT_EQ(TransientTiming::State::Decay, tt().m_state);
	EXPECT_NEAR(2, tt().getTimingCorrection(), 0.01f);
	quiet(30); // 400 ms: done
	EXPECT_EQ(TransientTiming::State::Idle, tt().m_state);
	EXPECT_FLOAT_EQ(0, tt().getTimingCorrection());
	EXPECT_EQ(0, engine->ignitionState.transientTimingState);
}

TEST_F(TransientTimingTest, BiggerOpeningUsesALaterRow) {
	opening(50); // row 3, +6 deg
	EXPECT_NEAR(6, tt().getTimingCorrection(), 1e-4f);
	// The value follows the TPS change while the event lasts.
	opening(80); // row 4, +8 deg
	EXPECT_NEAR(8, tt().getTimingCorrection(), 1e-4f);
}

TEST_F(TransientTimingTest, ClosingUsesTheClosingCurve) {
	closing(40);
	EXPECT_EQ(TransientTiming::State::Closing, tt().m_state);
	EXPECT_NEAR(-6, tt().getTimingCorrection(), 1e-4f);
	quiet(2);
	EXPECT_EQ(TransientTiming::State::Hold, tt().m_state);
	EXPECT_NEAR(-6, tt().getTimingCorrection(), 1e-4f);
}

TEST_F(TransientTimingTest, NewEventReplacesTheDecayingOne) {
	opening(20);
	quiet(60); // well into the decay
	ASSERT_EQ(TransientTiming::State::Decay, tt().m_state);
	ASSERT_LT(tt().getTimingCorrection(), 4);
	opening(60); // between rows 3 and 4
	EXPECT_EQ(TransientTiming::State::Opening, tt().m_state);
	EXPECT_NEAR(6.0f + 2.0f * (60 - 50) / (80 - 50), tt().getTimingCorrection(), 1e-3f);
}

TEST_F(TransientTimingTest, LimitsAndKnockInterlock) {
	config->transientTimingMaxAdvance = 3;
	config->transientTimingMaxRetard = 5;
	opening(20);
	EXPECT_NEAR(3, tt().getTimingCorrection(), 1e-4f);

	// No advance while knock control retards; retard still applies.
	engine->module<KnockController>()->m_knockRetard = 2;
	opening(20);
	EXPECT_FLOAT_EQ(0, tt().getTimingCorrection());
	closing(40);
	EXPECT_NEAR(-5, tt().getTimingCorrection(), 1e-4f);
	engine->module<KnockController>()->m_knockRetard = 0;
}

TEST_F(TransientTimingTest, LogOnlyShowsTheTargetWithoutApplying) {
	config->transientTimingMode = TRANSIENT_TIMING_LOG_ONLY;
	opening(20);
	EXPECT_NEAR(4, engine->ignitionState.transientTimingTarget, 0.01f);
	EXPECT_FLOAT_EQ(0, tt().getTimingCorrection());
	EXPECT_FLOAT_EQ(0, engine->ignitionState.transientTimingCorrection);
}

TEST_F(TransientTimingTest, OffOrStoppedDoesNothing) {
	config->transientTimingMode = TRANSIENT_TIMING_OFF;
	opening(20);
	EXPECT_FLOAT_EQ(0, tt().getTimingCorrection());
	EXPECT_EQ(TransientTiming::State::Idle, tt().m_state);

	config->transientTimingMode = TRANSIENT_TIMING_APPLY;
	opening(50);
	ASSERT_GT(tt().getTimingCorrection(), 0);
	engine->rpmCalculator.setRpmValue(0);
	opening(50);
	EXPECT_FLOAT_EQ(0, tt().getTimingCorrection());
	EXPECT_EQ(TransientTiming::State::Idle, tt().m_state);
}

TEST_F(TransientTimingTest, AddedToTheIgnitionTimingOnlyWhenApplied) {
	// An opening event with a 20 % TPS change, seen by the fast callback.
	ae().isAboveAccelThreshold = true;
	ae().deltaTps = 20;
	eth.moveTimeForwardMs(5);
	engine->periodicFastCallback();
	ASSERT_NEAR(4, tt().getTimingCorrection(), 1e-4f);
	float applied = engine->engineState.timingAdvance[0];
	float adjusted = engine->ignitionState.correctedIgnitionAdvance;

	// Same inputs, now in Log only: the timing loses the 4 degrees.
	config->transientTimingMode = TRANSIENT_TIMING_LOG_ONLY;
	eth.moveTimeForwardMs(5);
	engine->periodicFastCallback();
	EXPECT_NEAR(applied - 4, engine->engineState.timingAdvance[0], 0.01f);
	EXPECT_NEAR(adjusted - 4, engine->ignitionState.correctedIgnitionAdvance, 0.05f);

	// Fixed timing bypasses every correction.
	config->transientTimingMode = TRANSIENT_TIMING_APPLY;
	engineConfiguration->timingMode = TM_FIXED;
	engineConfiguration->fixedTiming = 10;
	eth.moveTimeForwardMs(5);
	engine->periodicFastCallback();
	EXPECT_NEAR(10, engine->ignitionState.correctedIgnitionAdvance, 0.05f);
}

TEST(TransientTimingDefaults, OffAndSeededForOldTunes) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	EXPECT_EQ(TRANSIENT_TIMING_OFF, config->transientTimingMode);
	EXPECT_EQ(1000, config->transientTimingRpmBins[0]);
	EXPECT_EQ(4000, config->transientTimingRpmBins[TRANSIENT_TIMING_SIZE - 1]);
	EXPECT_EQ(100, config->transientTimingHoldMs);
	EXPECT_EQ(300, config->transientTimingDecayMs);

	setArrayValues(config->transientTimingRpmBins, 0);
	EXPECT_TRUE(applyDefaultsOrFixAfterBurn(nullptr));
	EXPECT_EQ(4000, config->transientTimingRpmBins[TRANSIENT_TIMING_SIZE - 1]);
	EXPECT_EQ(TRANSIENT_TIMING_OFF, config->transientTimingMode);
}

// Whole trigger path: a 60-2 sequential inline-4. Each cylinder's spark angle is
// prepared when its previous spark fires, so a new correction reaches every cylinder
// within one engine cycle, and none of them before its next prepared spark.
TEST(TransientTimingIntegration, ReachesEachCylinderAtItsNextSpark) {
	EngineTestHelper eth(engine_type_e::MINIMAL_PINS);
	Sensor::setMockValue(SensorType::Clt, 80);
	Sensor::setMockValue(SensorType::Iat, 30);
	Sensor::setMockValue(SensorType::Map, 40);
	Sensor::setMockValue(SensorType::Tps1, 10);
	Sensor::setMockValue(SensorType::BatteryVoltage, 13.5f);
	// [tag:lambdaTable] Fuel-pipeline fixtures set the target explicitly.
	setTable(config->lambdaTable, 1.0f);
	EXPECT_CALL(*eth.mockAirmass, getAirmass(testing::_, testing::_))
		.WillRepeatedly(testing::Return(AirmassResult{0.15f, 40.0f}));
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->useIdleTimingPidControl = false;
	setDefaultTransientTiming();
	config->transientTimingMode = TRANSIENT_TIMING_APPLY;
	setTable(config->transientTimingOpeningTable, 4);
	config->transientTimingHoldMs = 2000;

	testSpinEngineUntilData spinInfo = { 0, 0, 0 };
	// The helper's RPM is double the crank-mounted wheel speed, see test_scheduling_pool.cpp.
	const int rpm = 2 * 1500;
	int deg = 720 * 4;
	eth.spin60_2UntilDeg(spinInfo, rpm, deg);
	ASSERT_NEAR(1500, Sensor::getOrZero(SensorType::Rpm), 20);
	// spin60_2UntilDeg() counts from spinInfo.currentDegree but never advances it: set it,
	// so that each call spins only the requested angle.
	auto fastCallbackAfter = [&](int degrees) {
		spinInfo.currentDegree = deg;
		deg += degrees;
		eth.spin60_2UntilDeg(spinInfo, rpm, deg);
		engine->periodicFastCallback();
	};
	for (int i = 0; i < 4; i++) {
		fastCallbackAfter(60);
	}
	// A full cycle so that every cylinder has prepared a spark with this timing.
	for (int i = 0; i < 8; i++) {
		fastCallbackAfter(90);
	}
	const size_t cylinders = engineConfiguration->cylindersCount;
	float before[MAX_CYLINDER_COUNT];
	for (size_t i = 0; i < cylinders; i++) {
		before[i] = engine->outputChannels.ignitionAdvanceCyl[i];
	}
	auto updatedCylinders = [&]() {
		int updated = 0;
		for (size_t i = 0; i < cylinders; i++) {
			if (std::abs(engine->outputChannels.ignitionAdvanceCyl[i] - (before[i] + 4)) < 0.05f) {
				updated++;
			}
		}
		return updated;
	};

	// The throttle opens: the correction is computed at once...
	auto& ae = engine->module<TpsAccelEnrichment>().unmock();
	ae.isAboveAccelThreshold = true;
	ae.deltaTps = 50;
	engine->periodicFastCallback();
	ASSERT_NEAR(4, engine->ignitionState.transientTimingCorrection, 0.01f);
	// ...but sparks that were already prepared keep their angle.
	EXPECT_EQ(0, updatedCylinders());

	// Half a cycle later some cylinders have it, a full cycle later all of them.
	for (int i = 0; i < 4; i++) {
		fastCallbackAfter(90);
	}
	int afterHalfCycle = updatedCylinders();
	EXPECT_GT(afterHalfCycle, 0);
	EXPECT_LT(afterHalfCycle, static_cast<int>(cylinders));
	for (int i = 0; i < 4; i++) {
		fastCallbackAfter(90);
	}
	EXPECT_EQ(static_cast<int>(cylinders), updatedCylinders());
}
