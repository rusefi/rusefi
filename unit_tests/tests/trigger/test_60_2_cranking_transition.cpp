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

// Same pattern for the noise-storm sync-gap hardening (m74_9 opt-in).
struct SyncGapHardeningScope {
	SyncGapHardeningScope() {
		custom_board_syncGapHardening = []() { return true; };
	}

	~SyncGapHardeningScope() {
		custom_board_syncGapHardening = std::nullopt;
	}
};

// Same pattern for the cranking-band early-gap acceptance (m74_9 opt-in): a
// ratio-validated gap candidate arriving 1-2 events early (events LOST
// between the gaps) is accepted as a valid sync instead of C9003-desyncing.
struct SyncEarlyGapWhileCrankingScope {
	SyncEarlyGapWhileCrankingScope() {
		custom_board_syncEarlyGapWhileCranking = []() { return true; };
	}

	~SyncEarlyGapWhileCrankingScope() {
		custom_board_syncEarlyGapWhileCranking = std::nullopt;
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
 * Fire one revolution with one tooth missing (57 teeth instead of 58): the
 * gap is then checked with count 57 -> C9003 -> desync. This is the signature
 * of a real missed-tooth event that kicks off a desync -> re-sync storm.
 */
static void fire60_2RevolutionMissingTooth(EngineTestHelper& eth, float slotMs, float gapRatio) {
	for (int i = 0; i < 57; i++) {
		eth.fireRise(slotMs / 2);
		eth.fireFall(slotMs / 2);
	}

	// trailing wait: the next call's first rise lands gapRatio * slotMs after tooth 56's rise
	eth.moveTimeForwardUs(MS2US((gapRatio - 1.0f) * slotMs));
}

/**
 * Fire one revolution with 'missing' (1-2) events LOST: the lost teeth merge
 * into the tooth at mergePos, so that tooth takes (missing + 1) * slotMs rise
 * to rise and the revolution has (58 - missing) teeth total. The real
 * missing-teeth gap then arrives at count 58 - missing - the exact signature
 * of the m74_9 first-combustion catch (the L9779 VR conditioner swallows 1-2
 * teeth, the decoder's C9003 printed 'got 56/0').
 * The merged tooth's mid-rev gap0 of 2.0-3.0 is rejected by the position gate
 * (count ~31, far from the expected gap position).
 */
static void fire60_2RevolutionWithLostEvents(EngineTestHelper& eth, float slotMs, int missing, float gapRatio) {
	const int mergePos = 30;
	for (int i = 0; i < 58 - missing; i++) {
		// rise-to-rise duration for this tooth: the merged tooth carries the
		// time of the lost teeth (their events never arrive)
		float riseToRise = (i == mergePos) ? slotMs * (missing + 1) : slotMs;

		// the previous fall happened slotMs/2 after the previous rise
		eth.moveTimeForwardUs(MS2US(riseToRise - slotMs / 2));
		eth.firePrimaryTriggerRise();
		eth.moveTimeForwardUs(MS2US(slotMs / 2));
		eth.firePrimaryTriggerFall();
	}

	// trailing wait: the next call's first rise lands gapRatio * slotMs after the last rise
	eth.moveTimeForwardUs(MS2US((gapRatio - 1.0f) * slotMs));
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
	// crank tooth index of the pair, from the newerr TRG eventIndex (mod 58).
	// Informational: any mid-rev position exercises the same gate, the
	// decoder rejects the pair regardless.
	int position;
};

static const ObservedFalseSyncPair observedFalseSyncPairs[] = {
	// rpm=290, 20:48 session, eventIndex=94 (window 1.6-4.199)
	{1.948f, 1.372f, 36},
	// rpm=292, 22:22 session (cam disabled), eventIndex=60 - gap0 looks like the real gap
	{3.333f, 1.022f, 2},
	// rpm=331, 20:29 session, eventIndex=32
	{1.735f, 1.295f, 32},
	// rpm=369, 20:48 session, eventIndex=13
	{2.776f, 1.274f, 13},
	// rpm=402, 22:11 first start attempt, eventIndex=41
	{2.476f, 1.284f, 41},
	// rpm=408, 20:48 session, eventIndex=16, gap0 below window - ratio-rejected
	{1.494f, 1.252f, 16},
	// rpm=265, 18:03 session, eventIndex=22 - gap0 above the 3.75 window, ratio-rejected
	{3.913f, 1.328f, 22},
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

		// one revolution with the observed pair at its logged position, then a
		// clean revolution so the real gap gets checked
		fire60_2RevolutionWithDistortedTeethAt(eth, steadySlotMs, pair.position, pair.gap1, pair.gap0, /*gapRatio*/3.0f);
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

	uint32_t counterBefore = engine->rpmCalculator.getRevolutionCounterSinceStart();

	for (int i = 0; i < 200; i++) {
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

	// Every revolution in the sweep was clean, so the revolution counter
	// advanced exactly once per engine cycle (2 revolutions): no false sync
	// may have raced it ahead.
	uint32_t counterAfter = engine->rpmCalculator.getRevolutionCounterSinceStart();
	EXPECT_EQ(200, counterAfter - counterBefore)
		<< "revolution counter must track real engine cycles through the sweep";
}

/**
 * The m74_9 first-combustion catch failure: the L9779 VR conditioner swallows
 * 1-2 teeth mid-revolution, the real missing-teeth gap arrives at count 56
 * (C9003 'expected 58/0 got 56/0'), the decoder desyncs and cuts fuel/spark
 * exactly when the engine first catches - it dies right after the catch.
 * With the cranking-band early-gap acceptance (m74_9 opt-in) the gap is
 * accepted as a valid sync: the ratio windows, the position gate and the
 * count-deficit direction (lost events, not noise) all prove it is the real
 * gap. The decoder stays synchronized through the catch.
 */
TEST(trigger, crankingTransition60_2EarlyGapAcceptedWhileCranking) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	SyncEarlyGapWhileCrankingScope earlyGap;
	SyncGapHardeningScope hardening;
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// rev1 + rev2: rev2's first rise is the first sync (counter 0)
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	// rev3 = the revolution after the first sync, with 2 events lost: the
	// next call's first rise (the real gap) arrives at count 56
	fire60_2RevolutionWithLostEvents(eth, steadySlotMs, /*missing*/2, /*gapRatio*/3.0f);

	// rev4's first rise checks rev3's gap at count 56
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no warnings while cranking steadily";

	// accepted as the real gap: no C9003, the sync survives the catch
	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no C9003 at the catch with 2 lost events";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "still synchronized through the catch";
	ASSERT_EQ(2, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter through the catch";

	// one more steady revolution: everything continues normally
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(0u, getRecentWarnings()->getCount());
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());
	ASSERT_EQ(3, engine->triggerCentral.triggerState.getSynchronizationCounter()) << "sync counter after the catch";
}

/**
 * Without the m74_9 opt-in the classic behavior is preserved: the early gap
 * (count 56) desyncs the decoder with C9003 exactly like the on-car log.
 */
TEST(trigger, crankingTransition60_2EarlyGapDesyncsWithoutOverride) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// rev1 + rev2: rev2's first rise is the first sync (counter 0)
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	// rev3 = the revolution after the first sync, with 2 events lost
	fire60_2RevolutionWithLostEvents(eth, steadySlotMs, /*missing*/2, /*gapRatio*/3.0f);

	// rev4's first rise checks rev3's gap at count 56
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	// classic C9003: warning + desync
	ASSERT_EQ(1u, getRecentWarnings()->getCount()) << "one C9003 without the opt-in";
	EXPECT_EQ(ObdCode::CUSTOM_PRIMARY_NOT_ENOUGH_TEETH, getRecentWarnings()->get(0).Code);
	ASSERT_FALSE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "desynced without the opt-in";

	// the next steady revolution re-synchronizes cleanly
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "re-synchronized";
}

/**
 * Once running (rpm above 2 * crankingRpm) the early-gap acceptance is off:
 * a count-56 gap is rejected like before (a tooth deficit at speed means
 * something else is wrong, and the phase cost of accepting it is not
 * harmless anymore).
 */
TEST(trigger, crankingTransition60_2EarlyGapRejectedWhenRunning) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	SyncEarlyGapWhileCrankingScope earlyGap;
	SyncGapHardeningScope hardening;
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// 1250 rpm steady revolutions: above the 2 * crankingRpm acceptance band
	static constexpr float runningSlotMs = 0.8f;
	fire60_2Revolution(eth, runningSlotMs, 3.0f);
	fire60_2Revolution(eth, runningSlotMs, 3.0f);

