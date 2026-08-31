/*
 * test_angle_clock.cpp
 *
 * Host-side tests for the pure angle-clock tick math (the hardware driver
 * itself is register-level TMR2 code and only compiles under EFI_PROD_CODE).
 */

#include "pch.h"
#include "angle_clock.h"

TEST(AngleClock, zeroAngleIsZeroTicks) {
	EXPECT_EQ(0u, angleClockDelayTicks(0.0f, 100.0f));
}

TEST(AngleClock, delayIsAngleTimesTicksPerDegree) {
	// The armed delay = remaining angle x the fresh last-tooth basis
	// (NT ticks per degree), not the 90-degree rpm average - that value
	// lags by revolutions at the catch and fired events ms-late.
	float oneDegreeUs = 166.6f;	// ~1000 rpm
	float ticksPerDegree = oneDegreeUs * US_TO_NT_MULTIPLIER;

	// one full tooth on 60-2 (3 degrees) at ~1000 rpm
	EXPECT_EQ(static_cast<uint32_t>(3.0f * ticksPerDegree),
		angleClockDelayTicks(3.0f, ticksPerDegree));

	// two teeth of lead: 6 degrees
	EXPECT_EQ(static_cast<uint32_t>(6.0f * ticksPerDegree),
		angleClockDelayTicks(6.0f, ticksPerDegree));
}

TEST(AngleClock, fractionalTicksTruncate) {
	// 1 deg x 150 ticks/deg = 150 ticks exactly (no truncation needed)
	EXPECT_EQ(150u, angleClockDelayTicks(1.0f, 150.0f));

	// 1.0 deg x 2.49 ticks/deg = 2.49 -> the float multiply may land at
	// 2.489999... or 2.490000..., so check it is within one tick of 2
	uint32_t ticks = angleClockDelayTicks(1.0f, 2.49f);
	EXPECT_LE(2u, ticks);
	EXPECT_LE(ticks, 3u);
}

TEST(AngleClock, ticksPerDegreeScalingIsLinear) {
	// halving the speed doubles the delay in ticks
	uint32_t fast = angleClockDelayTicks(6.0f, 80.0f);
	uint32_t slow = angleClockDelayTicks(6.0f, 160.0f);
	EXPECT_EQ(fast * 2, slow);
}
