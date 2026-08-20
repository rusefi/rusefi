/*
 * @file test_60_2_early_gap_latch.cpp
 *
 * Reproduces the m74_9 +1 phase-basis latch seen in the 19:53:34 car log:
 * a whole crank ran with the decoder synchronized on an ORDINARY tooth
 * (synctrace showed 9 consecutive 'S' syncs with gap0 ~1.0 instead of the
 * real gap's 2.0-3.0), so every scheduled event fired 6 degrees late - the
 * "wrong ignition angle" symptom.
 *
 * Seed sequence:
 *  - 1 event is lost mid-revolution (L9779 analog edge swallowing) AND the
 *    missing-teeth gap compresses to ~1.0-1.2x at the first-combustion
 *    catch: the gap arrives at count 57 with a ratio below the windows.
 *  - The decoder does not sync there (ratio check fails, raw check fails).
 *  - At count 58 the tooth AFTER the gap arrives with ratio 1/gapRatio
 *    (~0.83-1.0, above the 0.8 sync-by-position floor) and the
 *    cranking-band bypass accepts it as the sync point -> the phase basis
 *    shifts +1 tooth (6 degrees).
 *
 * Heal: at the next revolution the REAL gap arrives at count 57 with a raw
 * ratio ~3.0 and the cranking-band early-gap acceptance re-anchors the
 * decoder at the real gap ('A' sync event) instead of desyncing. With the
 * neutral profile of this test (factor 1.0 everywhere, the weak default)
 * the normalized check already passes and the regular early-gap path heals;
 * on the car the learned profile can distort the normalized check at the
 * shifted slots (the observed latch), and the raw-ratio acceptance
 * (custom_board_syncEarlyGapWhileCranking) covers exactly that case - the
 * raw gap ratio stays ~2-3 regardless of the slot belief. That path is
 * exercised on hardware; here the seed + heal chain is pinned end to end.
 */

#include "pch.h"
#include "board_overrides.h"

#include <vector>

// Strong override of the weak board hook: capture the decoder's sync events.
struct CapturedSyncEvent {
	char kind;
	int countErr;
	float gap0;
};

static std::vector<CapturedSyncEvent> capturedSyncEvents;

void boardTriggerSyncEvent(char kind, int countersError, float gap0, float gap1) {
	UNUSED(gap1);
	capturedSyncEvents.push_back({kind, countersError, gap0});
}

// The m74_9 board opts into all three cranking-band leniency mechanisms;
// reset at test end - the override globals persist across tests in this binary.
struct M74_9CrankingLeniencyScope {
	M74_9CrankingLeniencyScope() {
		custom_board_syncByPositionWhileCranking = []() { return true; };
		custom_board_syncGapHardening = []() { return true; };
		custom_board_syncEarlyGapWhileCranking = []() { return true; };
	}

	~M74_9CrankingLeniencyScope() {
		custom_board_syncByPositionWhileCranking = std::nullopt;
		custom_board_syncGapHardening = std::nullopt;
		custom_board_syncEarlyGapWhileCranking = std::nullopt;
	}
};

/**
 * Fire one full 60-2 revolution: 58 real teeth, then the missing-teeth gap.
 * The gap ratio of THIS call is checked at the NEXT call's first rise.
 */
static void fire60_2Revolution(EngineTestHelper& eth, float slotMs, float gapRatio) {
	for (int i = 0; i < 58; i++) {
		eth.fireRise(slotMs / 2);
		eth.fireFall(slotMs / 2);
	}

	eth.moveTimeForwardUs(MS2US((gapRatio - 1.0f) * slotMs));
}

/**
 * Fire one revolution with one event LOST: the lost tooth merges into the
 * tooth at mergePos (2x slot), the revolution has 57 teeth, and the real
 * gap arrives at count 57 - the seed signature from the car.
 */
static void fire60_2RevolutionWithLostEvent(EngineTestHelper& eth, float slotMs, float gapRatio) {
	const int mergePos = 30;
	for (int i = 0; i < 57; i++) {
		float riseToRise = (i == mergePos) ? slotMs * 2 : slotMs;

		eth.moveTimeForwardUs(MS2US(riseToRise - slotMs / 2));
		eth.firePrimaryTriggerRise();
		eth.moveTimeForwardUs(MS2US(slotMs / 2));
		eth.firePrimaryTriggerFall();
	}

	eth.moveTimeForwardUs(MS2US((gapRatio - 1.0f) * slotMs));
}

// steady 400 RPM: 60 slots * 2.5 ms = 150 ms per revolution
static constexpr float steadySlotMs = 2.5f;