	// 2 events lost at running speed: the early gap desyncs with C9003
	fire60_2RevolutionWithLostEvents(eth, runningSlotMs, /*missing*/2, /*gapRatio*/3.0f);
	fire60_2Revolution(eth, runningSlotMs, 3.0f);

	ASSERT_GT(Sensor::getOrZero(SensorType::Rpm), 2 * engineConfiguration->cranking.rpm) << "running rpm above the acceptance band";

	// At running rpm the merged tooth also trips the bad-tooth-timing check,
	// so expect the C9003 among the warnings rather than an exact count.
	bool hasNotEnoughTeeth = false;
	for (size_t i = 0; i < getRecentWarnings()->getCount(); i++) {
		if (getRecentWarnings()->get(i).Code == ObdCode::CUSTOM_PRIMARY_NOT_ENOUGH_TEETH) {
			hasNotEnoughTeeth = true;
		}
	}
	ASSERT_TRUE(hasNotEnoughTeeth) << "C9003 at running rpm";
	ASSERT_FALSE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "desynced at running rpm";

	// the next steady revolution re-synchronizes cleanly
	fire60_2Revolution(eth, runningSlotMs, 3.0f);

	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "re-synchronized";
}

/**
 * A deficit of 3+ events is outside the acceptance range: the first early gap
 * is skipped by the position gate (55 + 2 < 58), the index overruns the shape
 * (116) at count 59 and the decoder desyncs with C9002 'too many teeth'.
 * The acceptance must not mask a trigger input that is losing whole chunks
 * of teeth.
 */
