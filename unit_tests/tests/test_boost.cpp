#include "boost_control.h"
#include "engine_test_helper.h"

#include "mocks.h"

using ::testing::_;
using ::testing::NiceMock;

TEST(BoostControl, Setpoint) {
	MockVp3d targetMap;

	EXPECT_CALL(targetMap, getValue(_, _))
		.WillRepeatedly(Return(125.0f));

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	BoostController bc;
	INJECT_ENGINE_REFERENCE(bc);

	// Should return unexpected without a pedal map cfg'd
	EXPECT_EQ(bc.getSetpoint(), unexpected);

	// Now init with mock target map
	bc.init(nullptr, nullptr, &targetMap, nullptr);

	EXPECT_FLOAT_EQ(bc.getSetpoint().value_or(-1), 125.0f);
}

TEST(BoostControl, ObservePlant) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	BoostController bc;
	INJECT_ENGINE_REFERENCE(bc);

	// Check that invalid MAP returns unexpected
	EXPECT_EQ(bc.observePlant(), unexpected);

	auto sensorCfg = engineConfiguration->map.sensor;
	sensorCfg.highValue = 300;
	sensorCfg.lowValue = 0;
	engineConfiguration->mapLowValueVoltage = 0;
	engineConfiguration->mapHighValueVoltage = 5;

	// Set the MAP, check that we get it back
	setMockMapVoltage(2.5f, PASS_ENGINE_PARAMETER_SUFFIX);
	EXPECT_FLOAT_EQ(bc.observePlant().value_or(0), 150.0f);
}

TEST(BoostControl, OpenLoop) {
	MockVp3d openMap;

	EXPECT_CALL(openMap, getValue(_, _))
		.WillRepeatedly(Return(125.0f));

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	BoostController bc;
	INJECT_ENGINE_REFERENCE(bc);

	// Without table set, should return unexpected
	EXPECT_EQ(bc.getOpenLoop(0), unexpected);

	bc.init(nullptr, openMap, nullptr, nullptr);
}
