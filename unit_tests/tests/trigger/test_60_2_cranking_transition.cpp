/*
 * @file test_60_2_cranking_transition.cpp
 *
 * Reproduces the C9002 "too many teeth between sync points: expected 58/0 got 58/0"
 * failure seen on a 60-2 wheel when the crank changes speed hard at the
 * cranking -> running transition (first combustion).
 *
 * During that transition the missing-teeth gap ratio (nominally 3.0, one real
 * tooth gap is 1.0) moves outside the legacy [2.25, 3.75] window: the gap is
 * physically 3 tooth-slots long, but those 36 degrees take much less time than
 * 2.25x the preceding tooth when RPM is rising fast (and more than 3.75x when
 * the crank kicks back).
 *
 * The decoder then misses the sync point, counts exactly one full revolution
 * of teeth (58), and declares "too many teeth" right at the position where the
 * sync point should have been - the count is exactly right, only the gap ratio
 * check failed. It desynchronizes and the engine loses injection/ignition for
 * a revolution, exactly when it just started firing (user report: the engine
 * catches, C9002 appears, then it just spins without running).
 *
 * The acceleration case is fixed by the cranking-transition sync windows
 * (gap [1.6, 3.75], second gap [0.8, 1.2]): the transition revolution stays
 * synchronized, no warning, RPM continuous. Extreme deceleration (ratio above
 * 3.75) is intentionally still rejected and must recover on the next
 * revolution - documented below.
 */

#include "pch.h"
#include "board_overrides.h"
#include <random>

// The m74_9 board opts into the cranking-band sync-by-position skip; the
// decoder-level tests below set the same override. Reset at test end - the
// override global persists across tests in this binary.
struct SyncByPositionWhileCrankingScope {
	SyncByPositionWhileCrankingScope() {
		custom_board_syncByPositionWhileCranking = []() { return true; };
	}

	~SyncByPositionWhileCrankingScope() {
		custom_board_syncByPositionWhileCranking = std::nullopt;
	}
};

/**
 * Fire one full 60-2 revolution: 58 real teeth, then the missing-teeth gap.
 * The gap ratio of THIS call is checked at the NEXT call's first rise.
 *
 * slotMs:    rise-to-rise duration between normal teeth
 * gapRatio:  rise-to-rise duration of the missing-teeth gap in units of slotMs
 *            (physically 3.0: 2 skipped teeth + the normal gap)
 *
 * The waveform places the missing teeth at the end of the cycle, so the gap is
 * between the last real tooth (57) and the first tooth of the next revolution.
 */
static void fire60_2Revolution(EngineTestHelper& eth, float slotMs, float gapRatio) {
	// 58 real teeth: rise every slotMs, fall slotMs/2 after each rise
	for (int i = 0; i < 58; i++) {
		eth.fireRise(slotMs / 2);
		eth.fireFall(slotMs / 2);
	}

	// Tooth 57's fall happened slotMs/2 after its rise. Wait so that the next
	// call's first rise lands gapRatio * slotMs after tooth 57's rise (the
	// next call's first fireRise adds slotMs/2 of its own).
	eth.moveTimeForwardUs(MS2US((gapRatio - 1.0f) * slotMs));
}

// steady 400 RPM: 60 slots * 2.5 ms = 150 ms per revolution
static constexpr float steadySlotMs = 2.5f;
static constexpr float steadyRpm = 400;

