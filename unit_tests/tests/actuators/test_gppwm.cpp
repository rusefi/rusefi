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

TEST(GpPwmCoverage, SensorFailureUsesFallbackAndRecoversThroughUpdate) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	GppwmChannel ch;
	gppwm_channel cfg{};
	cfg.rpmAxis = GPPWM_Rpm;
	cfg.loadAxis = GPPWM_Tps;
	cfg.dutyIfError = 20;
	StrictMock<MockVp3d> table;
	StrictMock<MockPwm> pwm;
	ch.init(true, &pwm, nullptr, &table, &cfg);

	Sensor::setMockValue(SensorType::Rpm, 1200);
	Sensor::setMockValue(SensorType::Tps1, 35);
	{
		InSequence sequence;
		EXPECT_CALL(table, getValue(1200, 35)).WillOnce(Return(60));
		EXPECT_CALL(pwm, setSimplePwmDutyCycle(0.6f));
		// Each failed axis must bypass the table and use the configured fallback.
		EXPECT_CALL(pwm, setSimplePwmDutyCycle(0.2f));
		EXPECT_CALL(pwm, setSimplePwmDutyCycle(0.2f));
		EXPECT_CALL(table, getValue(1200, 35)).WillOnce(Return(75));
		EXPECT_CALL(pwm, setSimplePwmDutyCycle(0.75f));
	}
	EXPECT_FLOAT_EQ(60, ch.update(0).Result);
	Sensor::setInvalidMockValue(SensorType::Tps1);
	auto result = ch.update(0);
	EXPECT_FLOAT_EQ(20, result.Result);
	EXPECT_FLOAT_EQ(1200, result.X);
	EXPECT_FLOAT_EQ(0, result.Y);

	Sensor::setMockValue(SensorType::Tps1, 35);
	Sensor::setInvalidMockValue(SensorType::Rpm);
	result = ch.update(0);
	EXPECT_FLOAT_EQ(20, result.Result);
	EXPECT_FLOAT_EQ(0, result.X);
	EXPECT_FLOAT_EQ(35, result.Y);

	Sensor::setMockValue(SensorType::Rpm, 1200);
	EXPECT_FLOAT_EQ(75, ch.update(0).Result);
}

TEST(GpPwmCoverage, SwitchThresholdEqualityRetainsStateAndReportsActualOutput) {
	GppwmChannel ch;
	gppwm_channel cfg{};
	cfg.onAboveDuty = 50;
	cfg.offBelowDuty = 40;
	StrictMock<MockOutputPin> pin;
	ch.init(false, nullptr, &pin, nullptr, &cfg);

	struct Step { float duty; bool on; };
	for (auto step : {Step{50, false}, {51, true}, {50, true}, {40, true}, {39, false}, {40, false}}) {
		SCOPED_TRACE(step.duty);
		EXPECT_CALL(pin, setValue(step.on, false));
		EXPECT_FLOAT_EQ(step.on ? 100 : 0, ch.setOutput(step.duty));
		::testing::Mock::VerifyAndClearExpectations(&pin);
	}
}

TEST(GpPwmCoverage, CharacterizeNaNTableResultBypassingErrorDuty) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	GppwmChannel ch;
	gppwm_channel cfg{};
	cfg.rpmAxis = GPPWM_Rpm;
	cfg.loadAxis = GPPWM_Tps;
	cfg.dutyIfError = 21;
	StrictMock<MockVp3d> table;
	ch.init(false, nullptr, nullptr, &table, &cfg);
	Sensor::setMockValue(SensorType::Rpm, 1200);
	Sensor::setMockValue(SensorType::Tps1, 35);
	EXPECT_CALL(table, getValue(1200, 35)).WillOnce(Return(NAN));

	// Known bad behavior, deliberately captured before a separate fix (TDB):
	// getOutput checks the fallback duty for NaN instead of the table result.
	// A fix should change this expectation to the configured error duty, 21.
	EXPECT_TRUE(std::isnan(ch.getOutput(0).Result));
}
