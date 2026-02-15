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

TEST(trigger, twoStrokeSingleToothWith30DegOffset_sparkOutOfOrder) {
	EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
	engineConfiguration->twoStroke = true;
	engineConfiguration->globalTriggerAngleOffset = 30;

	engineConfiguration->trigger.customTotalToothCount = 1;
	engineConfiguration->trigger.customSkippedToothCount = 0;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL);

	// initial spin-up to get RPM sync
	eth.smartFireTriggerEvents2(/*count*/20, /*delay*/ 40);
	ASSERT_EQ(750, Sensor::getOrZero(SensorType::Rpm));

	// the bug doesn't appear immediately - need more revolutions for it to manifest
	eth.smartFireTriggerEvents2(/*count*/200, /*delay*/ 40);

	// continue running to accumulate more out-of-order events
	eth.smartFireTriggerEvents2(/*count*/200, /*delay*/ 40);

	EXPECT_NE(0, engine->engineState.sparkOutOfOrderCounter) << "demonstrates sparkOutOfOrder bug with single tooth + 30deg offset";
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
