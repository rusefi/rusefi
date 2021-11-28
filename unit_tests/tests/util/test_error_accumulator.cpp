#include "pch.h"

#include "error_accumulator.h"

TEST(errorAccumulator, ignoreSmallError) {
	ErrorAccumulator dut;
	dut.init(5, 0.01);

	for (size_t i = 0; i < 1'000'000; i++) {
		// An error just below the threshold should never trip
		ASSERT_EQ(0, dut.accumulate(4));
	}
}

TEST(errorAccumulator, integrateError) {
	ErrorAccumulator dut;
	dut.init(5, 0.01);

	for (size_t i = 0; i < 100; i++) {
		// error of 1 over the ignore value
		dut.accumulate(6);
	}

	// Integral should be 1 * dt * 100 = 1.0
	ASSERT_NEAR(dut.getAccumulator(), 1, 0.001f);
}

TEST(errorAccumulator, integrateNegativeError) {
	ErrorAccumulator dut;
	dut.init(5, 0.01);

	for (size_t i = 0; i < 100; i++) {
		// error of 1 over the ignore value, but negative
		dut.accumulate(-6);
	}

	// Integral should be 1 * dt * 100 = 1.0
	ASSERT_NEAR(dut.getAccumulator(), 1, 0.001f);
}

TEST(errorAccumulator, integrateErrorBothSigns) {
	ErrorAccumulator dut;
	dut.init(5, 0.01);

	for (size_t i = 0; i < 100; i++) {
		// These should collectively integrate 1 * dt worth of error
		dut.accumulate(-5.5);
		dut.accumulate(5.5);
	}

	// Integral should be 2 * 0.5 * dt * 100 = 1.0
	ASSERT_NEAR(dut.getAccumulator(), 1, 0.001f);
}
