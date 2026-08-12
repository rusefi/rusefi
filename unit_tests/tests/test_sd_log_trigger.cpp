/*
 * @file test_sd_log_trigger.cpp
 *
 * Conditional SD logging trigger (phase 1) state machine tests.
 */

#include "pch.h"
#include "sd_log_trigger.h"

TEST(sdLog, unconditionalWhenDisabled) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->sdCardConditionalLogging = false;

	auto m = engine->module<SdLogTrigger>();
	m->onSlowCallback();

	EXPECT_TRUE(m->shouldLog());
	EXPECT_EQ(SD_LOG_UNCONDITIONAL, m->getState());
}

TEST(sdLog, rpmHysteresisAndStopDelay) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->sdCardConditionalLogging = true;
	engineConfiguration->sdLogStartRpm = 4000;
	engineConfiguration->sdLogStopRpm = 3000;
	engineConfiguration->sdLogStopDelay = 2;

	auto m = engine->module<SdLogTrigger>();

	// below start -> not logging
	Sensor::setMockValue(SensorType::Rpm, 1000);
	m->onSlowCallback();
	EXPECT_FALSE(m->shouldLog());
	EXPECT_EQ(SD_LOG_WAIT_RPM, m->getState());

	// at/above start -> logging
	Sensor::setMockValue(SensorType::Rpm, 5000);
	m->onSlowCallback();
	EXPECT_TRUE(m->shouldLog());
	EXPECT_EQ(SD_LOG_ACTIVE, m->getState());

	// dead zone (stop <= rpm < start) -> keep logging (hysteresis)
	Sensor::setMockValue(SensorType::Rpm, 3500);
	m->onSlowCallback();
	EXPECT_TRUE(m->shouldLog());
	EXPECT_EQ(SD_LOG_ACTIVE, m->getState());

	// below stop but within the delay -> still logging
	Sensor::setMockValue(SensorType::Rpm, 1000);
	m->onSlowCallback();
	EXPECT_TRUE(m->shouldLog());
	EXPECT_EQ(SD_LOG_STOP_DELAY, m->getState());

	// still below stop after the delay -> stop logging
	eth.moveTimeForwardSec(3);
	m->onSlowCallback();
	EXPECT_FALSE(m->shouldLog());
	EXPECT_EQ(SD_LOG_WAIT_RPM, m->getState());
}

TEST(sdLog, secondaryConditionGatesEntry) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->sdCardConditionalLogging = true;
	engineConfiguration->sdLogStartRpm = 4000;
	engineConfiguration->sdLogStopRpm = 3000;
	engineConfiguration->sdLogMinTps = 50;

	auto m = engine->module<SdLogTrigger>();

	// RPM ok but TPS below the minimum -> does not start
	Sensor::setMockValue(SensorType::Rpm, 5000);
	Sensor::setMockValue(SensorType::Tps1, 10);
	m->onSlowCallback();
	EXPECT_FALSE(m->shouldLog());
	EXPECT_EQ(SD_LOG_WAIT_COND, m->getState());

	// TPS now above the minimum -> starts
	Sensor::setMockValue(SensorType::Tps1, 80);
	m->onSlowCallback();
	EXPECT_TRUE(m->shouldLog());
	EXPECT_EQ(SD_LOG_ACTIVE, m->getState());

	// secondary conditions gate ENTRY only: TPS dropping does not stop logging while RPM stays up
	Sensor::setMockValue(SensorType::Tps1, 0);
	m->onSlowCallback();
	EXPECT_TRUE(m->shouldLog());
	EXPECT_EQ(SD_LOG_ACTIVE, m->getState());
}
