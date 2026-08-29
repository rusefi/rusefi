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

TEST(AngleClock, delayIsAngleTimesMicrosecondsPerDegree) {
	// delayTicks = angleFromNow[deg] * oneDegreeUs[us/deg] * US_TO_NT_MULTIPLIER
	// (the angle clock runs at the NT tick rate, 4 MHz on the target).
	float oneDegreeUs = 166.6f;

	// one full tooth on 60-2 (3 degrees) at ~1000 rpm
	EXPECT_EQ(static_cast<uint32_t>(3.0f * oneDegreeUs * US_TO_NT_MULTIPLIER),
		angleClockDelayTicks(3.0f, oneDegreeUs));

	// two teeth of lead: 6 degrees
	EXPECT_EQ(static_cast<uint32_t>(6.0f * oneDegreeUs * US_TO_NT_MULTIPLIER),
		angleClockDelayTicks(6.0f, oneDegreeUs));
}

TEST(AngleClock, fractionalTicksTruncate) {
	// 1.5 us * 100 ticks/us = 150 ticks exactly (no truncation needed)
	EXPECT_EQ(150u, angleClockDelayTicks(1.0f, 1.5f));

	// 1.0 deg * 2.49 us/deg * 100 = 249.0 -> the float multiply may land at
	// 248.999... or 249.000..., so check it is within one tick of 249
	uint32_t ticks = angleClockDelayTicks(1.0f, 2.49f);
	EXPECT_LE(248u, ticks);
	EXPECT_LE(ticks, 250u);
}

TEST(AngleClock, oneDegreeUsScalingIsLinear) {
	// halving the speed doubles the delay in ticks
	uint32_t fast = angleClockDelayTicks(6.0f, 80.0f);
	uint32_t slow = angleClockDelayTicks(6.0f, 160.0f);
	EXPECT_EQ(fast * 2, slow);
}
