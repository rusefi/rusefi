#include "pch.h"
#include "trigger_decoder.h"

namespace {
// Independent transcription of the supplied 360-degree waveform. Do not use
// the production shape to generate input: incorrect geometry must fail tests.
constexpr int rises[] = {25, 35, 45, 55, 65, 75, 85, 95, 105, 115,
	145, 155, 165, 175, 185, 195, 205, 215, 225, 235, 245, 255,
	265, 275, 285, 295, 305, 315, 325, 335, 345, 355};
constexpr int toothCount = 32;
constexpr int syncTooth = 1;

int gapBefore(int index) {
	return index == 0 ? 30 : rises[index] - rises[index - 1];
}

void feed(EngineTestHelper& eth, TriggerDecoderBase& decoder, efitick_t now) {
	// The input path filters falling edges for RiseOnly before calling the decoder.
	setTimeNowNt(now);
	decoder.decodeTriggerEvent("Suzuki 36-2-2", eth.engine.triggerCentral.triggerShape,
		nullptr, eth.engine.triggerCentral.primaryTriggerConfiguration, SHAFT_PRIMARY_RISING, now);
}

void expectPhase(const TriggerDecoderBase& decoder, int index) {
	ASSERT_TRUE(decoder.shaft_is_synchronized);
	EXPECT_EQ(2 * ((index - syncTooth + toothCount) % toothCount), decoder.getCurrentIndex());
}

void checkStartup(int start, int rpm) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_SUZUKI_36_2_2);
	TriggerDecoderBase decoder("Suzuki startup");
	efitick_t now = 0;
	for (int n = 0; n < 5 * toothCount; n++) {
		const int index = (start + n) % toothCount;
		now += US2NT(gapBefore(index) * 60000000LL / (360 * rpm));
		feed(eth, decoder, now);
		if (decoder.shaft_is_synchronized) {
			expectPhase(decoder, index);
		}
		// Allow enough history for 13 ratios plus one complete revolution.
		if (n >= toothCount + 14) {
			ASSERT_TRUE(decoder.shaft_is_synchronized) << "event " << n;
		}
	}
	EXPECT_EQ(0u, decoder.totalTriggerErrorCounter);
}

void checkPulseFault(bool extraPulse) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_SUZUKI_36_2_2);
	// Direct decoder calls do not update RPM. Overflow checks require running RPM.
	Sensor::setMockValue(SensorType::Rpm, 1666.67f);
	TriggerDecoderBase decoder("Suzuki pulse fault");
	efitick_t now = 0;
	bool lostSync = false;
	for (int n = 0; n < 8 * toothCount; n++) {
		const int index = n % toothCount;
		if (extraPulse && n == 3 * toothCount + 5) {
			feed(eth, decoder, now + US2NT(100));
		}
		now += US2NT(gapBefore(index) * 100);
		if (!extraPulse && n == 3 * toothCount + 5) {
			continue;
		}
		feed(eth, decoder, now);
		if (n >= toothCount + 14 && n < 3 * toothCount + 5) {
			expectPhase(decoder, index);
		}
		if (n >= 3 * toothCount + 5 && !decoder.shaft_is_synchronized) {
			lostSync = true;
		}
		if (n >= 6 * toothCount) {
			expectPhase(decoder, index);
		}
	}
	EXPECT_TRUE(lostSync);
	EXPECT_GT(decoder.totalTriggerErrorCounter, 0u);
}
}

TEST(Suzuki36_2_2, GeometryAndRequestedTdcBaseline) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_SUZUKI_36_2_2);
	const auto& shape = eth.engine.triggerCentral.triggerShape;
	ASSERT_FALSE(shape.shapeDefinitionError);
	EXPECT_EQ(FOUR_STROKE_CRANK_SENSOR, shape.getWheelOperationMode());
	EXPECT_EQ(SyncEdge::RiseOnly, shape.syncEdge);
	EXPECT_FALSE(shape.needSecondTriggerInput);
	EXPECT_TRUE(shape.needsDisambiguation());
	EXPECT_FLOAT_EQ(360, shape.getCycleDuration());
	// Configuration baseline only: the physical TDC relationship is unverified.
	EXPECT_FLOAT_EQ(75, shape.tdcPosition);
	EXPECT_EQ(13, shape.gapTrackingLength);
	EXPECT_EQ(2 * syncTooth, shape.getTriggerWaveformSynchPointIndex());
	ASSERT_EQ(static_cast<size_t>(2 * toothCount), shape.getSize());
	for (int i = 0; i < toothCount; i++) {
		EXPECT_FLOAT_EQ(rises[i], shape.getSwitchAngle(2 * i));
		EXPECT_FLOAT_EQ(rises[i] + 5, shape.getSwitchAngle(2 * i + 1));
	}
}

