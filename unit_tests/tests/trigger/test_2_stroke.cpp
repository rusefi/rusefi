#include "pch.h"

TEST(trigger, twoStrokeSingleToothAsSkippedWheel) {
	EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
	engineConfiguration->twoStroke = true;

    engineConfiguration->trigger.customTotalToothCount = 1;
    engineConfiguration->trigger.customSkippedToothCount = 0;
    eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL);

    eth.smartFireTriggerEvents2(/*count*/20, /*delay*/ 40);
	ASSERT_EQ(750, Sensor::getOrZero(SensorType::Rpm));
}

TEST(trigger, twoStrokeSingleToothTrigger) {
	EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
	engineConfiguration->twoStroke = true;

    eth.setTriggerType(trigger_type_e::TT_HALF_MOON);

	eth.smartFireTriggerEvents2(/*count*/20, /*delay*/ 40);
	ASSERT_EQ(750, Sensor::getOrZero(SensorType::Rpm));
}

TEST(trigger, twoStrokeSingleToothWith72DegOffset) {
	EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
	engineConfiguration->twoStroke = true;
	engineConfiguration->globalTriggerAngleOffset = 72;

	engineConfiguration->trigger.customTotalToothCount = 1;
	engineConfiguration->trigger.customSkippedToothCount = 0;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL);

	eth.smartFireTriggerEvents2(/*count*/20, /*delay*/ 40);

	ASSERT_EQ(750, Sensor::getOrZero(SensorType::Rpm));
}