TEST(trigger, crankingTransition60_2ThreeMissingEventsStillDesync) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	SyncEarlyGapWhileCrankingScope earlyGap;
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// rev1 + rev2: rev2's first rise is the first sync (counter 0)
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	// rev3 = the revolution after the first sync, with 3 events lost
	fire60_2RevolutionWithLostEvents(eth, steadySlotMs, /*missing*/3, /*gapRatio*/3.0f);

	// rev4's first rise: count 55 -> position gate skips the candidate; the
	// index then overruns at count 59 -> C9002 + desync
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(1u, getRecentWarnings()->getCount()) << "one C9002 for a 3-event deficit";
	EXPECT_EQ(ObdCode::CUSTOM_PRIMARY_TOO_MANY_TEETH, getRecentWarnings()->get(0).Code);
	ASSERT_FALSE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "desynced";

	// rev5's first rise re-synchronizes cleanly
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "re-synchronized";
}

/**
 * The desync -> re-sync storm the m74_9 car sees during cranking chaos:
 * a missed tooth desyncs the decoder (C9003), a false gap pair re-syncs it
 * mid-revolution (the position gate is bypassed while unsynchronized), and
 * the real gap then fires another count mismatch. None of those syncs is a
 * validated crank revolution, so the revolution counter must not advance at
 * all during the storm - it raced ~2x ahead of real time before the
 * clean-sync guard, fast-forwarding ASE and the cranking fuel table.
 */
TEST(trigger, crankingTransition60_2DesyncResyncStormDoesNotRaceRevolutionCounter) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	SyncByPositionWhileCrankingScope syncSkip;
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// the car's cam setup: single-tooth cam with the phase drift cross-check
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;
	engineConfiguration->engineSyncCam = 0;
	engineConfiguration->vvtOffsets[0] = 0;
	custom_board_vvtDriftLimit = []() { return 15.0f; };
	// the override global persists across tests in this binary - clean up
	struct ResetOverride {
		~ResetOverride() { custom_board_vvtDriftLimit = std::nullopt; }
	} resetOverride;

	// steady revolutions to synchronize, then establish the cam phase reference
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "crank synced";
	hwHandleVvtCamSignal(true, getTimeNowNt(), 0);

	uint32_t counterBefore = engine->rpmCalculator.getRevolutionCounterSinceStart();

	// one storm episode: missed tooth -> C9003 desync -> false pair re-sync ->
	// real gap with a count mismatch -> C9003 desync again. Four revolutions
	// are driven in total - at most two honest engine cycles.
	fire60_2RevolutionMissingTooth(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	ASSERT_FALSE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "desynced by the missed tooth";

	fire60_2RevolutionWithDistortedTeethAt(eth, steadySlotMs, /*pos*/24, /*prevRatio*/1.284f, /*distRatio*/2.476f, /*gapRatio*/3.0f);
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "false pair re-synced mid-rev";

	// the cam arrives: either the drift check forces a desync or the phase
	// reference is re-established - both leave the counter untouched
	hwHandleVvtCamSignal(true, getTimeNowNt(), 0);

	// one clean revolution: the real gap either fires a count mismatch
	// (C9003 desync) or re-syncs the decoder
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	// The storm's false syncs must not race the revolution counter ahead of
	// the real revolutions driven (4 revolutions = 2 engine cycles max).
	EXPECT_LE(engine->rpmCalculator.getRevolutionCounterSinceStart() - counterBefore, 2)
		<< "false syncs raced the revolution counter";

	// Recovery: clean revolutions count again, once per engine cycle
	uint32_t counterAtRecovery = engine->rpmCalculator.getRevolutionCounterSinceStart();
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	EXPECT_GT(engine->rpmCalculator.getRevolutionCounterSinceStart(), counterAtRecovery)
		<< "counter must resume on validated revolutions";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "re-synced after the storm";
}

/**
 * The m74_9 trigger logs show noise edge bursts <50 us apart (VR comparator
 * ringing / starter interference). Those edges inflate the decoder's event
 * count, open the position gate early and false-sync it mid-revolution. The
 * board opt-in input debounce (custom_board_triggerDebounceUs = 100 us)
 * must drop any edge closer than the threshold to the previous accepted
 * edge - the real tooth period on cranking is 3.4 ms, so real teeth are
 * untouched.
 */
