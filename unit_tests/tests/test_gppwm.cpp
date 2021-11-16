#include "pch.h"

#include "gppwm_channel.h"
#include "gppwm.h"

#include "mocks.h"

using ::testing::InSequence;
using ::testing::StrictMock;

TEST(GpPwm, OutputWithPwm) {
	GppwmChannel ch;

	gppwm_channel cfg;

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

	gppwm_channel cfg;
	cfg.onAboveDuty = 50;
	cfg.offBelowDuty = 40;

	MockOutputPin pin;

	{
		InSequence i;

		// Rising edge test
		EXPECT_CALL(pin, setValue(0));
		EXPECT_CALL(pin, setValue(1));
		EXPECT_CALL(pin, setValue(1));

		// Falling edge test
		EXPECT_CALL(pin, setValue(1));
		EXPECT_CALL(pin, setValue(0));
		EXPECT_CALL(pin, setValue(0));
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

TEST(GpPwm, GetOutput) {
	EngineTestHelper eth(TEST_ENGINE);
	GppwmChannel ch;

	gppwm_channel cfg;
	cfg.loadAxis = GPPWM_Tps;
	cfg.dutyIfError = 21.0f;

	MockVp3d table;

	engine->rpmCalculator.mockRpm = 1200;
	EXPECT_CALL(table, getValue(1200 / RPM_1_BYTE_PACKING_MULT, 35.0f))
		.WillRepeatedly([](float x, float tps) {
			return tps;
		});

	ch.init(false, nullptr, nullptr, &table, &cfg);

	Sensor::resetAllMocks();

	// Should return dutyIfError
	EXPECT_FLOAT_EQ(21.0f, ch.getOutput());

	// Set TPS, should return tps value
	Sensor::setMockValue(SensorType::Tps1, 35.0f);
	EXPECT_FLOAT_EQ(35.0f, ch.getOutput());	
}
