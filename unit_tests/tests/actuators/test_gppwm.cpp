#include "pch.h"

#include "gppwm_channel.h"
#include "gppwm.h"
#include "dc_motor.h"
#include "dc_motors.h"

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

TEST(GpPwm, OutputOnOffViaPwmFallback) {
	// on-off mode with no plain output pin (H-bridge output, #9673): hysteresis state snaps IPwm duty to 0/1
	GppwmChannel ch;

	gppwm_channel cfg;
	cfg.onAboveDuty = 50;
	cfg.offBelowDuty = 40;

	StrictMock<MockPwm> pwm;

	{
		InSequence i;

		// Rising edge test
		EXPECT_CALL(pwm, setSimplePwmDutyCycle(0.0f));
		EXPECT_CALL(pwm, setSimplePwmDutyCycle(1.0f));
		EXPECT_CALL(pwm, setSimplePwmDutyCycle(1.0f));

		// Falling edge test
		EXPECT_CALL(pwm, setSimplePwmDutyCycle(1.0f));
		EXPECT_CALL(pwm, setSimplePwmDutyCycle(0.0f));
		EXPECT_CALL(pwm, setSimplePwmDutyCycle(0.0f));
	}

	ch.init(false, &pwm, nullptr, nullptr, &cfg);

	// Test rising edge - these should output 0, 1, 1
	ch.setOutput(49.0f);
	ch.setOutput(51.0f);
	ch.setOutput(49.0f);

	// Test falling edge - these should output 1, 0, 0
	ch.setOutput(41.0f);
	ch.setOutput(39.0f);
	ch.setOutput(41.0f);
}

// https://github.com/rusefi/rusefi/issues/9673 H-bridge as a general-purpose output:
// DC_Gppwm1 function routes GPPWM channel 1 duty into DC motor slot 0
TEST(GpPwm, DcMotorOutputOnOff) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->etbFunctions[0] = DC_Gppwm1;
	engineConfiguration->etbFunctions[1] = DC_None;

	auto& cfg = engineConfiguration->gppwm[0];
	cfg.pwmFrequency = 0; // on-off mode
	cfg.onAboveDuty = 50;
	cfg.offBelowDuty = 40;
	cfg.loadAxis = GPPWM_Tps;
	cfg.rpmAxis = GPPWM_Rpm;
	setTable(cfg.table, 100);

	Sensor::setMockValue(SensorType::Tps1, 35.0f);
	Sensor::setMockValue(SensorType::Rpm, 1200);

	// TEST_ENGINE construction already started DC hardware for the default DC_Wastegate function
	resetDcHardwareForUnitTest();
	initGpPwm();

	DcMotor* motor = getDcMotorForUnitTest(0);

	// table above onAboveDuty -> H-bridge fully on
	updateGppwm();
	EXPECT_FLOAT_EQ(1.0f, motor->get());
	EXPECT_FLOAT_EQ(100, engine->outputChannels.gppwmOutput[0]);

	// table value inside the hysteresis band -> state holds
	setTable(cfg.table, 45);
	updateGppwm();
	EXPECT_FLOAT_EQ(1.0f, motor->get());

	// table below offBelowDuty -> H-bridge off
	setTable(cfg.table, 30);
	updateGppwm();
	EXPECT_FLOAT_EQ(0.0f, motor->get());
	EXPECT_FLOAT_EQ(0, engine->outputChannels.gppwmOutput[0]);
}

TEST(GpPwm, DcMotorOutputPwm) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// second H-bridge this time, driven by GPPWM channel 2
	engineConfiguration->etbFunctions[0] = DC_None;
	engineConfiguration->etbFunctions[1] = DC_Gppwm2;

	auto& cfg = engineConfiguration->gppwm[1];
	cfg.pwmFrequency = 800; // PWM mode; the H-bridge actually switches at etbFreq
	cfg.loadAxis = GPPWM_Tps;
	cfg.rpmAxis = GPPWM_Rpm;
	setTable(cfg.table, 35);

	Sensor::setMockValue(SensorType::Tps1, 35.0f);
	Sensor::setMockValue(SensorType::Rpm, 1200);

	resetDcHardwareForUnitTest();
	initGpPwm();

	DcMotor* motor = getDcMotorForUnitTest(1);

	updateGppwm();
	EXPECT_FLOAT_EQ(0.35f, motor->get());

	setTable(cfg.table, 80);
	updateGppwm();
	EXPECT_FLOAT_EQ(0.80f, motor->get());
}

TEST(GpPwm, TestGetOutput) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	GppwmChannel ch;

	gppwm_channel cfg;
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