TEST(trigger, crankingTransition60_2DebounceDropsNoiseEdgeBursts) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	custom_board_triggerDebounceUs = []() { return 100.0f; };
	struct ResetOverride {
		~ResetOverride() { custom_board_triggerDebounceUs = std::nullopt; }
	} resetOverride;

	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// two clean revolutions to synchronize
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "synced on clean revolutions";

	uint32_t hwCountBefore = engine->triggerCentral.getHwEventCounter(SHAFT_PRIMARY_RISING);

	// ten noise pairs: the first edge of each pair arrives 1 ms after the
	// previous accepted edge (accepted), its double 30 us later (dropped).
	for (int i = 0; i < 10; i++) {
		eth.moveTimeForwardUs(1000);
		eth.firePrimaryTriggerRise();
		eth.moveTimeForwardUs(30);
		eth.firePrimaryTriggerRise();
	}

	EXPECT_EQ(hwCountBefore + 10, engine->triggerCentral.getHwEventCounter(SHAFT_PRIMARY_RISING))
		<< "sub-threshold edges must be debounced";

	// the injected edges disturb the decoder (they look like fast teeth), but
	// clean revolutions must re-sync it fully
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "re-synced after the noise burst";
}

/**
 * Replay of the REAL per-tooth periods learned on the m74_9 car (toothdump
 * 2026-08-19 21:54:50, new firmware, gap correctly at tooth 57). The car's
 * tooth times carry the real compression ripple - slot 0 is 1.29x the mean,
 * slot 44 1.35x, the gap 3.08x. scale=1 replays at the native ~84 rpm the
 * profile was learned at; the tests scale it to ~300 rpm cranking speed.
 */
static constexpr float carProfileUs[58] = {
	15337, 12503, 12058, 11942, 11953, 12167, 12486, 13082, 14588, 11375, 11887, 12443,
	12314, 11869, 12450, 13180, 12852, 13047, 13517, 13350, 12840, 12225, 11605, 11081,
	10669, 10402, 10201, 10062, 10010, 9846, 9870, 9840, 9891, 9945, 10017, 10139,
	10307, 10479, 10756, 11108, 11571, 12211, 13087, 13651, 16077, 13899, 14008, 13296,
	12907, 12898, 12613, 12164, 11682, 11293, 11030, 10896, 10728, 36760, // 57 = the missing-teeth gap
};

// native profile speed: 57 * 11890 + 36760 = 714.5 ms per revolution = ~84 rpm
static constexpr float carProfileNativeRevMs = 714.5f;

/**
 * Fire one revolution with the real car tooth periods (scaled). The gap of
 * THIS revolution is checked at the next call's first rise, exactly like the
 * uniform helper above. One revolution = 58 real teeth (0..57): the
 * rise-to-rise of tooth i is carProfileUs[i] for i < 57, and carProfileUs[57]
 * is the missing-teeth gap between tooth 57 and the next revolution's tooth 0.
 *
 * skipTooth:       tooth index whose rise+fall are suppressed (VR threshold
 *                  misses a real tooth at low speed)
 * noiseAfterTooth: insert an extra rise+fall pair noiseDelayUs after this
 *                  tooth's fall - an extra edge that passed the 100 us input
 *                  debounce (300 us is above the threshold; the real m74_9
 *                  noise bursts that survive the debounce look like this)
 */
static void fire60_2RealCarRevolution(EngineTestHelper& eth, float scale,
		int skipTooth = -1, int noiseAfterTooth = -1, float noiseDelayUs = 300.0f,
		int camAfterTooth = -1, bool camEdge = true) {
	for (int tooth = 0; tooth < 58; tooth++) {
		// rise-to-rise from this tooth to the next one; for tooth 57 that is
		// the missing-teeth gap (its fall sits 1/3 into it: 6 deg of tooth,
		// then 12 deg of missing teeth)
		float riseToRiseUs = carProfileUs[tooth < 57 ? tooth : 57] * scale;
		float fallAtUs = tooth < 57 ? riseToRiseUs / 2 : riseToRiseUs / 3;

		if (tooth == skipTooth) {
			// the tooth is invisible to the decoder, only its time passes
			eth.moveTimeForwardUs(MS2US(riseToRiseUs / 1000.0f));
			continue;
		}

		eth.firePrimaryTriggerRise();
		eth.moveTimeForwardUs(MS2US(fallAtUs / 1000.0f));
		eth.firePrimaryTriggerFall();
		eth.moveTimeForwardUs(MS2US((riseToRiseUs - fallAtUs) / 1000.0f));

		if (tooth == camAfterTooth) {
			// the half-moon cam edge lands at this crank position (VVT_SINGLE_TOOTH
			// only uses the RISE edge; the fall is logged and ignored)
			hwHandleVvtCamSignal(camEdge, getTimeNowNt(), 0);
		}

		if (tooth == noiseAfterTooth) {
			// extra edge pair noiseDelayUs after this tooth's fall
			eth.moveTimeForwardUs(MS2US(noiseDelayUs / 1000.0f));
			eth.firePrimaryTriggerRise();
			eth.moveTimeForwardUs(MS2US(noiseDelayUs / 1000.0f));
			eth.firePrimaryTriggerFall();
		}
	}

	// the last event was tooth 57's fall; the gap wait above already advanced
	// the full carProfileUs[57] past tooth 57's rise, so the next call's first
	// rise (tooth 0 of the next revolution) fires at the right time
}

