#include "pch.h"
#include "misfire_detection.h"
#include "idle_thread.h"

// Drives synthetic primary-trigger teeth into MisfireController::onEnginePhase, optionally
// inflating the expansion-stroke segment of one cylinder to simulate a misfire.

static constexpr float kCycle      = 720.0f; // four-stroke engine cycle, degrees
static constexpr float kToothDeg   = 6.0f;   // fine synthetic tooth spacing
static constexpr float kUsPerDeg   = 100.0f; // healthy idle pace (~1666 rpm equiv, value irrelevant)

static MisfireController& getMc() {
	return engine->module<MisfireController>().unmock();
}

static void setupMisfireConfig(EngineTestHelper& eth) {
	// The tests feed synthetic kToothDeg teeth straight into onEnginePhase, but the misfire
	// guard reads the *configured* trigger to reject sub-tooth windows. Give it a fine wheel so
	// the configured tooth pitch matches the synthetic feed (otherwise the coarse default
	// TEST_ENGINE trigger makes the guard refuse every realistic window).
	engineConfiguration->trigger.customTotalToothCount = 60;
	engineConfiguration->trigger.customSkippedToothCount = 2;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL);

	engineConfiguration->misfireDetectionEnabled = true;
	engineConfiguration->misfireK                = 3.0f; // threshold = baseline + 3 * wobble
	engineConfiguration->misfireSettleCycles     = 0;    // no settle delay in tests
	engineConfiguration->misfireConsecutiveCount = 2;    // need >=2 flagged firings within the window
	engineConfiguration->misfireWindowFirings    = 16;   // sliding window across all cylinders (~4 cycles)
	engineConfiguration->misfireCountThreshold   = 4;    // small so the test latches quickly
	engineConfiguration->misfireWindowStart      = 20;
	engineConfiguration->misfireWindowEnd        = 120;
}

static float normAngle(float a) {
	a = fmodf(a, kCycle);
	if (a < 0) {
		a += kCycle;
	}
	return a;
}

// Is `phase` inside the forward arc [start, end) (mod cycle)?
static bool phaseInArc(float start, float end, float phase) {
	start = normAngle(start);
	end = normAngle(end);
	phase = normAngle(phase);
	if (start <= end) {
		return phase >= start && phase < end;
	}
	return phase >= start || phase < end;
}

namespace {
struct ToothDriver {
	efitick_t now = 0;

	// Feed one full engine cycle of teeth. If targetCyl >= 0, multiply the per-tooth
	// duration by `factor` for teeth inside that cylinder's detection window.
	void feedCycle(int targetCyl, float factor) {
		float winStart = 0, winEnd = 0;
		if (targetCyl >= 0) {
			angle_t tdc = engine->cylinders[targetCyl].getAngleOffset();
			winStart = tdc + engineConfiguration->misfireWindowStart;
			winEnd = tdc + engineConfiguration->misfireWindowEnd;
		}

		for (float phase = 0; phase < kCycle; phase += kToothDeg) {
			float nextPhase = phase + kToothDeg;
			float dtUs = kUsPerDeg * kToothDeg;
			if (targetCyl >= 0 && phaseInArc(winStart, winEnd, phase + kToothDeg * 0.5f)) {
				dtUs *= factor;
			}
			now += US2NT((efitick_t)dtUs);
			getMc().onEnginePhase(1500.0f, now, phase, nextPhase);
		}
	}

	void feedHealthyCycle() { feedCycle(-1, 1.0f); }
};
} // namespace

// ---- Disabled by default ----

TEST(MisfireDetection, disabledByDefault) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	ASSERT_FALSE(engineConfiguration->misfireDetectionEnabled);

	// Even forcing idle, nothing should be monitored or counted while disabled.
	engine->module<IdleController>().unmock().isIdling = true;

	ToothDriver d;
	for (int i = 0; i < 6; i++) {
		d.feedCycle(0, 2.0f); // big slowdowns, but feature is off
	}

	EXPECT_FALSE(getMc().misfireDetectionActive);
	EXPECT_EQ(0, getMc().misfireTotalCount);
	EXPECT_FALSE(getMc().misfireLatched);
}

// ---- Healthy idle produces no misfires ----

