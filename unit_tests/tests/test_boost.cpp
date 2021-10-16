#include "pch.h"

#include "boost_control.h"

using ::testing::_;
using ::testing::StrictMock;

TEST(BoostControl, Setpoint) {
	MockVp3d targetMap;

	// Just pass TPS input to output
	EXPECT_CALL(targetMap, getValue(_, _))
		.WillRepeatedly([](float xRpm, float tps) { return tps * 2; });

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	engineConfiguration->boostType = CLOSED_LOOP;

	BoostController bc;
	INJECT_ENGINE_REFERENCE(&bc);

	// Should return unexpected without a pedal map cfg'd
	EXPECT_EQ(bc.getSetpoint(), unexpected);

	// Now init with mock target map
	bc.init(nullptr, nullptr, &targetMap, nullptr);

	// Should still return unxepected since TPS is invalid
	EXPECT_EQ(bc.getSetpoint(), unexpected);

	// Configure TPS, should get passthru of tps value
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 35.0f);
	EXPECT_FLOAT_EQ(bc.getSetpoint().value_or(-1), 35.0f);
}

TEST(BoostControl, ObservePlant) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	BoostController bc;
	INJECT_ENGINE_REFERENCE(&bc);

	Sensor::resetMockValue(SensorType::Map);
	// Check that invalid MAP returns unexpected
	EXPECT_EQ(bc.observePlant(), unexpected);

	// Test valid MAP value
	Sensor::setMockValue(SensorType::Map, 150);

	EXPECT_FLOAT_EQ(bc.observePlant().value_or(0), 150.0f);
}

TEST(BoostControl, OpenLoop) {
	MockVp3d openMap;

	// Just pass MAP input to output
	EXPECT_CALL(openMap, getValue(_, _))
		.WillRepeatedly([](float xRpm, float tps) { return tps * 2; });

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	BoostController bc;
	INJECT_ENGINE_REFERENCE(&bc);

	// Without table set, should return unexpected
	EXPECT_EQ(bc.getOpenLoop(0), unexpected);

	bc.init(nullptr, &openMap, nullptr, nullptr);

	// Should pass TPS value thru
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 47.0f);
	EXPECT_FLOAT_EQ(bc.getOpenLoop(0).value_or(-1), 47.0f);
}

TEST(BoostControl, ClosedLoop) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	BoostController bc;
	INJECT_ENGINE_REFERENCE(&bc);

	pid_s pidCfg = {
		1, 0, 0,	 // P controller, easier to test
		0,	// no offset
		5,	// 5ms period
		-100, 100 // min/max output
	};

	bc.init(nullptr, nullptr, nullptr, &pidCfg);

	// Enable closed loop
	CONFIG(boostType) = CLOSED_LOOP;
	// Minimum 75kpa
	CONFIG(minimumBoostClosedLoopMap) = 75;

	// At 0 RPM, closed loop is disabled
	ENGINE(rpmCalculator.mockRpm) = 0;
	EXPECT_EQ(0, bc.getClosedLoop(150, 100).value_or(-1000));

	// too low MAP, disable closed loop
	ENGINE(rpmCalculator.mockRpm) = 0;
	EXPECT_EQ(0, bc.getClosedLoop(150, 50).value_or(-1000));

	// With RPM, we should get an output
	ENGINE(rpmCalculator.mockRpm) = 1000;
	// Actual is below target -> positive output
	EXPECT_FLOAT_EQ(50, bc.getClosedLoop(150, 100).value_or(-1000));
	// Actual is above target -> negative output
	EXPECT_FLOAT_EQ(-25.0f, bc.getClosedLoop(150, 175).value_or(-1000));

	// Disabling closed loop should return 0
	CONFIG(boostType) = OPEN_LOOP;
	EXPECT_FLOAT_EQ(0, bc.getClosedLoop(150, 175).value_or(-1000));
}

TEST(BoostControl, SetOutput) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	StrictMock<MockPwm> pwm;
	StrictMock<MockEtb> etb;
	BoostController bc;
	INJECT_ENGINE_REFERENCE(&bc);

	// ETB wastegate position & PWM should both be set
	EXPECT_CALL(etb, setWastegatePosition(25.0f));
	EXPECT_CALL(pwm, setSimplePwmDutyCycle(0.25f));

	// Don't crash if not init'd (don't deref null ptr m_pwm)
	EXPECT_NO_THROW(bc.setOutput(25.0f));

	// Init with mock PWM device and ETB
	bc.init(&pwm, nullptr, nullptr, nullptr);
	engine->etbControllers[0] = &etb;

	bc.setOutput(25.0f);
}