// scale the profile to ~300 rpm cranking: 200 ms per revolution
static constexpr float carProfileCrankingScale = 200.0f / carProfileNativeRevMs;

TEST(trigger, crankingTransition60_2RealCarProfileCleanReplay) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// four revolutions to synchronize; the first sync consumes the harness's
	// disturbed-cycle flag left by the default-trigger simulation, so the
	// steady state is reached after rev 3 (syncCtr=2, one validated cycle)
	for (int i = 0; i < 4; i++) {
		fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	}
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "synced";
	ASSERT_EQ(2, engine->triggerCentral.triggerState.getSynchronizationCounter());

	uint32_t counterBefore = engine->rpmCalculator.getRevolutionCounterSinceStart();
	size_t warningsBefore = getRecentWarnings()->getCount();

	// eight more revolutions of the real learned profile: exactly one sync
	// point per crank revolution, no warnings, and the revolution counter
	// counts ENGINE CYCLES (720 deg = two crank revolutions) 1:1
	for (int i = 0; i < 8; i++) {
		fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	}

	ASSERT_EQ(warningsBefore, getRecentWarnings()->getCount()) << "real car profile must not desync the decoder";
	EXPECT_EQ(10, engine->triggerCentral.triggerState.getSynchronizationCounter())
		<< "exactly one sync point per crank revolution";
	EXPECT_EQ(counterBefore + 4u, engine->rpmCalculator.getRevolutionCounterSinceStart())
		<< "revolution counter counts engine cycles (one per two crank revolutions)";
	EXPECT_NEAR(300, Sensor::getOrZero(SensorType::Rpm), 30) << "rpm from the real profile";
}

/**
 * Intermittent noise: one extra edge in ONE revolution (above the debounce
 * threshold). The gap after that revolution arrives with count 59 - the
 * decoder must reject it (silent C9003 on the car: silentTriggerError is on),
 * re-sync one revolution later (accepted while unsynchronized, but NOT
 * validated), and validate the next clean revolution again. This is the exact
 * signature the 21:55:46 attempt showed on the car: only 3 of ~8 revolutions
 * counted as validated.
 */
TEST(trigger, crankingTransition60_2RealCarProfileIntermittentNoise) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// clean revolutions to synchronize
	for (int i = 0; i < 4; i++) {
		fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	}
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());
	ASSERT_EQ(2, engine->triggerCentral.triggerState.getSynchronizationCounter());

	uint32_t counterBefore = engine->rpmCalculator.getRevolutionCounterSinceStart();
	size_t warningsBefore = getRecentWarnings()->getCount();

	// revolution with one extra noise tooth mid-rev. The extra edge shifts the
	// decoder index: at the end of the revolution the index overflows the wheel
	// -> silent C9002 + desync (warns here, silent on the car). The next clean
	// revolution's gap re-syncs the decoder, but that sync is NOT validated
	// (the previous revolution had a count mismatch).
	fire60_2RealCarRevolution(eth, carProfileCrankingScale, /*skipTooth*/-1, /*noiseAfterTooth*/20);
	fire60_2RealCarRevolution(eth, carProfileCrankingScale);

	ASSERT_EQ(warningsBefore + 1, getRecentWarnings()->getCount()) << "noisy revolution desyncs once";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized())
		<< "re-synced by the next gap (unvalidated)";
	EXPECT_EQ(0, engine->triggerCentral.triggerState.getSynchronizationCounter())
		<< "sync counter restarts after the error";
	EXPECT_EQ(counterBefore, engine->rpmCalculator.getRevolutionCounterSinceStart())
		<< "the noisy revolution is not validated";

	// two more clean revolutions: the first re-validates the sync, the full
	// engine cycle after it counts
	fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	EXPECT_EQ(2, engine->triggerCentral.triggerState.getSynchronizationCounter());
	EXPECT_EQ(counterBefore + 1u, engine->rpmCalculator.getRevolutionCounterSinceStart())
		<< "one validated cycle after the noise";
}

/**
 * Persistent noise: one extra edge in EVERY revolution. The gap always
 * arrives with count 59: the decoder oscillates desync -> unvalidated re-sync
 * forever, the sync counter never stays above zero for a validated
 * revolution, and the validated-sync gate (custom_board_requireValidatedSync)
 * never releases injection/ignition. Documenting this matters: on the car
 * this looks like a crank that spins with NO fuel/spark at all even though
 * the decoder 'sees' teeth.
 */
