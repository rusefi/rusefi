#include "pch.h"
#include "trigger_decoder.h"

namespace {
// Independent low-resolution edge timestamps from Ardu-Stim's optispark_lt1
// sample stream (720 samples per engine cycle). Do not generate these from
// the production waveform: an incorrect definition must fail the tests.
constexpr int edges[] = {86, 100, 176, 180, 266, 290, 356, 360,
                        446, 480, 536, 540, 626, 670, 716, 720};
constexpr int edgeCount = 16;
constexpr int syncEdge = 1;

trigger_event_e edgeType(int index) {
	return index % 2 == 0 ? SHAFT_PRIMARY_RISING : SHAFT_PRIMARY_FALLING;
}

int gapBefore(int index) {
	return index == 0 ? 86 : edges[index] - edges[index - 1];
}

void feed(EngineTestHelper& eth, TriggerDecoderBase& decoder, int index, efitick_t now) {
	setTimeNowNt(now);
	decoder.decodeTriggerEvent("Optispark", eth.engine.triggerCentral.triggerShape,
		nullptr, eth.engine.triggerCentral.primaryTriggerConfiguration, edgeType(index), now);
}

void expectPhase(const TriggerDecoderBase& decoder, int index) {
	ASSERT_TRUE(decoder.shaft_is_synchronized);
	EXPECT_EQ((index - syncEdge + edgeCount) % edgeCount, decoder.getCurrentIndex());
}
}

static void checkStartup(int start, int rpm) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_GM_LT1_OPTISPARK_8);
	TriggerDecoderBase decoder("Optispark startup");
	efitick_t now = 0;
	for (int n = 0; n < 5 * edgeCount; n++) {
		const int index = (start + n) % edgeCount;
		now += US2NT(gapBefore(index) * 60000000LL / (360 * rpm));
		feed(eth, decoder, index, now);
		if (decoder.shaft_is_synchronized) {
			expectPhase(decoder, index);
		}
		if (n >= edgeCount + 3) {
			ASSERT_TRUE(decoder.shaft_is_synchronized) << "event " << n;
		}
	}
	EXPECT_EQ(0u, decoder.totalTriggerErrorCounter);
}

TEST(Optispark, AcquiresCorrect720DegreePhaseFromEveryEdgeAndSpeed) {
	for (int rpm : {80, 200, 1000, 6500}) {
		for (int start = 0; start < edgeCount; start++) {
			SCOPED_TRACE(::testing::Message() << "start=" << start << " rpm=" << rpm);
			ASSERT_NO_FATAL_FAILURE(checkStartup(start, rpm));
		}
	}
}

TEST(Optispark, ReverseRotationCannotEstablishPhase) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_GM_LT1_OPTISPARK_8);
	TriggerDecoderBase decoder("Optispark reverse rotation");
	efitick_t now = 0;
	for (int n = 0; n < 5 * edgeCount; n++) {
		const int index = edgeCount - 1 - n % edgeCount;
		now += US2NT(gapBefore((index + 1) % edgeCount) * 100);
		// Edges reverse polarity when the wheel turns backwards.
		setTimeNowNt(now);
		decoder.decodeTriggerEvent("Optispark reverse", eth.engine.triggerCentral.triggerShape,
			nullptr, eth.engine.triggerCentral.primaryTriggerConfiguration,
			index % 2 == 0 ? SHAFT_PRIMARY_FALLING : SHAFT_PRIMARY_RISING, now);
		ASSERT_FALSE(decoder.shaft_is_synchronized) << "event " << n;
	}
}

TEST(Optispark, WaveformUsesOneCamSpeedInputAndBothEdges) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_GM_LT1_OPTISPARK_8);
	const auto& shape = eth.engine.triggerCentral.triggerShape;
	ASSERT_FALSE(shape.shapeDefinitionError);
	EXPECT_EQ(FOUR_STROKE_CAM_SENSOR, shape.getWheelOperationMode());
	EXPECT_EQ(SyncEdge::Both, shape.syncEdge);
	EXPECT_FALSE(shape.needSecondTriggerInput);
	EXPECT_FALSE(shape.shapeWithoutTdc);
	ASSERT_EQ(static_cast<size_t>(edgeCount), shape.getSize());
	EXPECT_EQ(syncEdge, shape.getTriggerWaveformSynchPointIndex());
	for (int i = 0; i < edgeCount; i++) {
		EXPECT_FLOAT_EQ(edges[i], shape.getSwitchAngle(i));
	}
}

TEST(Optispark, TracksAccelerationAndIntervalJitter) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_GM_LT1_OPTISPARK_8);
	TriggerDecoderBase decoder("Optispark acceleration");
	efitick_t now = 0;
	float usPerDegree = 1000; // Start near 167 RPM.
	for (int n = 0; n < 12 * edgeCount; n++) {
		const int index = n % edgeCount;
		const float jitter = n % 2 == 0 ? 0.97f : 1.03f;
		now += US2NT(static_cast<int>(gapBefore(index) * usPerDegree * jitter));
		feed(eth, decoder, index, now);
		if (n >= edgeCount + 3) {
			expectPhase(decoder, index);
		}
		usPerDegree *= 0.99f;
	}
	EXPECT_EQ(0u, decoder.totalTriggerErrorCounter);
}

