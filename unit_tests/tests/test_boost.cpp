#include "boost_control.h"
#include "engine_controller.h"
#include "engine_test_helper.h"

#include "mocks.h"

using ::testing::_;
using ::testing::NiceMock;

TEST(BoostControl, Setpoint) {
	MockVp3d targetMap;

	// Just pass TPS input to output
	EXPECT_CALL(targetMap, getValue(_, _))
		.WillRepeatedly([](float xRpm, float y) { return y; });

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

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

	engine->mockMapValue = NAN;
	// Check that invalid MAP returns unexpected
	EXPECT_EQ(bc.observePlant(), unexpected);

	// Test valid MAP value
	engine->mockMapValue = 150.0f;

	EXPECT_FLOAT_EQ(bc.observePlant().value_or(0), 150.0f);
}

TEST(BoostControl, OpenLoop) {
	MockVp3d openMap;

	EXPECT_CALL(openMap, getValue(_, _))
		.WillRepeatedly(Return(125.0f));

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	BoostController bc;
	INJECT_ENGINE_REFERENCE(&bc);

	// Without table set, should return unexpected
	EXPECT_EQ(bc.getOpenLoop(0), unexpected);

	bc.init(nullptr, &openMap, nullptr, nullptr);
}
