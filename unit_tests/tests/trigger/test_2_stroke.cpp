#include "pch.h"

TEST(trigger, twoStrokeSingleToothAsSkippedWheel) {
	EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
	setTwoStrokeOperationMode();

    engineConfiguration->trigger.customTotalToothCount = 1;
    engineConfiguration->trigger.customSkippedToothCount = 0;
    eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL);

    EXPECT_FATAL_ERROR(eth.smartFireTriggerEvents2(/*count*/20, /*delay*/ 40));
	// todo ASSERT_EQ(750, Sensor::getOrZero(SensorType::Rpm));
}


TEST(trigger, twoStrokeSingleToothTrigger) {
	EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
	setTwoStrokeOperationMode();

    eth.setTriggerType(trigger_type_e::TT_ONE);

	eth.smartFireTriggerEvents2(/*count*/20, /*delay*/ 40);
	ASSERT_EQ(750, Sensor::getOrZero(SensorType::Rpm));
}