TEST(Optispark, EqualWidthPulsesCannotEstablishPhase) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_GM_LT1_OPTISPARK_8);
	TriggerDecoderBase decoder("Optispark invalid wheel");
	efitick_t now = 0;
	for (int n = 0; n < 5 * edgeCount; n++) {
		now += US2NT(4500);
		feed(eth, decoder, n % edgeCount, now);
		ASSERT_FALSE(decoder.shaft_is_synchronized);
	}
}

TEST(Optispark, MissingPulseIsDetectedAndCleanSignalResynchronizes) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_GM_LT1_OPTISPARK_8);
	TriggerDecoderBase decoder("Optispark missing pulse");
	efitick_t now = 0;
	bool lostSync = false;
	for (int n = 0; n < 7 * edgeCount; n++) {
		const int index = n % edgeCount;
		now += US2NT(gapBefore(index) * 100);
		// Drop a complete pulse after two good cycles, retaining elapsed time.
		if (n == 2 * edgeCount + 4 || n == 2 * edgeCount + 5) {
			continue;
		}
		feed(eth, decoder, index, now);
		if (n >= 2 * edgeCount + 6 && !decoder.shaft_is_synchronized) {
			lostSync = true;
		}
		if (n >= 5 * edgeCount) {
			expectPhase(decoder, index);
		}
	}
	EXPECT_TRUE(lostSync);
	EXPECT_GT(decoder.totalTriggerErrorCounter, 0u);
}

TEST(Optispark, EngineStopRequiresNewSynchronization) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_GM_LT1_OPTISPARK_8);
	TriggerDecoderBase decoder("Optispark restart");
	efitick_t now = 0;
	for (int n = 0; n < 3 * edgeCount; n++) {
		const int index = n % edgeCount;
		now += US2NT(gapBefore(index) * 100);
		feed(eth, decoder, index, now);
	}
	ASSERT_TRUE(decoder.shaft_is_synchronized);
	now += US2NT(2000000);
	feed(eth, decoder, 0, now);
	ASSERT_FALSE(decoder.shaft_is_synchronized);
	for (int n = 1; n < 3 * edgeCount; n++) {
		const int index = n % edgeCount;
		now += US2NT(gapBefore(index) * 100);
		feed(eth, decoder, index, now);
		if (n >= edgeCount + 3) {
			expectPhase(decoder, index);
		}
	}
}

TEST(Optispark, ReportsCrankshaftRpmWithOnlyLowResolutionInput) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_GM_LT1_OPTISPARK_8);
	for (int n = 0; n < 6 * edgeCount; n++) {
		const int index = n % edgeCount;
		// 100 us/degree = 1666.67 crank RPM, not 833.33 cam RPM.
		eth.moveTimeForwardUs(gapBefore(index) * 100);
		if (index % 2 == 0) {
			eth.firePrimaryTriggerRise();
		} else {
			eth.firePrimaryTriggerFall();
		}
	}
	EXPECT_NEAR(1666.67f, Sensor::getOrZero(SensorType::Rpm), 1.0f);
	EXPECT_TRUE(eth.engine.triggerCentral.triggerState.shaft_is_synchronized);
	EXPECT_TRUE(eth.engine.triggerCentral.triggerState.hasSynchronizedPhase());
}

TEST(Optispark, ExtraPulseIsDetectedAndCleanSignalResynchronizes) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_GM_LT1_OPTISPARK_8);
	// Overflow diagnostics are suppressed at zero RPM; this test drives the
	// decoder directly rather than the RPM calculator. Model a running engine.
	Sensor::setMockValue(SensorType::Rpm, 1666.67f);
	TriggerDecoderBase decoder("Optispark extra pulse");
	efitick_t now = 0;
	bool lostSync = false;
	for (int n = 0; n < 7 * edgeCount; n++) {
		const int index = n % edgeCount;
		if (n == 2 * edgeCount + 6) {
			// An extra 100 us pulse in the low interval before the next real rise.
			feed(eth, decoder, 0, now + US2NT(100));
			feed(eth, decoder, 1, now + US2NT(200));
		}
		now += US2NT(gapBefore(index) * 100);
		feed(eth, decoder, index, now);
		if (n >= 2 * edgeCount + 6 && !decoder.shaft_is_synchronized) {
			lostSync = true;
		}
		if (n >= 5 * edgeCount) {
			expectPhase(decoder, index);
		}
	}
	EXPECT_TRUE(lostSync);
	EXPECT_GT(decoder.totalTriggerErrorCounter, 0u);
}
