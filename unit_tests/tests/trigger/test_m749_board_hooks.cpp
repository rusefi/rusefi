// Selected trigger/VR regression tests from fc961ebcb56 on maccan-tx-fix.
#include "pch.h"
#include "board_overrides.h"

struct TriggerDebounceScope {
	decltype(custom_board_triggerDebounceUs) previous = custom_board_triggerDebounceUs;

	TriggerDebounceScope() {
		custom_board_triggerDebounceUs = []() { return 1000.75f; };
	}

	~TriggerDebounceScope() {
		custom_board_triggerDebounceUs = previous;
	}
};

TEST(trigger, debounceIntegerMicrosecondBoundary) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	TriggerDebounceScope debounce;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);
	auto& central = engine->triggerCentral;
	const efitick_t start = US2NT(1'000'000'000LL);

	central.handleShaftSignal(SHAFT_PRIMARY_RISING, start);
	ASSERT_EQ(0u, central.triggerDebounceDropCount);
	// Opposite polarity has its own window.
	central.handleShaftSignal(SHAFT_PRIMARY_FALLING, start + US2NT(500));
	ASSERT_EQ(0u, central.triggerDebounceDropCount);
	central.handleShaftSignal(SHAFT_PRIMARY_RISING, start + US2NT(1000) - 1);
	ASSERT_EQ(1u, central.triggerDebounceDropCount);
	// Preserve truncation to whole microseconds; a dropped edge does not extend the window.
	central.handleShaftSignal(SHAFT_PRIMARY_RISING, start + US2NT(1000));
	ASSERT_EQ(1u, central.triggerDebounceDropCount);
	central.handleShaftSignal(SHAFT_PRIMARY_FALLING, start + US2NT(1500));
	ASSERT_EQ(1u, central.triggerDebounceDropCount);
}

struct SyncAcceptToothLossScope {
	SyncAcceptToothLossScope() {
		custom_board_syncAcceptToothLoss = []() { return true; };
	}

	~SyncAcceptToothLossScope() {
		custom_board_syncAcceptToothLoss = std::nullopt;
	}
};

struct SyncEarlyGapWhileCrankingScope {
	SyncEarlyGapWhileCrankingScope() {
		custom_board_syncEarlyGapWhileCranking = []() { return true; };
	}

	~SyncEarlyGapWhileCrankingScope() {
		custom_board_syncEarlyGapWhileCranking = std::nullopt;
	}
};

struct SyncGapAnchorCorrectionScope {
	SyncGapAnchorCorrectionScope() {
		custom_board_syncGapAnchorCorrection = []() { return true; };
	}

	~SyncGapAnchorCorrectionScope() {
		custom_board_syncGapAnchorCorrection = std::nullopt;
	}
};

static void fire60_2Revolution(EngineTestHelper& eth, float slotMs, float gapRatio) {
	// 58 real teeth: rise every slotMs, fall slotMs/2 after each rise
	for (int i = 0; i < 58; i++) {
		eth.fireRise(slotMs / 2);
		eth.fireFall(slotMs / 2);
	}

	// Tooth 57's fall happened slotMs/2 after its rise. Wait so that the next
	// call's first rise lands gapRatio * slotMs after tooth 57's rise (the
	// next call's first fireRise adds slotMs/2 of its own).
	eth.moveTimeForwardUs(static_cast<int>(MS2US((gapRatio - 1.0f) * slotMs)));
}

static void fire60_2RevolutionWithLostEvents(EngineTestHelper& eth, float slotMs, int missing, float gapRatio) {
	const int mergePos = 30;
	for (int i = 0; i < 58 - missing; i++) {
		// rise-to-rise duration for this tooth: the merged tooth carries the
		// time of the lost teeth (their events never arrive)
		float riseToRise = (i == mergePos) ? slotMs * (missing + 1) : slotMs;

		// the previous fall happened slotMs/2 after the previous rise
		eth.moveTimeForwardUs(static_cast<int>(MS2US(riseToRise - slotMs / 2)));
		eth.firePrimaryTriggerRise();
		eth.moveTimeForwardUs(static_cast<int>(MS2US(slotMs / 2)));
		eth.firePrimaryTriggerFall();
	}

	// trailing wait: the next call's first rise lands gapRatio * slotMs after the last rise
	eth.moveTimeForwardUs(static_cast<int>(MS2US((gapRatio - 1.0f) * slotMs)));
}

static float observedShifts[16];
static size_t observedShiftCount;

void triggerObserveGapShift(float measuredPitch) {
	if (observedShiftCount < efi::size(observedShifts)) {
		observedShifts[observedShiftCount++] = measuredPitch;
	}
}