TEST(trigger, crankingTransition60_2RealCarProfileNoiseEveryRevolution) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// synchronize on clean revolutions first
	for (int i = 0; i < 4; i++) {
		fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	}
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());
	ASSERT_EQ(2, engine->triggerCentral.triggerState.getSynchronizationCounter());

	uint32_t counterBefore = engine->rpmCalculator.getRevolutionCounterSinceStart();

	// six noisy revolutions: the decoder never validates one
	for (int i = 0; i < 6; i++) {
		fire60_2RealCarRevolution(eth, carProfileCrankingScale, /*skipTooth*/-1, /*noiseAfterTooth*/20);
	}

	EXPECT_EQ(counterBefore, engine->rpmCalculator.getRevolutionCounterSinceStart())
		<< "revolution counter must not advance on unvalidated revolutions";

	// recovery: one revolution to re-sync (unvalidated - its gap was noisy),
	// then a full clean cycle validates the next one
	fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());
	EXPECT_EQ(2, engine->triggerCentral.triggerState.getSynchronizationCounter());
	EXPECT_EQ(counterBefore + 1u, engine->rpmCalculator.getRevolutionCounterSinceStart())
		<< "one validated cycle after the noise stops";
}

/**
 * A real tooth missed by the VR comparator (low amplitude at slow crank) in
 * one revolution: the gap arrives with count 57 - same desync -> unvalidated
 * re-sync -> validated-clean cycle as the inserted-noise case.
 */
TEST(trigger, crankingTransition60_2RealCarProfileMissedTooth) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	for (int i = 0; i < 4; i++) {
		fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	}
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());
	ASSERT_EQ(2, engine->triggerCentral.triggerState.getSynchronizationCounter());

	uint32_t counterBefore = engine->rpmCalculator.getRevolutionCounterSinceStart();
	size_t warningsBefore = getRecentWarnings()->getCount();

	// tooth 30 goes missing: count 57 at the gap -> desync
	fire60_2RealCarRevolution(eth, carProfileCrankingScale, /*skipTooth*/30);
	fire60_2RealCarRevolution(eth, carProfileCrankingScale);

	ASSERT_EQ(warningsBefore + 1, getRecentWarnings()->getCount()) << "missed tooth desyncs once";
	ASSERT_FALSE(engine->triggerCentral.triggerState.getShaftSynchronized());
	EXPECT_EQ(counterBefore, engine->rpmCalculator.getRevolutionCounterSinceStart());

	// recovery: re-sync (validated) then a full clean cycle
	fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());
	EXPECT_EQ(0, engine->triggerCentral.triggerState.getSynchronizationCounter());
	EXPECT_EQ(counterBefore + 1u, engine->rpmCalculator.getRevolutionCounterSinceStart());

	fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	EXPECT_EQ(2, engine->triggerCentral.triggerState.getSynchronizationCounter());
	EXPECT_EQ(counterBefore + 2u, engine->rpmCalculator.getRevolutionCounterSinceStart());
}

/**
 * Full m74_9 configuration replay: the real crank profile plus the
 * half-moon cam at a stable phase (the car reads ~213 deg). With a stable
 * cam the sync counter must stay at exactly one increment per crank
 * revolution - the cam contributes nothing, no phase-jump warnings, no
 * phase resyncs. This pins down the decoder behavior behind the car logs:
 * the ~2x sync-counter rate seen on the car is NOT clean-signal behavior,
 * it needs the noise scenarios above.
 */
TEST(trigger, crankingTransition60_2RealCarProfileWithStableCam) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	SyncByPositionWhileCrankingScope syncSkip;
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);
	engineConfiguration->alwaysInstantRpm = true;

	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;
	engineConfiguration->engineSyncCam = 0;
	engineConfiguration->vvtOffsets[0] = 0;
	custom_board_vvtDriftLimit = []() { return 15.0f; };
	struct ResetOverride {
		~ResetOverride() { custom_board_vvtDriftLimit = std::nullopt; }
	} resetOverride;

	// synchronize the crank
	for (int i = 0; i < 4; i++) {
		fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	}
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "crank synced";
	ASSERT_EQ(2, engine->triggerCentral.triggerState.getSynchronizationCounter());

	uint32_t counterBefore = engine->rpmCalculator.getRevolutionCounterSinceStart();
	size_t warningsBefore = getRecentWarnings()->getCount();

	// 8 revolutions; the half-moon cam fires one RISE edge per cam revolution
	// (every 2 crank revolutions) at the same crank tooth (tooth 30) so the
	// phase is stable - the car's cam reads a constant ~213 deg the same way.
	// The cam lands on revolutions whose sync parity is even, so the phase
	// alignment loop never shifts.
	for (int rev = 0; rev < 8; rev++) {
		bool camThisRev = (rev % 2) == 0;
		fire60_2RealCarRevolution(eth, carProfileCrankingScale, /*skipTooth*/-1,
				/*noiseAfterTooth*/-1, /*noiseDelayUs*/300.0f,
				camThisRev ? /*camAfterTooth*/30 : -1, /*camEdge*/true);
	}

	ASSERT_EQ(warningsBefore, getRecentWarnings()->getCount())
		<< "stable cam phase must not warn or desync";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());
	// 7 syncs for 8 revolutions, plus ONE initial cam phase-alignment shift
	// (the first cam event lands on odd sync parity and shifts the cycle basis
	// by 360 deg). After that the stable cam adds nothing per revolution.
	EXPECT_EQ(11, engine->triggerCentral.triggerState.getSynchronizationCounter())
		<< "crank syncs once per revolution; the stable cam adds nothing after the initial alignment";
	EXPECT_EQ(counterBefore + 2u, engine->rpmCalculator.getRevolutionCounterSinceStart())
		<< "two cycles validated; the cam-alignment cycle is skipped by design";
	EXPECT_TRUE(engine->triggerCentral.triggerState.hasSynchronizedPhase()) << "cam phase established";
}