TEST(trigger, crankingTransition60_2Acceleration) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// steady revolutions: the first does not sync yet (the decoder has not
	// seen a gap), the next ones synchronize it
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no warnings while cranking steadily";
	ASSERT_EQ(steadyRpm, round(Sensor::getOrZero(SensorType::Rpm))) << "RPM after sync";
	ASSERT_EQ(1, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter";

	// transition revolution: the crank accelerates, the missing-teeth gap
	// ratio collapses to 1.7 (outside the legacy [2.25, 3.75] window, inside
	// the cranking-transition [1.6, 3.75] window)
	fire60_2Revolution(eth, steadySlotMs, /*gapRatio*/1.7f);

	// the gap of the transition revolution is checked at this first rise
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	// the transition stays synchronized: no C9002, no sync loss
	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no warning through the acceleration transition";
	EXPECT_NEAR(steadyRpm, Sensor::getOrZero(SensorType::Rpm), 10) << "RPM through the transition";
	ASSERT_EQ(3, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync maintained through the transition";

	// one more steady revolution: everything continues normally
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(0u, getRecentWarnings()->getCount());
	EXPECT_NEAR(steadyRpm, Sensor::getOrZero(SensorType::Rpm), 10) << "RPM after the transition";
	ASSERT_EQ(4, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter after the transition";
}

/**
 * Fire one revolution with a distorted tooth pair mid-revolution - the
 * signature of a misfire-induced crank wobble on a running engine.
 * At tooth (pos+1)'s rise the decoder sees gap0 = distRatio and
 * gap1 = prevRatio (tooth pos is prevRatio, tooth pos+1 is
 * distRatio*prevRatio times the slot).
 * The sync gap at the end of the revolution stays gapRatio.
 */
static void fire60_2RevolutionWithDistortedTeethAt(EngineTestHelper& eth, float slotMs, int pos, float prevRatio, float distRatio, float gapRatio) {
	for (int i = 0; i < 58; i++) {
		// rise-to-rise duration for this tooth
		float riseToRise = slotMs;
		if (i == pos) riseToRise = prevRatio * slotMs;
		if (i == pos + 1) riseToRise = distRatio * prevRatio * slotMs;

		// the previous fall happened slotMs/2 after the previous rise
		eth.moveTimeForwardUs(MS2US(riseToRise - slotMs / 2));
		eth.firePrimaryTriggerRise();
		eth.moveTimeForwardUs(MS2US(slotMs / 2));
		eth.firePrimaryTriggerFall();
	}

	// trailing wait: the next call's first rise lands gapRatio * slotMs after tooth 57's rise
	eth.moveTimeForwardUs(MS2US((gapRatio - 1.0f) * slotMs));
}

static void fire60_2RevolutionWithDistortedTeeth(EngineTestHelper& eth, float slotMs, float prevRatio, float distRatio, float gapRatio) {
	fire60_2RevolutionWithDistortedTeethAt(eth, slotMs, 24, prevRatio, distRatio, gapRatio);
}

TEST(trigger, crankingTransition60_2MisfireDistortedTooth) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// steady revolutions to synchronize
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no warnings while cranking steadily";
	ASSERT_EQ(1, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter";

	// one revolution with a misfire-distorted tooth pair: gap0 ratio 1.601
	// (inside the widened window) with the pair before it at 1.195. On a
	// running m74_9 engine this exact pair false-synced mid-revolution
	// (C9003, expected 58 got 51) and killed the engine. The tightened
	// second gap rejects it: the distortion is just one noisy tooth, the
	// real gap at the end of the revolution re-synchronizes cleanly.
	fire60_2RevolutionWithDistortedTeeth(eth, steadySlotMs, /*prevRatio*/1.195f, /*distRatio*/1.601f, /*gapRatio*/3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no false sync from the distorted tooth pair";
	EXPECT_NEAR(steadyRpm, Sensor::getOrZero(SensorType::Rpm), 10) << "RPM through the distorted revolution";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "still synchronized";
	ASSERT_EQ(3, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter through the distorted revolution";
}

/**
 * The false-sync pair grows with RPM: on the car the mid-rev stretched pair
 * measured gap0=2.476 / gap1=1.284 at 402 rpm (firing) - BOTH inside the
 * 60-2 ratio windows, so no ratio window can reject it. It false-synced 42
 * teeth after the real sync point (C9003, expected 58 got 42) and killed the
 * engine. Only the position gate rejects it: once synchronized, a gap is only
 * accepted at the expected event position.
 */
TEST(trigger, crankingTransition60_2FalseSyncAtRunningRpmRejectedByPositionGate) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// steady revolutions to synchronize
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no warnings while cranking steadily";
	ASSERT_EQ(1, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter";

	// one revolution with the on-car false pair (both ratios inside the
	// windows) at a mid-rev position: the position gate must reject it
	fire60_2RevolutionWithDistortedTeeth(eth, steadySlotMs, /*prevRatio*/1.284f, /*distRatio*/2.476f, /*gapRatio*/3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no false sync from the in-window distorted tooth pair";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "still synchronized";
	ASSERT_EQ(3, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter through the distorted revolution";
}

/**
 * The 22:22:19 m74_9 log (cam disabled): at 292 rpm a mid-rev pair measured
 * gap0=3.333 / gap1=1.022 - gap0 is indistinguishable from the real missing
 * teeth gap (3.0-3.75), so the ratio check alone can never reject it. It
 * false-synced 31 teeth after the real gap (C9003, expected 58 got 31).
 * Only the position gate rejects it.
 */
TEST(trigger, crankingTransition60_2GapLikeFalseSyncRejectedByPositionGate) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// steady revolutions to synchronize
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no warnings while cranking steadily";
	ASSERT_EQ(1, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter";

	// one revolution with the on-car gap-like false pair at a mid-rev position
	fire60_2RevolutionWithDistortedTeeth(eth, steadySlotMs, /*prevRatio*/1.022f, /*distRatio*/3.333f, /*gapRatio*/3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no false sync from the gap-like distorted tooth pair";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "still synchronized";
	ASSERT_EQ(3, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter through the distorted revolution";
}

TEST(trigger, crankingTransition60_2DecelerationStaysSyncedWhileCranking) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	SyncByPositionWhileCrankingScope syncSkip;
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// steady revolutions to synchronize
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no warnings while cranking steadily";
	ASSERT_EQ(1, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter";

	// transition revolution: the crank kicks back, the gap stretches to 4.2x.
	// The ratio check rejects it, but the cranking-band sync-by-position skip
	// accepts the gap at the exact expected position (the tooth count proves
	// it IS the real gap) - the decoder stays synchronized through the kick.
	fire60_2Revolution(eth, steadySlotMs, /*gapRatio*/4.2f);

	// the stretched gap of the transition revolution is checked at this first rise
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no C9002 while cranking";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "still synchronized through the kick";
	ASSERT_EQ(3, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter";
}

/**
 * At the first-combustion catch the crank accelerates so hard that the REAL
 * missing-teeth gap compresses below the ratio window (1.4 here, window low
 * side 1.6). In the cranking band the sync-by-position skip accepts it and
 * the engine keeps running.
 */
TEST(trigger, crankingTransition60_2CompressedGapAcceptedWhileCranking) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	SyncByPositionWhileCrankingScope syncSkip;
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// steady revolutions to synchronize
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no warnings while cranking steadily";
	ASSERT_EQ(1, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter";

	// transition revolution: first combustion compresses the gap to 1.4x
	fire60_2Revolution(eth, steadySlotMs, /*gapRatio*/1.4f);

	// the compressed gap is checked at this first rise - the cranking-band
	// skip accepts it: no C9002, the sync survives the catch
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no C9002 at the catch";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "still synchronized through the catch";
	ASSERT_EQ(3, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter";
}

/**
 * Once running (rpm above 2 * crankingRpm) the sync-by-position skip is off:
 * a compressed gap at the expected position is rejected like before (the
 * crank speed is uniform when running, a 1.4x gap is not physical there).
 */
TEST(trigger, crankingTransition60_2CompressedGapRejectedWhenRunning) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	SyncByPositionWhileCrankingScope syncSkip;
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// 1250 rpm steady revolutions: above the 2 * crankingRpm skip band
	static constexpr float runningSlotMs = 0.8f;
	fire60_2Revolution(eth, runningSlotMs, 3.0f);
	fire60_2Revolution(eth, runningSlotMs, 3.0f);
	fire60_2Revolution(eth, runningSlotMs, 3.0f);

	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no warnings while running steadily";
	ASSERT_EQ(1, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter";
	ASSERT_GT(Sensor::getOrZero(SensorType::Rpm), 2 * engineConfiguration->cranking.rpm) << "running rpm above the skip band";

	// compressed gap revolution - the ratio check rejects it, the skip is off
	fire60_2Revolution(eth, runningSlotMs, /*gapRatio*/1.4f);
	fire60_2Revolution(eth, runningSlotMs, 3.0f);

	ASSERT_EQ(1u, getRecentWarnings()->getCount()) << "one C9002 at running rpm";
	EXPECT_EQ(ObdCode::CUSTOM_PRIMARY_TOO_MANY_TEETH, getRecentWarnings()->get(0).Code);

	// the next steady revolution re-synchronizes cleanly
	fire60_2Revolution(eth, runningSlotMs, 3.0f);

	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "re-synchronized";
}

/**
 * Every false-sync tooth pair captured on the m74_9 car over the whole
 * session (grep 'newerr TRG' across the logs). gap0/gap1 as printed by the
 * decoder at the desync event. Some pairs are out of window (rejected by the
 * ratio check itself), the dangerous ones have BOTH ratios inside the
 * windows - only the position gate rejects those. Each pair is replayed at
 * a mid-rev position followed by a clean revolution: the decoder must never
 * desync on a mid-rev pair.
 */
struct ObservedFalseSyncPair {
	float gap0;
	float gap1;
};

static const ObservedFalseSyncPair observedFalseSyncPairs[] = {
	// rpm=290, tooth 47, 20:48 session, window was 1.6-4.199
	{1.948f, 1.372f},
	// rpm=292, tooth 30, 22:22 session (cam disabled), gap0 looks like the real gap
	{3.333f, 1.022f},
	// rpm=331, tooth 16, 20:29 session
	{1.735f, 1.295f},
	// rpm=369, tooth 13, 20:48 session
	{2.776f, 1.274f},
	// rpm=402, tooth 41, 22:11 first start attempt
	{2.476f, 1.284f},
	// rpm=408, tooth 16, 20:48 session, gap0 below window - ratio-rejected
	{1.494f, 1.252f},
};

TEST(trigger, crankingTransition60_2AllObservedFalseSyncPairsRejected) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// steady revolutions to synchronize
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no warnings while cranking steadily";
	ASSERT_EQ(1, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter";

	for (size_t i = 0; i < efi::size(observedFalseSyncPairs); i++) {
		const auto& pair = observedFalseSyncPairs[i];
		size_t warningsBefore = getRecentWarnings()->getCount();

		// one revolution with the observed pair at a mid-rev position, then a
		// clean revolution so the real gap gets checked
		fire60_2RevolutionWithDistortedTeethAt(eth, steadySlotMs, /*pos*/24, pair.gap1, pair.gap0, /*gapRatio*/3.0f);
		fire60_2Revolution(eth, steadySlotMs, 3.0f);

		ASSERT_EQ(warningsBefore, getRecentWarnings()->getCount())
			<< "no false sync for observed pair #" << i << " gap0=" << pair.gap0 << " gap1=" << pair.gap1;
		ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized())
			<< "still synchronized through pair #" << i << " gap0=" << pair.gap0 << " gap1=" << pair.gap1;
	}
}

TEST(trigger, crankingTransition60_2RandomMidRevPairsNeverDesync) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// steady revolutions to synchronize
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no warnings while cranking steadily";
	ASSERT_EQ(1, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter";

	// Fixed seed: reproducible sweep over mid-rev distortion positions and
	// ratio combinations spanning the window boundaries. gap0 covers
	// 1.45-4.3 (below, inside and above the [1.6, 3.75] window), gap1 covers
	// 0.85-1.40 (around the [0.85, 1.35] window). A mid-rev pair must never
	// desync the decoder regardless of the ratios - the real gap at the end
	// of the revolution re-syncs cleanly.
	std::mt19937 gen(20260818);
	std::uniform_int_distribution<int> posDist(2, 54);
	std::uniform_real_distribution<float> prevDist(0.85f, 1.40f);
	std::uniform_real_distribution<float> distDist(1.45f, 4.30f);

	for (int i = 0; i < 50; i++) {
		int pos = posDist(gen);
		float prevRatio = prevDist(gen);
		float distRatio = distDist(gen);
		size_t warningsBefore = getRecentWarnings()->getCount();

		fire60_2RevolutionWithDistortedTeethAt(eth, steadySlotMs, pos, prevRatio, distRatio, /*gapRatio*/3.0f);
		fire60_2Revolution(eth, steadySlotMs, 3.0f);

		ASSERT_EQ(warningsBefore, getRecentWarnings()->getCount())
			<< "no false sync for random pair #" << i << " pos=" << pos
			<< " gap0=" << distRatio << " gap1=" << prevRatio;
		ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized())
			<< "still synchronized through random pair #" << i << " pos=" << pos
			<< " gap0=" << distRatio << " gap1=" << prevRatio;
	}
}