TEST(MisfireDetection, healthyIdleNoMisfire) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupMisfireConfig(eth);
	engine->module<IdleController>().unmock().isIdling = true;

	ToothDriver d;
	for (int i = 0; i < 20; i++) {
		d.feedHealthyCycle();
	}

	EXPECT_TRUE(getMc().misfireDetectionActive);
	EXPECT_EQ(0, getMc().misfireTotalCount);
	EXPECT_FALSE(getMc().misfireLatched);
}

// ---- Repeated misfires on one cylinder latch the MIL ----

TEST(MisfireDetection, repeatedMisfireLatchesMil) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupMisfireConfig(eth);
	engine->module<IdleController>().unmock().isIdling = true;

	ToothDriver d;
	// Seed the shared engine-wide baseline with healthy cycles first.
	for (int i = 0; i < 5; i++) {
		d.feedHealthyCycle();
	}
	ASSERT_EQ(0, getMc().misfireTotalCount);

	// Now misfire cylinder 0 every cycle (1.6x slower segment >> 1.15 ratio). Even though the
	// other cylinders fire cleanly in between, a single dead cylinder puts >=2 flagged firings
	// in the 16-firing window, so the engine-wide rate test counts it and the MIL latches.
	for (int i = 0; i < 12; i++) {
		d.feedCycle(0, 1.6f);
	}

	EXPECT_GE(getMc().misfireTotalCount, engineConfiguration->misfireCountThreshold);
	EXPECT_TRUE(getMc().misfireLatched);
}

// ---- threshold == 0: monitor-only, never latches ----

TEST(MisfireDetection, thresholdZeroMonitorOnly) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupMisfireConfig(eth);
	engineConfiguration->misfireCountThreshold = 0; // monitor-only
	engine->module<IdleController>().unmock().isIdling = true;

	ToothDriver d;
	for (int i = 0; i < 5; i++) {
		d.feedHealthyCycle();
	}
	for (int i = 0; i < 20; i++) {
		d.feedCycle(0, 1.6f); // misfire cylinder 0 every cycle
	}

	EXPECT_GT(getMc().misfireTotalCount, 0u); // counter accumulated
	EXPECT_FALSE(getMc().misfireLatched);     // never latched
}

// ---- Leaving idle stops monitoring ----

TEST(MisfireDetection, leavingIdleStopsMonitoring) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupMisfireConfig(eth);
	auto& idleCtrl = engine->module<IdleController>().unmock();
	idleCtrl.isIdling = true;

	ToothDriver d;
	d.feedHealthyCycle();
	EXPECT_TRUE(getMc().misfireDetectionActive);

	// Drop out of idle — detector must go inactive and not count.
	idleCtrl.isIdling = false;
	for (int i = 0; i < 6; i++) {
		d.feedCycle(0, 2.0f);
	}

	EXPECT_FALSE(getMc().misfireDetectionActive);
	EXPECT_EQ(0, getMc().misfireTotalCount);
}

// ---- Window narrower than one trigger tooth: refuse + configError, never freeze ----

TEST(MisfireDetection, narrowWindowBelowToothSpacingRefuses) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupMisfireConfig(eth);
	engine->module<IdleController>().unmock().isIdling = true;

	// A sub-tooth window: both ends fall inside one trigger interval, so the timed segment
	// would collapse to zero and every live-data field would silently freeze. The guard must
	// refuse to run and raise a configError instead of measuring degenerate segments.
	clearConfigErrorMessage();
	engineConfiguration->misfireWindowStart = 20;
	engineConfiguration->misfireWindowEnd   = 21; // 1 deg wide, far below any real tooth pitch

	ToothDriver d;
	for (int i = 0; i < 6; i++) {
		d.feedCycle(0, 1.6f); // even with big slowdowns, nothing should be measured
	}

	EXPECT_FALSE(getMc().misfireDetectionActive);
	EXPECT_EQ(0, getMc().misfireTotalCount);
	EXPECT_FALSE(getMc().misfireLatched);
	EXPECT_TRUE(hasConfigError());

	// Widening back to a valid window must clear the refusal and resume detection — and the
	// guard must not keep re-raising the configError once the window is large enough.
	clearConfigErrorMessage();
	engineConfiguration->misfireWindowStart = 20;
	engineConfiguration->misfireWindowEnd   = 120;
	for (int i = 0; i < 3; i++) {
		d.feedHealthyCycle();
	}

	EXPECT_TRUE(getMc().misfireDetectionActive);
	EXPECT_FALSE(hasConfigError());
}
