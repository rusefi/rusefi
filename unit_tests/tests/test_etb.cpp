/*
 * @file test_etb.cpp
 *
 * @date Dec 13, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine_test_helper.h"
#include "electronic_throttle.h"
#include "dc_motor.h"
#include "engine_controller.h"
#include "sensor.h"

using ::testing::_;
using ::testing::StrictMock;

class MockEtb : public IEtbController {
public:
	// PeriodicTimerController mocks
	MOCK_METHOD(void, PeriodicTask, (), (override));
	MOCK_METHOD(int, getPeriodMs, (), (override));

	// IEtbController mocks
	MOCK_METHOD(void, reset, (), ());
	MOCK_METHOD(void, Start, (), (override));
	MOCK_METHOD(void, init, (DcMotor* motor, int ownIndex, pid_s* pidParameters));

	// ClosedLoopController mocks
	MOCK_METHOD(expected<percent_t>, getSetpoint, (), (const, override));
	MOCK_METHOD(expected<percent_t>, observePlant, (), (const, override));
	MOCK_METHOD(expected<percent_t>, getOpenLoop, (percent_t setpoint), (const, override));
	MOCK_METHOD(expected<percent_t>, getClosedLoop, (percent_t setpoint, percent_t observation), (override));
	MOCK_METHOD(void, setOutput, (expected<percent_t> outputValue), (override));
};

class MockMotor : public DcMotor {
public:
	MOCK_METHOD(bool, set, (float duty), (override));
	MOCK_METHOD(float, get, (), (const, override));
	MOCK_METHOD(void, enable, (), (override));
	MOCK_METHOD(void, disable, (), (override));
	MOCK_METHOD(bool, isOpenDirection, (), (const, override));
};

TEST(etb, initializationNoPedal) {
	StrictMock<MockEtb> mocks[ETB_COUNT];

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	for (int i = 0; i < ETB_COUNT; i++) {
		engine->etbControllers[i] = &mocks[i];
	}

	// We expect no throttle init stuff to be called - lack of pedal should disable ETB

	doInitElectronicThrottle(PASS_ENGINE_PARAMETER_SIGNATURE);
}

TEST(etb, initializationSingleThrottle) {

	StrictMock<MockEtb> mocks[ETB_COUNT];

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	for (int i = 0; i < ETB_COUNT; i++) {
		engine->etbControllers[i] = &mocks[i];
	}

	// Must have a sensor configured before init
	Sensor::setMockValue(SensorType::AcceleratorPedal, 0);

	// Expect mock0 to be init with index 0, and PID params
	EXPECT_CALL(mocks[0], init(_, 0, &engineConfiguration->etb));
	EXPECT_CALL(mocks[0], reset);
	EXPECT_CALL(mocks[0], Start);

	// We do not expect throttle #2 to be initialized

	doInitElectronicThrottle(PASS_ENGINE_PARAMETER_SIGNATURE);
}

TEST(etb, initializationDualThrottle) {
	StrictMock<MockEtb> mocks[ETB_COUNT];

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	for (int i = 0; i < ETB_COUNT; i++) {
		engine->etbControllers[i] = &mocks[i];
	}

	// Must have a sensor configured before init
	Sensor::setMockValue(SensorType::AcceleratorPedal, 0);

	// The presence of a second TPS indicates dual throttle
	Sensor::setMockValue(SensorType::Tps2, 25.0f);

	// Expect mock0 to be init with index 0, and PID params
	EXPECT_CALL(mocks[0], init(_, 0, &engineConfiguration->etb));
	EXPECT_CALL(mocks[0], reset);
	EXPECT_CALL(mocks[0], Start);

	// Expect mock1 to be init with index 2, and PID params
	EXPECT_CALL(mocks[1], init(_, 1, &engineConfiguration->etb));
	EXPECT_CALL(mocks[1], reset);
	EXPECT_CALL(mocks[1], Start);

	doInitElectronicThrottle(PASS_ENGINE_PARAMETER_SIGNATURE);
}

TEST(etb, testSetpointOnlyPedal) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	// Don't use ETB for idle, we aren't testing that yet - just pedal table for now
	engineConfiguration->useETBforIdleControl = false;

	// Must have a sensor configured before init
	Sensor::setMockValue(SensorType::AcceleratorPedal, 0);
	Sensor::setMockValue(SensorType::Tps1, 0);

	EtbController etb;
	engine->etbControllers[0] = &etb;
	doInitElectronicThrottle(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Check endpoints and midpoint
	Sensor::setMockValue(SensorType::AcceleratorPedal, 0.0f);
	EXPECT_EQ(0, etb.getSetpoint().value_or(-1));
	Sensor::setMockValue(SensorType::AcceleratorPedal, 50.0f);
	EXPECT_EQ(50, etb.getSetpoint().value_or(-1));
	Sensor::setMockValue(SensorType::AcceleratorPedal, 100.0f);
	EXPECT_EQ(100, etb.getSetpoint().value_or(-1));

	// Test some floating point pedal/output values
	Sensor::setMockValue(SensorType::AcceleratorPedal, 50.8302f);
	EXPECT_NEAR(50.8302, etb.getSetpoint().value_or(-1), EPS4D);
	Sensor::setMockValue(SensorType::AcceleratorPedal, 51.6605f);
	EXPECT_NEAR(51.6605, etb.getSetpoint().value_or(-1), EPS4D);

	// Valid but out of range - should clamp to [0, 100]
	Sensor::setMockValue(SensorType::AcceleratorPedal, -5);
	EXPECT_EQ(0, etb.getSetpoint().value_or(-1));
	Sensor::setMockValue(SensorType::AcceleratorPedal, 105);
	EXPECT_EQ(100, etb.getSetpoint().value_or(-1));

	// Test invalid pedal position - should give unexpected
	Sensor::resetMockValue(SensorType::AcceleratorPedal);
	EXPECT_EQ(etb.getSetpoint(), unexpected);
}

TEST(etb, etbTpsSensor) {
	// Throw some distinct values on the TPS sensors so we can identify that we're getting the correct one
	Sensor::setMockValue(SensorType::Tps1, 25.0f);
	Sensor::setMockValue(SensorType::Tps2, 75.0f);

	// Test first throttle
	{
		EtbController etb;
		etb.init(nullptr, 0, nullptr);
		EXPECT_EQ(etb.observePlant().Value, 25.0f);
	}

	// Test second throttle
	{
		EtbController etb;
		etb.init(nullptr, 1, nullptr);
		EXPECT_EQ(etb.observePlant().Value, 75.0f);
	}
}

TEST(etb, setOutputInvalid) {
	StrictMock<MockMotor> motor;

	EtbController etb;
	etb.init(&motor, 0, nullptr);

	// Should be disabled in case of unexpected
	EXPECT_CALL(motor, disable());

	etb.setOutput(unexpected);
}

TEST(etb, setOutputValid) {
	StrictMock<MockMotor> motor;

	EtbController etb;
	etb.init(&motor, 0, nullptr);

	// Should be enabled and value set
	EXPECT_CALL(motor, enable());
	EXPECT_CALL(motor, set(0.25f))
		.WillOnce(Return(false));

	etb.setOutput(25.0f);
}

TEST(etb, setOutputValid2) {
	StrictMock<MockMotor> motor;

	EtbController etb;
	etb.init(&motor, 0, nullptr);

	// Should be enabled and value set
	EXPECT_CALL(motor, enable());
	EXPECT_CALL(motor, set(-0.25f))
		.WillOnce(Return(false));

	etb.setOutput(-25.0f);
}

TEST(etb, setOutputOutOfRangeHigh) {
	StrictMock<MockMotor> motor;

	EtbController etb;
	etb.init(&motor, 0, nullptr);

	// Should be enabled and value set
	EXPECT_CALL(motor, enable());
	EXPECT_CALL(motor, set(0.90f));

	// Off scale - should get clamped to 90%
	etb.setOutput(110);
}

TEST(etb, setOutputOutOfRangeLow) {
	StrictMock<MockMotor> motor;

	EtbController etb;
	etb.init(&motor, 0, nullptr);

	// Should be enabled and value set
	EXPECT_CALL(motor, enable());
	EXPECT_CALL(motor, set(-0.90f));

	// Off scale - should get clamped to -90%
	etb.setOutput(-110);
}