// Count warnings relevant to the trigger path. The synthetic tooth patterns
// (merged teeth) make the scheduler re-add an action into the execution
// queue (9011) - an environmental artifact, not a trigger diagnostic.
static size_t countTriggerWarnings() {
	size_t count = 0;
	for (size_t i = 0; i < getRecentWarnings()->getCount(); i++) {
		if (getRecentWarnings()->get(i).Code == ObdCode::CUSTOM_RE_ADDING_INTO_EXECUTION_QUEUE) {
			continue;
		}
		count++;
	}
	return count;
}

TEST(trigger, crankingTransition60_2LatchSeedHealsAtTheRealGap) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	M74_9CrankingLeniencyScope leniency;
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	capturedSyncEvents.clear();

	// rev1 + rev2: rev2's first rise is the first sync (counter 0)
	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	// Baseline: the scheduler may raise unrelated diagnostics (9011
	// re-add-into-queue) on the synthetic tooth pattern; the trigger path
	// under test must not add any NEW trigger warnings.
	size_t warningsBase = countTriggerWarnings();

	// rev3: 1 event lost AND the gap compressed to 1.2x - the seed
	fire60_2RevolutionWithLostEvent(eth, steadySlotMs, /*gapRatio*/1.2f);

	// rev4: first rise = the compressed gap at count 57 (ratio check and raw
	// check both fail, no sync); second rise = ordinary tooth at count 58 ->
	// the cranking-band bypass accepts it -> phase basis +1 tooth
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(warningsBase, countTriggerWarnings()) << "no new warnings through the latch seed";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "bypass kept the decoder synchronized (on the wrong tooth)";

	// the bypass sync was on an ordinary tooth: captured 'S' with gap0 ~0.83
	ASSERT_FALSE(capturedSyncEvents.empty());
	ASSERT_EQ('S', capturedSyncEvents.back().kind);
	EXPECT_NEAR(1.0f / 1.2f, capturedSyncEvents.back().gap0, 0.05) << "bypass accepted the tooth after the gap";

	// rev5's first rise: the REAL gap arrives at count 57 with raw ratio 3.0
	// and the early-gap acceptance re-anchors the decoder at the real gap:
	// kind 'A', countErr -1.
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(warningsBase, countTriggerWarnings()) << "no C9003 at the heal";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "still synchronized through the heal";

	ASSERT_EQ('A', capturedSyncEvents.back().kind) << "early-gap acceptance healed the latch";
	EXPECT_EQ(-1, capturedSyncEvents.back().countErr);
	EXPECT_NEAR(3.0f, capturedSyncEvents.back().gap0, 0.1) << "heal happened at the real gap";

	// rev6: after the heal the basis is correct again - a clean 'S' at the
	// real gap, and the sync counter advances normally
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(warningsBase, countTriggerWarnings());
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());
	ASSERT_EQ('S', capturedSyncEvents.back().kind) << "clean sync after the heal";
	EXPECT_NEAR(3.0f, capturedSyncEvents.back().gap0, 0.1) << "clean sync at the real gap";
	EXPECT_EQ(0, capturedSyncEvents.back().countErr);
}

/**
 * A clean revolution at the correct basis must not false-fire the raw-ratio
 * acceptance: the ordinary tooth at count 57 has ratio ~1.0, far below the
 * 1.6 window low side, so the real gap at count 58 syncs cleanly.
 */
TEST(trigger, crankingTransition60_2OrdinaryToothNeverRawFires) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	M74_9CrankingLeniencyScope leniency;
	// the user's m74_9 runs the 60-2 wheel on the crank
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	capturedSyncEvents.clear();

	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	// Baseline: the scheduler may raise unrelated diagnostics (9011
	// re-add-into-queue) on the synthetic tooth pattern; the trigger path
	// under test must not add any NEW trigger warnings.
	size_t warningsBase = countTriggerWarnings();

	fire60_2Revolution(eth, steadySlotMs, 3.0f);
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(warningsBase, countTriggerWarnings()) << "no warnings while cranking steadily";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());

	// every captured sync after the first must be a clean 'S' at the real gap
	for (size_t i = 1; i < capturedSyncEvents.size(); i++) {
		ASSERT_EQ('S', capturedSyncEvents[i].kind) << "no early-gap acceptance on clean revolutions (event " << i << ")";
		EXPECT_EQ(0, capturedSyncEvents[i].countErr);
		EXPECT_NEAR(3.0f, capturedSyncEvents[i].gap0, 0.1);
	}
}
