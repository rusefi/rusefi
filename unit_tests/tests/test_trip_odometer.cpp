#include "pch.h"

TEST(TripOdometer, TestLargePulses) {
	TripOdometer dut;

	// Initial consumption should be zero
	EXPECT_EQ(0, dut.getConsumedGrams());

	dut.consumeFuel(100, 0);
	EXPECT_EQ(100, dut.getConsumedGrams());

	dut.consumeFuel(100, 0);
	EXPECT_EQ(200, dut.getConsumedGrams());
}

TEST(TripOdometer, TestSmallPulses) {
	TripOdometer dut;

	// Initial consumption should be zero
	EXPECT_EQ(0, dut.getConsumedGrams());

	dut.consumeFuel(0.6, 0);
	EXPECT_EQ(0, dut.getConsumedGrams());

	dut.consumeFuel(0.6, 0);
	EXPECT_EQ(1, dut.getConsumedGrams());

	for (int i = 0; i < 6; i++) {
		dut.consumeFuel(0.6, 0);
	}

	EXPECT_EQ(4, dut.getConsumedGrams());
	dut.consumeFuel(0.6, 0);
	EXPECT_EQ(5, dut.getConsumedGrams());
}
