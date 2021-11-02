#include "pch.h"

#include "knock_logic.h"

TEST(Knock, Retards) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	// Knock threshold of 20dBv
	ENGINE(engineState).knockThreshold = 20;
	// Aggression of 10%
	CONFIG(knockRetardAggression) = 100;
	// Maximum 8 degrees retarded
	CONFIG(knockRetardMaximum) = 8;

	KnockController dut;
	INJECT_ENGINE_REFERENCE(&dut);

	// No retard unless we knock
	ASSERT_FLOAT_EQ(dut.getKnockRetard(), 0);

	// Send some weak knocks, should yield no response
	for (size_t i = 0; i < 10; i++) {
		dut.onKnockSenseCompleted(0, 10, 0);
	}

	EXPECT_FLOAT_EQ(dut.getKnockRetard(), 0);

	// Send a strong knock!
	dut.onKnockSenseCompleted(0, 30, 0);

	// Should retard 10% of the distance between current timing and "maximum"
	EXPECT_FLOAT_EQ(dut.getKnockRetard(), 2);

	// Send tons of strong knocks, make sure we don't go over the configured limit
	for (size_t i = 0; i < 100; i++) {
		dut.onKnockSenseCompleted(0, 30, 0);
	}

	EXPECT_FLOAT_EQ(dut.getKnockRetard(), 8);
}

TEST(Knock, Reapply) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	KnockController dut;
	INJECT_ENGINE_REFERENCE(&dut);

	// Knock threshold of 20dBv
	ENGINE(engineState).knockThreshold = 20;
	// Aggression of 10%
	CONFIG(knockRetardAggression) = 100;
	// Maximum 8 degrees retarded
	CONFIG(knockRetardMaximum) = 8;
	// Apply 1 degree/second
	CONFIG(knockRetardReapplyRate) = 10;

	// Send a strong knock!
	dut.onKnockSenseCompleted(0, 30, 0);

	// Should retard 10% of the distance between current timing and "maximum"
	EXPECT_FLOAT_EQ(dut.getKnockRetard(), 2);

	constexpr auto fastPeriodSec = FAST_CALLBACK_PERIOD_MS / 1000.0f;

	// call the fast callback, should reapply 1 degree * callback period
	dut.periodicFastCallback();
	EXPECT_FLOAT_EQ(dut.getKnockRetard(), 2 - 1.0f * fastPeriodSec);

	// 10 updates total
	for (size_t i = 0; i < 9; i++) {
		dut.periodicFastCallback();
	}
	EXPECT_FLOAT_EQ(dut.getKnockRetard(), 2 - 10 * 1.0f * fastPeriodSec);

	// Spend a long time without knock
	for (size_t i = 0; i < 1000; i++) {
		dut.periodicFastCallback();
	}

	// Should have no knock retard
	EXPECT_FLOAT_EQ(dut.getKnockRetard(), 0);
}
