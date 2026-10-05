#include "pch.h"
#include "trigger_decoder.h"

namespace {
// Synthetic 36-slot wheel, slots 0 and 18 missing. Independent of the
// production waveform: seventeen teeth per 180-degree period.
constexpr int teeth = 17;
int gapBefore(int tooth) {
	return tooth == 0 ? 20 : 10;
}

void feed(EngineTestHelper& eth, TriggerDecoderBase& decoder, efitick_t now) {
	setTimeNowNt(now);
	decoder.decodeTriggerEvent("VK56DE", eth.engine.triggerCentral.triggerShape,
		nullptr, eth.engine.triggerCentral.primaryTriggerConfiguration, SHAFT_PRIMARY_RISING, now);
}

void expectPosition(const TriggerDecoderBase& decoder, int tooth) {
	ASSERT_TRUE(decoder.shaft_is_synchronized);
	EXPECT_EQ(2 * ((tooth + teeth - 1) % teeth), decoder.getCurrentIndex());
}

void checkStartup(int start, int rpm) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_NISSAN_VK56DE);
	TriggerDecoderBase decoder("VK56DE startup");
	efitick_t now = 0;
	for (int n = 0; n < 6 * teeth; n++) {
		const int tooth = (start + n) % teeth;
		now += US2NT(gapBefore(tooth) * 60000000LL / (360 * rpm));
		feed(eth, decoder, now);
		if (decoder.shaft_is_synchronized) {
			expectPosition(decoder, tooth);
		}
		if (n >= teeth + 3) {
			ASSERT_TRUE(decoder.shaft_is_synchronized);
		}
	}
	EXPECT_EQ(0u, decoder.totalTriggerErrorCounter);
}
}

TEST(NissanVK56, Geometry) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_NISSAN_VK56DE);
	const auto& shape = eth.engine.triggerCentral.triggerShape;
	ASSERT_FALSE(shape.shapeDefinitionError);
	EXPECT_EQ(FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR, shape.getWheelOperationMode());
	EXPECT_EQ(SyncEdge::RiseOnly, shape.syncEdge);
	EXPECT_FALSE(shape.needSecondTriggerInput);
	ASSERT_EQ(34u, shape.getSize());
	EXPECT_EQ(180, shape.getCycleDuration());
	EXPECT_EQ(2, shape.getTriggerWaveformSynchPointIndex());
	for (int i = 0; i < teeth; i++) {
		EXPECT_FLOAT_EQ(15 + 10 * i, shape.getSwitchAngle(2 * i));
		EXPECT_FLOAT_EQ(20 + 10 * i, shape.getSwitchAngle(2 * i + 1));
	}
}

TEST(NissanVK56, AcquiresFromEveryToothAtCrankingAndRunningSpeeds) {
	for (int rpm : {80, 200, 1000, 6500}) {
		for (int start = 0; start < teeth; start++) {
			SCOPED_TRACE(::testing::Message() << "start=" << start << " rpm=" << rpm);
			ASSERT_NO_FATAL_FAILURE(checkStartup(start, rpm));
		}
	}
}

TEST(NissanVK56, TracksAccelerationAndJitter) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_NISSAN_VK56DE);
	TriggerDecoderBase decoder("VK56DE acceleration");
	efitick_t now = 0;
	float usPerDegree = 1000;
	for (int n = 0; n < 12 * teeth; n++) {
		const int tooth = n % teeth;
		now += US2NT(static_cast<int>(gapBefore(tooth) * usPerDegree * (n % 2 ? 0.97f : 1.03f)));
		feed(eth, decoder, now);
		if (n >= teeth + 3) {
			expectPosition(decoder, tooth);
		}
		usPerDegree *= 0.99f;
	}
	EXPECT_EQ(0u, decoder.totalTriggerErrorCounter);
}

TEST(NissanVK56, RejectsQR25AndUniformWheel) {
	for (int missing : {0, 2}) {
		EngineTestHelper eth(engine_type_e::TEST_ENGINE);
		eth.setTriggerType(trigger_type_e::TT_NISSAN_VK56DE);
		TriggerDecoderBase decoder("VK56DE wrong wheel");
		efitick_t now = 0;
		for (int n = 0; n < 6 * (18 - missing); n++) {
			now += US2NT(n % (18 - missing) == 0 ? (missing + 1) * 1000 : 1000);
			feed(eth, decoder, now);
			ASSERT_FALSE(decoder.shaft_is_synchronized);
		}
	}
}

TEST(NissanVK56, MissingToothProducesErrorAndReacquires) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_NISSAN_VK56DE);
	TriggerDecoderBase decoder("VK56DE missing tooth");
	efitick_t now = 0;
	for (int n = 0; n < 8 * teeth; n++) {
		const int tooth = n % teeth;
		now += US2NT(gapBefore(tooth) * 100);
		if (n == 2 * teeth + 5) {
			continue;
		}
		feed(eth, decoder, now);
		if (n >= 5 * teeth) {
			expectPosition(decoder, tooth);
		}
	}
	EXPECT_GT(decoder.totalTriggerErrorCounter, 0u);
}

TEST(NissanVK56, ReportsCrankRpmWithoutClaimingCamPhase) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->cylindersCount = 8;
	// A valid V8 fixture; firing order does not affect crank RPM decoding.
	engineConfiguration->firingOrder = FO_1_8_7_2_6_5_4_3;
	eth.setTriggerType(trigger_type_e::TT_NISSAN_VK56DE);
	for (int n = 0; n < 8 * teeth; n++) {
		eth.moveTimeForwardUs(gapBefore(n % teeth) * 100 - 500);
		eth.firePrimaryTriggerRise();
		eth.moveTimeForwardUs(500);
		eth.firePrimaryTriggerFall();
	}
	EXPECT_NEAR(1666.67f, Sensor::getOrZero(SensorType::Rpm), 1.0f);
	EXPECT_TRUE(eth.engine.triggerCentral.triggerState.shaft_is_synchronized);
	EXPECT_FALSE(eth.engine.triggerCentral.triggerState.hasSynchronizedPhase());
}

TEST(NissanVK56, StopAndRestartRequiresSynchronization) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_NISSAN_VK56DE);
	TriggerDecoderBase decoder("VK56DE restart");
	efitick_t now = 0;
	for (int n = 0; n < 3 * teeth; n++) {
		now += US2NT(gapBefore(n % teeth) * 100);
		feed(eth, decoder, now);
	}
	ASSERT_TRUE(decoder.shaft_is_synchronized);
	now += US2NT(2000000);
	feed(eth, decoder, now);
	ASSERT_FALSE(decoder.shaft_is_synchronized);
	for (int n = 1; n < 4 * teeth; n++) {
		now += US2NT(gapBefore(n % teeth) * 100);
		feed(eth, decoder, now);
		if (n >= teeth + 3) {
			expectPosition(decoder, n % teeth);
		}
	}
}

TEST(NissanVK56, ExtraToothProducesErrorAndReacquires) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_NISSAN_VK56DE);
	Sensor::setMockValue(SensorType::Rpm, 1666.67f);
	TriggerDecoderBase decoder("VK56DE extra tooth");
	efitick_t now = 0;
	for (int n = 0; n < 8 * teeth; n++) {
		if (n == 2 * teeth + 5) {
			feed(eth, decoder, now + US2NT(500));
		}
		now += US2NT(gapBefore(n % teeth) * 100);
		feed(eth, decoder, now);
		if (n >= 5 * teeth) {
			expectPosition(decoder, n % teeth);
		}
	}
	EXPECT_GT(decoder.totalTriggerErrorCounter, 0u);
}
