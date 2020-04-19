#include "closed_loop_controller.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::StrictMock;
using ::testing::Return;
using ::testing::Eq;

class TestController : public ClosedLoopController<float, float> {
public:
	MOCK_METHOD(expected<float>, getSetpoint, (), (const, override));
	MOCK_METHOD(expected<float>, observePlant, (), (const, override));
	MOCK_METHOD(expected<float>, getOpenLoop, (float setpoint), (const, override));
	MOCK_METHOD(expected<float>, getClosedLoop, (float setpoint, float observation), (override));
	MOCK_METHOD(void, setOutput, (expected<float> outputValue), (override));
};

TEST(ClosedLoopController, TestSetpoint) {
	StrictMock<TestController> controller;

	// If getSetpoint returns unexpected, no other functions should be called
	EXPECT_CALL(controller, getSetpoint())
		.WillOnce(Return(unexpected));

	// And output should be called with unexpected
	EXPECT_CALL(controller, setOutput(Eq(unexpected)));

	controller.update();
}

TEST(ClosedLoopController, TestSetpointSuccessPlantFail) {
	StrictMock<TestController> controller;

	// If getSetpoint returns unexpected, no other functions should be called
	EXPECT_CALL(controller, getSetpoint())
		.WillOnce(Return(25.0f));

	// Fail to observe plant - exec should stop there
	EXPECT_CALL(controller, observePlant())
		.WillOnce(Return(unexpected));

	// And output should be called with unexpected
	EXPECT_CALL(controller, setOutput(Eq(unexpected)));

	controller.update();
}

TEST(ClosedLoopController, TestPlantSuccessOpenLoopFail) {
	StrictMock<TestController> controller;

	// Return valid setpoint
	EXPECT_CALL(controller, getSetpoint())
		.WillOnce(Return(25.0f));

	// Return valid observation
	EXPECT_CALL(controller, observePlant())
		.WillOnce(Return(75.0f));

	// Setpoint should get passed to open loop
	// It will fail, nothing else should be called
	EXPECT_CALL(controller, getOpenLoop(25.0f))
		.WillOnce(Return(unexpected));

	// And output should be called with unexpected
	EXPECT_CALL(controller, setOutput(Eq(unexpected)));

	controller.update();
}


TEST(ClosedLoopController, TestPlantOpenLoopSuccessClosedLoopFail) {
	StrictMock<TestController> controller;

	// Return valid setpoint
	EXPECT_CALL(controller, getSetpoint())
		.WillOnce(Return(25.0f));

	// Return valid observation
	EXPECT_CALL(controller, observePlant())
		.WillOnce(Return(75.0f));

	// Setpoint should get passed to open loop
	// Return something valid
	EXPECT_CALL(controller, getOpenLoop(25.0f))
		.WillOnce(Return(37.0f));

	// Setpoint & observation should get passed
	// But return invalid
	EXPECT_CALL(controller, getClosedLoop(25.0f, 75.0f))
		.WillOnce(Return(unexpected));

	// And output should be called with unexpected
	EXPECT_CALL(controller, setOutput(Eq(unexpected)));

	controller.update();
}

TEST(ClosedLoopController, TestAllSuccess) {
	StrictMock<TestController> controller;

	// Return valid setpoint
	EXPECT_CALL(controller, getSetpoint())
		.WillOnce(Return(25.0f));

	// Return valid observation
	EXPECT_CALL(controller, observePlant())
		.WillOnce(Return(75.0f));

	// Setpoint should get passed to open loop
	// Return something valid
	EXPECT_CALL(controller, getOpenLoop(25.0f))
		.WillOnce(Return(37.0f));

	// Setpoint & observation should get passed
	// Return something valid
	EXPECT_CALL(controller, getClosedLoop(25.0f, 75.0f))
		.WillOnce(Return(22.0f));

	// Output should get called with sum of open & closed loop
	EXPECT_CALL(controller, setOutput(expected<float>(59.0f)));

	controller.update();
}