TEST(trigger, crankingTransition60_2ToothLossAcceptedWhileRunning) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	SyncAcceptToothLossScope acceptLoss;
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// 2500 rpm steady revolutions to synchronize
	static constexpr float runningSlotMs = 0.4f;
	fire60_2Revolution(eth, runningSlotMs, 3.0f);
	fire60_2Revolution(eth, runningSlotMs, 3.0f);

	ASSERT_GT(Sensor::getOrZero(SensorType::Rpm), engineConfiguration->cranking.rpm) << "running rpm above the cranking band";
	// the first sync (rev2's first rise) does not increment the counter:
	// onShaftSynchronization counts only wasSynchronized=true syncs
	ASSERT_EQ(0, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter after the first sync";

	// 2 events lost at running rpm (the merged-tooth harness; on the car the
	// chip eats a single edge - the acceptance covers both)
	fire60_2RevolutionWithLostEvents(eth, runningSlotMs, /*missing*/2, /*gapRatio*/3.0f);
	fire60_2Revolution(eth, runningSlotMs, 3.0f);

	// the merged tooth trips the bad-tooth-timing warning at running rpm,
	// but the C9003 desync must be gone
	bool hasNotEnoughTeeth = false;
	for (size_t i = 0; i < getRecentWarnings()->getCount(); i++) {
		if (getRecentWarnings()->get(i).Code == ObdCode::CUSTOM_PRIMARY_NOT_ENOUGH_TEETH) {
			hasNotEnoughTeeth = true;
		}
	}
	ASSERT_FALSE(hasNotEnoughTeeth) << "no C9003 with the tooth-loss tolerance at running rpm";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "still synchronized through the lost tooth";
	ASSERT_EQ(2, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter through the accepted tooth loss";

	// one more steady revolution: everything continues normally
	fire60_2Revolution(eth, runningSlotMs, 3.0f);

	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());
	ASSERT_EQ(3, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter after the accepted tooth loss";
}

TEST(trigger, crankingTransition60_2ToothLossDesyncsWhenRunningWithoutTolerance) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	SyncEarlyGapWhileCrankingScope earlyGap;
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// 2500 rpm steady revolutions to synchronize
	static constexpr float runningSlotMs = 0.4f;
	fire60_2Revolution(eth, runningSlotMs, 3.0f);
	fire60_2Revolution(eth, runningSlotMs, 3.0f);

	// 2 events lost at running speed: no tolerance active -> C9003 + desync
	fire60_2RevolutionWithLostEvents(eth, runningSlotMs, /*missing*/2, /*gapRatio*/3.0f);
	fire60_2Revolution(eth, runningSlotMs, 3.0f);

	bool hasNotEnoughTeeth = false;
	for (size_t i = 0; i < getRecentWarnings()->getCount(); i++) {
		if (getRecentWarnings()->get(i).Code == ObdCode::CUSTOM_PRIMARY_NOT_ENOUGH_TEETH) {
			hasNotEnoughTeeth = true;
		}
	}
	ASSERT_TRUE(hasNotEnoughTeeth) << "C9003 without the running tolerance";
	ASSERT_FALSE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "desynced without the running tolerance";

	// the next steady revolution re-synchronizes cleanly
	fire60_2Revolution(eth, runningSlotMs, 3.0f);

	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "re-synchronized";
}

TEST(trigger, crankingTransition60_2GapAnchorCorrectionAppliesModel) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	SyncGapAnchorCorrectionScope anchorCorr;
	custom_board_vrGapShiftPitch = []() { return 0.64f; };
	struct ResetVrModel {
		~ResetVrModel() { custom_board_vrGapShiftPitch = std::nullopt; }
	} resetVrModel;
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// 2500 rpm with the compressed spurious-edge gap (2.36)
	static constexpr float runningSlotMs = 0.4f;
	fire60_2Revolution(eth, runningSlotMs, 2.36f);
	fire60_2Revolution(eth, runningSlotMs, 2.36f);

	// the compressed gap stays inside the [1.6, 4.5] sync windows: no desync
	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "compressed gap is a valid sync";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());

	// the model value applies directly: 0.64 pitch * 6 deg = 3.84 deg, at the
	// FIRST sync (no EMA convergence needed)
	EXPECT_NEAR(3.84f, engine->triggerCentral.gapAnchorCorrectionDeg, 0.01f) << "model value applied";

	// the model switches - the correction follows on the next sync
	custom_board_vrGapShiftPitch = []() { return 0.5f; };
	fire60_2Revolution(eth, runningSlotMs, 2.36f);
	EXPECT_NEAR(3.0f, engine->triggerCentral.gapAnchorCorrectionDeg, 0.01f) << "model switch applied immediately";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());
}

TEST(trigger, crankingTransition60_2GapAnchorCorrectionTrainsFromInBandOnly) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	SyncGapAnchorCorrectionScope anchorCorr;
	custom_board_vrGapShiftPitch = []() { return 0.64f; };
	struct ResetVrModel {
		~ResetVrModel() { custom_board_vrGapShiftPitch = std::nullopt; }
	} resetVrModel;
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	observedShiftCount = 0;
	static constexpr float runningSlotMs = 0.4f;

	// compressed gaps: 3 revs -> 2 syncs -> 2 observations of 0.64 pitch
	fire60_2Revolution(eth, runningSlotMs, 2.36f);
	fire60_2Revolution(eth, runningSlotMs, 2.36f);
	fire60_2Revolution(eth, runningSlotMs, 2.36f);
	ASSERT_EQ(2u, observedShiftCount) << "in-band measurements are observed";
	EXPECT_NEAR(0.64f, observedShifts[0], 0.01f) << "observed shift = 3.0 - 2.36";

	// stretched gaps (3.6): the FIRST stretched call still checks the last
	// compressed rev (one more observation), then every stretched rev checks
	// out of band and must not train
	fire60_2Revolution(eth, runningSlotMs, 3.6f);
	fire60_2Revolution(eth, runningSlotMs, 3.6f);
	fire60_2Revolution(eth, runningSlotMs, 3.6f);
	ASSERT_EQ(3u, observedShiftCount) << "stretched gaps never train the model";
	EXPECT_NEAR(3.84f, engine->triggerCentral.gapAnchorCorrectionDeg, 0.01f) << "model value unchanged";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());
}
