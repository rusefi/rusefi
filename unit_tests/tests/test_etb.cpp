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

#include "mocks.h"

using ::testing::_;
using ::testing::Ne;
using ::testing::StrictMock;

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
	EXPECT_CALL(mocks[0], init(_, 0, &engineConfiguration->etb, Ne(nullptr)));
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
	EXPECT_CALL(mocks[0], init(_, 0, &engineConfiguration->etb, Ne(nullptr)));
	EXPECT_CALL(mocks[0], reset);
	EXPECT_CALL(mocks[0], Start);

	// Expect mock1 to be init with index 2, and PID params
	EXPECT_CALL(mocks[1], init(_, 1, &engineConfiguration->etb, Ne(nullptr)));
	EXPECT_CALL(mocks[1], reset);
	EXPECT_CALL(mocks[1], Start);

	doInitElectronicThrottle(PASS_ENGINE_PARAMETER_SIGNATURE);
}

TEST(etb, idlePlumbing) {
	StrictMock<MockEtb> mocks[ETB_COUNT];

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	for (int i = 0; i < ETB_COUNT; i++) {
		engine->etbControllers[i] = &mocks[i];

		EXPECT_CALL(mocks[i], setIdlePosition(33.0f));
	}

	setEtbIdlePosition(33.0f PASS_ENGINE_PARAMETER_SUFFIX);
}

TEST(etb, testSetpointOnlyPedal) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	// Don't use ETB for idle, we aren't testing that yet - just pedal table for now
	engineConfiguration->useETBforIdleControl = false;

	EtbController etb;
	INJECT_ENGINE_REFERENCE(&etb);

	// Mock pedal map that's just passthru pedal -> target
	StrictMock<MockVp3d> pedalMap;
	EXPECT_CALL(pedalMap, getValue(_, _))
		.WillRepeatedly([](float xRpm, float y) {
			return y;
		});

	// Uninitialized ETB must return unexpected (and not deference a null pointer)
	EXPECT_EQ(etb.getSetpoint(), unexpected);

	etb.init(nullptr, 0, nullptr, &pedalMap);

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

TEST(etb, setpointIdle) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	// Use ETB for idle, but don't give it any range (yet)
	engineConfiguration->useETBforIdleControl = true;
	engineConfiguration->etbIdleThrottleRange = 0;

	EtbController etb;
	INJECT_ENGINE_REFERENCE(&etb);

	// Mock pedal map that's just passthru pedal -> target
	StrictMock<MockVp3d> pedalMap;
	EXPECT_CALL(pedalMap, getValue(_, _))
		.WillRepeatedly([](float xRpm, float y) {
			return y;
		});
	etb.init(nullptr, 0, nullptr, &pedalMap);

	// No idle range, should just pass pedal
	Sensor::setMockValue(SensorType::AcceleratorPedal, 0.0f);
	EXPECT_EQ(0, etb.getSetpoint().value_or(-1));
	Sensor::setMockValue(SensorType::AcceleratorPedal, 50.0f);
	EXPECT_EQ(50, etb.getSetpoint().value_or(-1));

	// Idle should now have 10% range
	engineConfiguration->etbIdleThrottleRange = 10;

	// 50% idle position should increase setpoint by 5%
	etb.setIdlePosition(50);
	Sensor::setMockValue(SensorType::AcceleratorPedal, 0.0f);
	EXPECT_FLOAT_EQ(5, etb.getSetpoint().value_or(-1));
	Sensor::setMockValue(SensorType::AcceleratorPedal, 50.0f);
	EXPECT_FLOAT_EQ(55, etb.getSetpoint().value_or(-1));

	// 100% setpoint should increase by 10%
	etb.setIdlePosition(100);
	Sensor::setMockValue(SensorType::AcceleratorPedal, 0.0f);
	EXPECT_FLOAT_EQ(10, etb.getSetpoint().value_or(-1));
	Sensor::setMockValue(SensorType::AcceleratorPedal, 50.0f);
	EXPECT_FLOAT_EQ(60, etb.getSetpoint().value_or(-1));

	// 125% setpoint should clamp to 10% increase
	etb.setIdlePosition(125);
	Sensor::setMockValue(SensorType::AcceleratorPedal, 0.0f);
	EXPECT_FLOAT_EQ(10, etb.getSetpoint().value_or(-1));
	Sensor::setMockValue(SensorType::AcceleratorPedal, 50.0f);
	EXPECT_FLOAT_EQ(60, etb.getSetpoint().value_or(-1));
}

TEST(etb, etbTpsSensor) {
	// Throw some distinct values on the TPS sensors so we can identify that we're getting the correct one
	Sensor::setMockValue(SensorType::Tps1, 25.0f);
	Sensor::setMockValue(SensorType::Tps2, 75.0f);

	// Test first throttle
	{
		EtbController etb;
		etb.init(nullptr, 0, nullptr, nullptr);
		EXPECT_EQ(etb.observePlant().Value, 25.0f);
	}

	// Test second throttle
	{
		EtbController etb;
		etb.init(nullptr, 1, nullptr, nullptr);
		EXPECT_EQ(etb.observePlant().Value, 75.0f);
	}
}

TEST(etb, setOutputInvalid) {
	StrictMock<MockMotor> motor;

	EtbController etb;
	etb.init(&motor, 0, nullptr, nullptr);

	// Should be disabled in case of unexpected
	EXPECT_CALL(motor, disable());

	etb.setOutput(unexpected);
}

TEST(etb, setOutputValid) {
	StrictMock<MockMotor> motor;

	EtbController etb;
	etb.init(&motor, 0, nullptr, nullptr);

	// Should be enabled and value set
	EXPECT_CALL(motor, enable());
	EXPECT_CALL(motor, set(0.25f))
		.WillOnce(Return(false));

	etb.setOutput(25.0f);
}

TEST(etb, setOutputValid2) {
	StrictMock<MockMotor> motor;

	EtbController etb;

	etb.init(&motor, 0, nullptr, nullptr);

	// Should be enabled and value set
	EXPECT_CALL(motor, enable());
	EXPECT_CALL(motor, set(-0.25f))
		.WillOnce(Return(false));

	etb.setOutput(-25.0f);
}

TEST(etb, setOutputOutOfRangeHigh) {
	StrictMock<MockMotor> motor;

	EtbController etb;
	etb.init(&motor, 0, nullptr, nullptr);

	// Should be enabled and value set
	EXPECT_CALL(motor, enable());
	EXPECT_CALL(motor, set(0.90f));

	// Off scale - should get clamped to 90%
	etb.setOutput(110);
}

TEST(etb, setOutputOutOfRangeLow) {
	StrictMock<MockMotor> motor;

	EtbController etb;
	etb.init(&motor, 0, nullptr, nullptr);

	// Should be enabled and value set
	EXPECT_CALL(motor, enable());
	EXPECT_CALL(motor, set(-0.90f));

	// Off scale - should get clamped to -90%
	etb.setOutput(-110);
}