/**
 * Dense noise storm: the m74_9 car logs show sync-counter bursts (~7 syncs in
 * ~10 ms at 270 rpm - 9 syncs/s while the crank makes 4.5 rev/s). A storm of
 * edges 250 us apart (above the 100 us input debounce) inflates the event
 * count, so a storm edge can become the 58th event since the previous sync.
 * If that edge also lands on a ratio inside the windows (chaotic spacings
 * occasionally do), the decoder false-syncs "cleanly": count matches,
 * no error, no desync - just a silent +1 to the sync counter and a shifted
 * phase basis. On the car this races the sync counter ~2-3x and fires
 * ignition/injection from a wrong phase.
 *
 * The fix: a real gap can only arrive roughly one full revolution after the
 * previous sync point. Storm edges arrive at a fraction of a revolution.
 * The sync candidates must now pass a minimum-elapsed-time gate
 * (lastFullRevolutionDurationNt / 4) - the storm sync at ~15% of a
 * revolution is rejected, the revolution then ends in a visible count
 * mismatch at the real gap (proper error + desync + recovery), and the
 * sync counter no longer races.
 *
 * This test FAILS without the time gate (the storm edge false-syncs and the
 * sync counter advances before the real gap) and PASSES with it.
 */
static void fire60_2RealCarStormRevolution(EngineTestHelper& eth, float scale) {
	// tooth 0: the previous revolution's gap wait already positioned us here,
	// this rise is the clean sync event for that gap
	eth.firePrimaryTriggerRise();

	// four more real teeth (counts 1..4)
	for (int tooth = 1; tooth < 5; tooth++) {
		eth.moveTimeForwardUs(MS2US(carProfileUs[tooth] * scale / 1000.0f));
		eth.firePrimaryTriggerRise();
	}

	// 53 storm edges at 250 us (counts 5..57), then a stretched 625 us pair
	// and the 58th event: ratio 625/250 = 2.5 inside [1.6, 3.75], the second
	// gap 250/250 = 1.0 inside [0.85, 1.15] - a perfect false sync candidate
	// (the revolution's first tooth is the sync event itself and is not
	// counted into the new cycle, hence 53 and not 52)
	for (int i = 0; i < 53; i++) {
		eth.moveTimeForwardUs(250);
		eth.firePrimaryTriggerRise();
	}
	eth.moveTimeForwardUs(625);
	eth.firePrimaryTriggerRise(); // the 58th event: the false sync candidate

	// the rest of the real teeth (5..56)
	for (int tooth = 5; tooth < 57; tooth++) {
		eth.moveTimeForwardUs(MS2US(carProfileUs[tooth] * scale / 1000.0f));
		eth.firePrimaryTriggerRise();
	}

	// the gap before the next revolution
	eth.moveTimeForwardUs(MS2US(carProfileUs[57] * scale / 1000.0f));
}

TEST(trigger, crankingTransition60_2RealCarProfileNoiseStormDoesNotFalseSync) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	SyncGapHardeningScope hardening;
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// synchronize on clean revolutions
	for (int i = 0; i < 4; i++) {
		fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	}
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());
	ASSERT_EQ(2, engine->triggerCentral.triggerState.getSynchronizationCounter());

	uint32_t counterBefore = engine->rpmCalculator.getRevolutionCounterSinceStart();
	size_t warningsBefore = getRecentWarnings()->getCount();

	// the storm revolution: its first tooth syncs the previous clean revolution
	// (legitimate +1), but the storm's false-sync candidate must be rejected
	extern bool printTriggerTrace;
	printTriggerTrace = true;
	fire60_2RealCarStormRevolution(eth, carProfileCrankingScale);
	printTriggerTrace = false;
	EXPECT_EQ(3, engine->triggerCentral.triggerState.getSynchronizationCounter())
		<< "only the legitimate gap sync; the storm edge must not false-sync";

	// the storm inflated the event count: the decoder overflows the wheel
	// index mid-revolution -> C9002 (silent on the car) + desync, then the
	// clean revolution's gap re-syncs it (unvalidated)
	fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	ASSERT_EQ(warningsBefore + 1, getRecentWarnings()->getCount()) << "the stormed revolution desyncs once";

	// recovery: re-sync (unvalidated), then a validated cycle
	fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());
	EXPECT_EQ(3, engine->triggerCentral.triggerState.getSynchronizationCounter());
	EXPECT_EQ(counterBefore + 1u, engine->rpmCalculator.getRevolutionCounterSinceStart())
		<< "one validated cycle after the storm";
}

