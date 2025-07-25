#include "pch.h"

TEST(TripOdometer, TestLargePulses) {
	TripOdometer dut;

	// Initial consumption should be zero
	EXPECT_EQ(0u, dut.getConsumedGrams());

	dut.consumeFuel(100, 0);
	EXPECT_EQ(100u, dut.getConsumedGrams());

	dut.consumeFuel(100, 0);
	EXPECT_EQ(200u, dut.getConsumedGrams());
}

TEST(TripOdometer, TestSmallPulses) {
	TripOdometer dut;

	// Initial consumption should be zero
	EXPECT_EQ(0u, dut.getConsumedGrams());

	dut.consumeFuel(0.6, 0);
	EXPECT_EQ(0u, dut.getConsumedGrams());

	dut.consumeFuel(0.6, 0);
	EXPECT_EQ(1u, dut.getConsumedGrams());

	for (int i = 0; i < 6; i++) {
		dut.consumeFuel(0.6, 0);
	}

	EXPECT_EQ(4u, dut.getConsumedGrams());
	dut.consumeFuel(0.6, 0);
	EXPECT_EQ(5u, dut.getConsumedGrams());
}
