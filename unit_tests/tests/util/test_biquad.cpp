#include "pch.h"
#include "biquad.h"
#include "unit_test_framework.h"

TEST(util, biQuad) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE); // LOL engineConfiguration->verboseQuad
	Biquad b;
	float filtered = 0;

	// 0.01 means filter bandwidth of ~1hz with ~100hz sensor
	b.configureLowpass(1, 0.01f);

	for (size_t i = 0; i < 10000; i++) {
		filtered = b.filter(25.0);
	}

	// very close, but never actually reaches
	EXPECT_NEAR(filtered, 25.0, 0.1);
}


// The interesting cases are the slow ones: as cutoff/sampling shrinks the poles crowd up against
// z = 1, and the (1 + b1 + b2) that sets the DC gain is computed by catastrophic cancellation.
// fs/1000 is the slowest we allow, and it must still pass DC at unity gain.
static float measureDcGain(float samplingFrequency, float cutoffFrequency, float input = 2.5f, bool startSteady = true) {
	Biquad bq;
	bq.configureLowpass(samplingFrequency, cutoffFrequency);

	if (startSteady) {
		bq.cookSteadyState(input);
	}

	// Long enough to settle even the slowest filter we allow
	float out = 0;
	for (int i = 0; i < 100000; i++) {
		out = bq.filter(input);
	}

	return out / input;
}

TEST(Biquad, LowpassSensorScale) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	for (float input : {1.0f, 2.5f, 25.0f, 100.0f, -10.0f}) {
		for (bool startSteady : {false, true}) {
			SCOPED_TRACE(::testing::Message() << "input=" << input << " startSteady=" << startSteady);
			// Preserve sensor scale both after a step and after priming the filter.
			EXPECT_NEAR(measureDcGain(1000, 1, input, startSteady), 1.0f, 1e-3);
		}
	}
}

TEST(Biquad, LowpassDcGain) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE); // Biquad::filter reads verboseQuad.

	// ADC subscription rates, from the fastest sensor bandwidth down to the slowest we allow
	EXPECT_NEAR(measureDcGain(500, 200), 1.0f, 1e-3);
	EXPECT_NEAR(measureDcGain(500, 10), 1.0f, 1e-3);
	EXPECT_NEAR(measureDcGain(500, 2), 1.0f, 1e-3);
	EXPECT_NEAR(measureDcGain(500, 0.5f), 1.0f, 1e-3);

	// H7 boards run the ADC twice as fast, so every cutoff is half as many samples wide
	EXPECT_NEAR(measureDcGain(1000, 2), 1.0f, 1e-3);
	EXPECT_NEAR(measureDcGain(1000, 1), 1.0f, 1e-3);

	// FrequencySensor filters per-edge, so its sampling frequency is 1
	EXPECT_NEAR(measureDcGain(1, 0.35f), 1.0f, 1e-3);
	EXPECT_NEAR(measureDcGain(1, 0.001f), 1.0f, 1e-3);
}

TEST(Biquad, LowpassSettles) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE); // Biquad::filter reads verboseQuad.

	// The slowest filter we allow is the one most likely to walk off on its own
	Biquad bq;
	bq.configureLowpass(1000, 1);

	// A step from 0 should approach the new value monotonically enough to stay well bounded -
	// Q of 0.54 gives a fraction of a percent of overshoot and nothing more
	for (int i = 0; i < 1000000; i++) {
		float out = bq.filter(1);
		EXPECT_GE(out, 0);
		EXPECT_LE(out, 1.01f);
	}

	EXPECT_NEAR(bq.filter(1), 1.0f, 1e-3);
}

TEST(Biquad, RejectsUnsupportedCutoff) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	Biquad bq;

	for (float cutoff : {0.0f, 0.1f, 401.0f}) {
		SCOPED_TRACE(cutoff);
		EXPECT_FATAL_ERROR(bq.configureLowpass(1000, cutoff));
		EXPECT_FATAL_ERROR(bq.configureHighpass(1000, cutoff));
		EXPECT_FATAL_ERROR(bq.configureBandpass(1000, cutoff, 3));
	}
}

TEST(Biquad, AcceptsCutoffBoundaries) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	Biquad bq;

	for (float cutoff : {1.0f, 400.0f}) {
		SCOPED_TRACE(cutoff);
		EXPECT_NO_FATAL_ERROR(bq.configureLowpass(1000, cutoff));
		EXPECT_NO_FATAL_ERROR(bq.configureHighpass(1000, cutoff));
		EXPECT_NO_FATAL_ERROR(bq.configureBandpass(1000, cutoff, 3));
	}
}