/**
 * Trailing noise edge: a VR ringing edge 250 us after the last real tooth
 * (57) becomes the 58th event with a gap ratio of ~0.07 - far below any
 * plausible compressed gap. The sync-by-position override used to accept it
 * (count matches, position matches, ratio check failed - exactly the
 * override's trigger condition), silently shifting the phase basis by one
 * tooth. The override now requires the gap ratio to be at least 1.2 (the
 * missing-teeth gap is physically 3 tooth slots and cannot compress below
 * ~1.2 even at the first-combustion catch).
 */
static void fire60_2RealCarRevolutionWithTrailingNoiseEdge(EngineTestHelper& eth, float scale) {
	for (int tooth = 0; tooth < 57; tooth++) {
		float riseToRiseUs = carProfileUs[tooth] * scale;
		eth.firePrimaryTriggerRise();
		eth.moveTimeForwardUs(MS2US(riseToRiseUs / 1000.0f / 2));
		eth.firePrimaryTriggerFall();
		eth.moveTimeForwardUs(MS2US(riseToRiseUs / 1000.0f / 2));
	}

	// tooth 57: a ringing edge 250 us after its RISE (the 58th event: count
	// matches, ratio ~0.08 - the gap-tooth time from the previous rise)
	float gapUs = carProfileUs[57] * scale;
	eth.firePrimaryTriggerRise();
	eth.moveTimeForwardUs(250);
	eth.firePrimaryTriggerRise();

	// finish the tooth fall and the gap: the next revolution's first rise
	// lands gapUs after tooth 57's rise
	eth.moveTimeForwardUs(MS2US((gapUs - 250.0f) / 1000.0f));
}

TEST(trigger, crankingTransition60_2RealCarProfileTrailingNoiseEdgeNotAccepted) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	SyncByPositionWhileCrankingScope syncSkip;
	SyncGapHardeningScope hardening;
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	for (int i = 0; i < 4; i++) {
		fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	}
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());
	ASSERT_EQ(2, engine->triggerCentral.triggerState.getSynchronizationCounter());

	uint32_t counterBefore = engine->rpmCalculator.getRevolutionCounterSinceStart();
	size_t warningsBefore = getRecentWarnings()->getCount();

	// the trailing noise edge must not be accepted as the sync point: the
	// ratio floor rejects it and its index overflows the wheel -> one proper
	// C9002 + desync at the noise edge itself. The next revolution's gap has
	// an absurd ratio (gap/250us) and cannot re-sync; the one after that
	// re-syncs (unvalidated - the count was inflated), and only the cycle
	// after that is validated again.
	fire60_2RealCarRevolutionWithTrailingNoiseEdge(eth, carProfileCrankingScale);
	ASSERT_EQ(warningsBefore + 1, getRecentWarnings()->getCount()) << "the noise edge desyncs once";
	ASSERT_FALSE(engine->triggerCentral.triggerState.getShaftSynchronized())
		<< "desynced by the wheel index overflow at the noise edge";

	fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	ASSERT_FALSE(engine->triggerCentral.triggerState.getShaftSynchronized())
		<< "the absurd gap ratio cannot re-sync the decoder";

	// recovery: the next revolution's gap re-syncs (unvalidated), then a
	// full clean cycle validates again
	fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());
	EXPECT_EQ(0, engine->triggerCentral.triggerState.getSynchronizationCounter());
	EXPECT_EQ(counterBefore, engine->rpmCalculator.getRevolutionCounterSinceStart());

	fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	fire60_2RealCarRevolution(eth, carProfileCrankingScale);
	EXPECT_EQ(2, engine->triggerCentral.triggerState.getSynchronizationCounter());
	EXPECT_EQ(counterBefore + 1u, engine->rpmCalculator.getRevolutionCounterSinceStart());
}

/**
 * The first-combustion catch compresses the REAL gap below 1.2 (the m74_9
 * 00:05 log showed exactly this: the gap was rejected, the decoder counted a
 * full revolution and fired C9002 "expected 58 got 58" right as the engine
 * caught - and died). The sync-by-position override floor must sit BELOW any
 * physical gap compression: the gap spans 3 tooth slots and cannot compress
 * below ~1.0 even with absurd acceleration, so a 0.8 floor accepts the catch
 * (ratio 1.0 here) while the trailing-noise case (0.07) stays rejected.
 */
TEST(trigger, crankingTransition60_2CatchGapBelow1_2AcceptedWithHardening) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	SyncByPositionWhileCrankingScope syncSkip;
	SyncGapHardeningScope hardening;
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// steady revolutions to synchronize
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no warnings while cranking steadily";
	ASSERT_EQ(1, engine->triggerCentral.triggerState.getSynchronizationCounter());

	// the catch: the gap compresses to 1.0x (below the ratio window and below
	// the old 1.2 floor) - the override must accept it
	fire60_2Revolution(eth, steadySlotMs, /*gapRatio*/1.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "no C9002 at the catch";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "sync survives the catch";
	ASSERT_EQ(3, engine->triggerCentral.triggerState.getSynchronizationCounter());
}
