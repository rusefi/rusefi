#include "pch.h"

#include "gppwm_channel.h"
#include "gppwm.h"

#include "mocks.h"

using ::testing::InSequence;
using ::testing::StrictMock;

TEST(GpPwm, OutputWithPwm) {
	GppwmChannel ch;

	gppwm_channel cfg{};

	StrictMock<MockPwm> pwm;

	// Shouldn't throw with no config
	EXPECT_NO_THROW(ch.setOutput(10));

	{
		InSequence i;
		EXPECT_CALL(pwm, setSimplePwmDutyCycle(0.25f));
		EXPECT_CALL(pwm, setSimplePwmDutyCycle(0.75f));
		EXPECT_CALL(pwm, setSimplePwmDutyCycle(0.0f));
		EXPECT_CALL(pwm, setSimplePwmDutyCycle(1.0f));
	}

	ch.init(true, &pwm, nullptr, nullptr, &cfg);

	// Set the output - should set directly to PWM
	ch.setOutput(25.0f);
	ch.setOutput(75.0f);

	// Test clamping behavior - should clamp to [0, 100]
	ch.setOutput(-10.0f);
	ch.setOutput(110.0f);
}

TEST(GpPwm, OutputOnOff) {
	GppwmChannel ch;

	gppwm_channel cfg{};
	cfg.onAboveDuty = 50;
	cfg.offBelowDuty = 40;

	MockOutputPin pin;

	{
		InSequence i;

		// Rising edge test
		EXPECT_CALL(pin, setValue(0, false));
		EXPECT_CALL(pin, setValue(1, false));
		EXPECT_CALL(pin, setValue(1, false));

		// Falling edge test
		EXPECT_CALL(pin, setValue(1, false));
		EXPECT_CALL(pin, setValue(0, false));
		EXPECT_CALL(pin, setValue(0, false));
	}

	ch.init(false, nullptr, &pin, nullptr, &cfg);

	// Test rising edge - these should output 0, 1, 1
	ch.setOutput(49.0f);
	ch.setOutput(51.0f);
	ch.setOutput(49.0f);

	// Test falling edge - these should output 1, 0, 0
	ch.setOutput(41.0f);
	ch.setOutput(39.0f);
	ch.setOutput(41.0f);
}

TEST(GpPwm, TestGetOutput) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	GppwmChannel ch;

	gppwm_channel cfg{};
	cfg.loadAxis = GPPWM_Tps;
	cfg.rpmAxis = GPPWM_Rpm;
	cfg.dutyIfError = 21.0f;

	MockVp3d table;

	EXPECT_CALL(table, getValue(1200, 35.0f))
		.WillRepeatedly([](float x, float tps) {
			return tps;
		});

	ch.init(false, nullptr, nullptr, &table, &cfg);

	Sensor::resetAllMocks();

	// Should return dutyIfError
	EXPECT_FLOAT_EQ(21.0f, ch.getOutput(-1).Result);

	// Set TPS, should return tps value
	Sensor::setMockValue(SensorType::Tps1, 35.0f);
	Sensor::setMockValue(SensorType::Rpm, 1200);
	EXPECT_FLOAT_EQ(35.0f, ch.getOutput(-1).Result);
}

// ---------------------------------------------------------------------------
// #9386: the duty band stops chatter caused by noise, but not by a table value
// which drifts slowly back and forth across a threshold. minimumStateTime holds
// each state for a while so a relay is not switched repeatedly.
// ---------------------------------------------------------------------------

TEST(GpPwm, OnOffMinimumStateTimeHoldsState) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	gppwm_channel cfg{};
	cfg.onAboveDuty = 50;
	cfg.offBelowDuty = 40;
	// scaled_channel<uint8_t, 10> - assign seconds directly, it stores tenths
	cfg.minimumStateTime = 2;

	MockOutputPin pin;
	EXPECT_CALL(pin, setValue(::testing::_, false)).Times(::testing::AnyNumber());

	GppwmChannel ch;
	ch.init(false, nullptr, &pin, nullptr, &cfg);

	// the very first switch is not held back - nothing has happened yet
	EXPECT_FLOAT_EQ(100, ch.setOutput(60));

	// wants to turn off immediately, but the minimum has not elapsed
	EXPECT_FLOAT_EQ(100, ch.setOutput(30));
	eth.moveTimeForwardAndInvokeEventsUs(1'000'000);
	EXPECT_FLOAT_EQ(100, ch.setOutput(30));

	// past two seconds it is allowed through
	eth.moveTimeForwardAndInvokeEventsUs(1'100'000);
	EXPECT_FLOAT_EQ(0, ch.setOutput(30));

	// and the hold applies to the next switch as well
	EXPECT_FLOAT_EQ(0, ch.setOutput(60));
	eth.moveTimeForwardAndInvokeEventsUs(2'100'000);
	EXPECT_FLOAT_EQ(100, ch.setOutput(60));
}

TEST(GpPwm, OnOffZeroMinimumStateTimeKeepsOldBehaviour) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	gppwm_channel cfg{};
	cfg.onAboveDuty = 50;
	cfg.offBelowDuty = 40;
	cfg.minimumStateTime = 0;

	MockOutputPin pin;
	EXPECT_CALL(pin, setValue(::testing::_, false)).Times(::testing::AnyNumber());

	GppwmChannel ch;
	ch.init(false, nullptr, &pin, nullptr, &cfg);

	// no time passes between any of these - every crossing switches straight away
	EXPECT_FLOAT_EQ(100, ch.setOutput(60));
	EXPECT_FLOAT_EQ(0, ch.setOutput(30));
	EXPECT_FLOAT_EQ(100, ch.setOutput(60));
	EXPECT_FLOAT_EQ(0, ch.setOutput(30));
}

TEST(GpPwm, OnOffMinimumStateTimeDoesNotBlockStayingPut) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	gppwm_channel cfg{};
	cfg.onAboveDuty = 50;
	cfg.offBelowDuty = 40;
	cfg.minimumStateTime = 2;

	MockOutputPin pin;
	EXPECT_CALL(pin, setValue(::testing::_, false)).Times(::testing::AnyNumber());

	GppwmChannel ch;
	ch.init(false, nullptr, &pin, nullptr, &cfg);

	EXPECT_FLOAT_EQ(100, ch.setOutput(60));

	// inside the duty band: no switch is being asked for, so the timer is irrelevant
	EXPECT_FLOAT_EQ(100, ch.setOutput(45));
	EXPECT_FLOAT_EQ(100, ch.setOutput(45));

	// still on, and once the value drops below the band the hold has long expired
	eth.moveTimeForwardAndInvokeEventsUs(3'000'000);
	EXPECT_FLOAT_EQ(0, ch.setOutput(30));
}
