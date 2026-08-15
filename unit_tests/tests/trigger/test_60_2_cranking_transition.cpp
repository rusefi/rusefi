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
 * At tooth 25's rise the decoder sees gap0 = distRatio and gap1 = prevRatio
 * (tooth 24 is prevRatio, tooth 25 is distRatio*prevRatio times the slot).
 * The sync gap at the end of the revolution stays gapRatio.
 */
static void fire60_2RevolutionWithDistortedTeeth(EngineTestHelper& eth, float slotMs, float prevRatio, float distRatio, float gapRatio) {
	for (int i = 0; i < 58; i++) {
		// rise-to-rise duration for this tooth
		float riseToRise = slotMs;
		if (i == 24) riseToRise = prevRatio * slotMs;
		if (i == 25) riseToRise = distRatio * prevRatio * slotMs;

		// the previous fall happened slotMs/2 after the previous rise
		eth.moveTimeForwardUs(MS2US(riseToRise - slotMs / 2));
		eth.firePrimaryTriggerRise();
		eth.moveTimeForwardUs(MS2US(slotMs / 2));
		eth.firePrimaryTriggerFall();
	}

	// trailing wait: the next call's first rise lands gapRatio * slotMs after tooth 57's rise
	eth.moveTimeForwardUs(MS2US((gapRatio - 1.0f) * slotMs));
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

TEST(trigger, crankingTransition60_2DecelerationRecovers) {
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

	// transition revolution: the crank kicks back, the gap stretches to 4.2x
	fire60_2Revolution(eth, steadySlotMs, /*gapRatio*/4.2f);

	// the stretched gap of the transition revolution is checked at this first
	// rise. It is above the intentional 3.75 upper limit, so the sync point is
	// rejected: exactly one full revolution of teeth arrives without a sync
	// point, the decoder reports C9002 and desynchronizes.
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(1u, getRecentWarnings()->getCount()) << "one C9002 for the extreme deceleration";
	EXPECT_EQ(ObdCode::CUSTOM_PRIMARY_TOO_MANY_TEETH, getRecentWarnings()->get(0).Code);

	// the next steady revolution re-synchronizes cleanly
	fire60_2Revolution(eth, steadySlotMs, 3.0f);

	ASSERT_EQ(steadyRpm, round(Sensor::getOrZero(SensorType::Rpm))) << "RPM after recovery";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "re-synchronized";
}