TEST(Suzuki36_2_2, AcquiresCorrectCrankPhaseFromEveryToothAndSpeed) {
	for (int rpm : {80, 200, 1000, 6500}) {
		for (int start = 0; start < toothCount; start++) {
			SCOPED_TRACE(::testing::Message() << "start=" << start << " rpm=" << rpm);
			ASSERT_NO_FATAL_FAILURE(checkStartup(start, rpm));
		}
	}
}

TEST(Suzuki36_2_2, TracksAccelerationAndJitterWithoutConfusingTheTwoGaps) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_SUZUKI_36_2_2);
	TriggerDecoderBase decoder("Suzuki acceleration");
	efitick_t now = 0;
	float usPerDegree = 1000;
	for (int n = 0; n < 12 * toothCount; n++) {
		const int index = n % toothCount;
		const float jitter = n % 2 == 0 ? 0.97f : 1.03f;
		now += US2NT(static_cast<int>(gapBefore(index) * usPerDegree * jitter));
		feed(eth, decoder, now);
		if (n >= toothCount + 14) {
			expectPhase(decoder, index);
		}
		usPerDegree *= 0.99f;
	}
	EXPECT_EQ(0u, decoder.totalTriggerErrorCounter);
}

TEST(Suzuki36_2_2, UninterruptedTeethCannotSynchronize) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_SUZUKI_36_2_2);
	TriggerDecoderBase decoder("Suzuki invalid wheel");
	efitick_t now = 0;
	for (int n = 0; n < 5 * toothCount; n++) {
		now += US2NT(1000);
		feed(eth, decoder, now);
		ASSERT_FALSE(decoder.shaft_is_synchronized);
	}
}

TEST(Suzuki36_2_2, MissingPulseIsDetectedAndCleanSignalResynchronizes) {
	checkPulseFault(false);
}

TEST(Suzuki36_2_2, ExtraPulseIsDetectedAndCleanSignalResynchronizes) {
	checkPulseFault(true);
}

TEST(Suzuki36_2_2, StopRequiresNewSynchronization) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_SUZUKI_36_2_2);
	TriggerDecoderBase decoder("Suzuki restart");
	efitick_t now = 0;
	for (int n = 0; n < 3 * toothCount; n++) {
		now += US2NT(gapBefore(n % toothCount) * 100);
		feed(eth, decoder, now);
	}
	ASSERT_TRUE(decoder.shaft_is_synchronized);
	now += US2NT(2000000);
	feed(eth, decoder, now);
	ASSERT_FALSE(decoder.shaft_is_synchronized);
	for (int n = 1; n < 3 * toothCount; n++) {
		const int index = n % toothCount;
		now += US2NT(gapBefore(index) * 100);
		feed(eth, decoder, now);
		if (n >= toothCount + 14) {
			expectPhase(decoder, index);
		}
	}
}

TEST(Suzuki36_2_2, PrimaryInputFromEveryEdgeReportsCrankRpmWithoutCamPhase) {
	for (int start = 0; start < 2 * toothCount; start++) {
		SCOPED_TRACE(start);
		EngineTestHelper eth(engine_type_e::TEST_ENGINE);
		eth.setTriggerType(trigger_type_e::TT_SUZUKI_36_2_2);
		for (int n = 0; n < 6 * 2 * toothCount; n++) {
			const int edge = (start + n) % (2 * toothCount);
			const int tooth = edge / 2;
			// Each high pulse is 5 degrees. At 100 us/degree RPM is 1666.67.
			eth.moveTimeForwardUs((edge % 2 == 0 ? gapBefore(tooth) - 5 : 5) * 100);
			if (edge % 2 == 0) {
				eth.firePrimaryTriggerRise();
			} else {
				eth.firePrimaryTriggerFall();
			}
		}
		EXPECT_NEAR(1666.67f, Sensor::getOrZero(SensorType::Rpm), 1.0f);
		EXPECT_TRUE(eth.engine.triggerCentral.triggerState.shaft_is_synchronized);
		EXPECT_FALSE(eth.engine.triggerCentral.triggerState.hasSynchronizedPhase());
		EXPECT_EQ(0u, eth.engine.triggerCentral.triggerState.totalTriggerErrorCounter);
	}
}
