/**
 * @file test_config_error_refresh.cpp
 *
 * Level-triggered config-error evaluation via the custom_board_updateConfigError
 * hook: priority within the hook's if/else chain, auto-clear, and non-interference
 * with legacy direct configError() callers.
 * See refreshConfigErrorState() in error_handling.h and board_overrides.h.
 */

#include "pch.h"

#include "board_overrides.h"
#include "flash_main.h"

namespace {

bool firstActive = false;
bool secondActive = false;

// mirrors the fw-iws updateConfigError() shape: worst condition first
bool testBoardUpdateConfigError() {
	if (firstActive) {
		configError("first problem");
		return true;
	}
	if (secondActive) {
		configError("second problem");
		return true;
	}
	return false;
}

class ConfigErrorRefresh : public ::testing::Test {
protected:
	void SetUp() override {
		resetConfigErrorStateForUnitTest();
		firstActive = false;
		secondActive = false;
		custom_board_updateConfigError = testBoardUpdateConfigError;
	}

	void TearDown() override {
		resetConfigErrorStateForUnitTest();
	}
};

} // namespace

TEST_F(ConfigErrorRefresh, priorityMaskingAndAutoClear) {
	// both conditions hold - worst (first in the chain) wins
	firstActive = true;
	secondActive = true;
	refreshConfigErrorState();
	ASSERT_TRUE(hasConfigError());
	EXPECT_STREQ("first problem", getConfigErrorMessage());

	// worst condition clears - next one surfaces on the following refresh
	firstActive = false;
	refreshConfigErrorState();
	ASSERT_TRUE(hasConfigError());
	EXPECT_STREQ("second problem", getConfigErrorMessage());

	// no condition holds - message cleared without any producer calling clear
	secondActive = false;
	refreshConfigErrorState();
	EXPECT_FALSE(hasConfigError());
}

TEST_F(ConfigErrorRefresh, reRaisesAfterConditionReturns) {
	firstActive = true;
	refreshConfigErrorState();
	ASSERT_TRUE(hasConfigError());

	firstActive = false;
	refreshConfigErrorState();
	ASSERT_FALSE(hasConfigError());

	// unlike an edge-triggered "reported" latch, a returning condition is reported again
	firstActive = true;
	refreshConfigErrorState();
	ASSERT_TRUE(hasConfigError());
	EXPECT_STREQ("first problem", getConfigErrorMessage());
}

TEST_F(ConfigErrorRefresh, legacyDirectCallStaysLatched) {
	configError("legacy latched message");
	// no hook condition holds, but this evaluation did not raise the message
	refreshConfigErrorState();
	refreshConfigErrorState();
	ASSERT_TRUE(hasConfigError());
	EXPECT_STREQ("legacy latched message", getConfigErrorMessage());
}

TEST_F(ConfigErrorRefresh, legacyCallAfterConditionStopsIsNotCleared) {
	firstActive = true;
	refreshConfigErrorState();
	ASSERT_TRUE(hasConfigError());

	// a legacy caller overwrites the message between the condition going inactive
	// and the next refresh - that message is theirs to keep
	firstActive = false;
	configError("legacy raised in between");
	refreshConfigErrorState();
	ASSERT_TRUE(hasConfigError());
	EXPECT_STREQ("legacy raised in between", getConfigErrorMessage());
}

TEST_F(ConfigErrorRefresh, invokedFromPeriodicSlowCallback) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// EngineTestHelper construction resets the hook, so install after
	custom_board_updateConfigError = testBoardUpdateConfigError;

	firstActive = true;
	engine->periodicSlowCallback();
	ASSERT_TRUE(hasConfigError());
	EXPECT_STREQ("first problem", getConfigErrorMessage());

	firstActive = false;
	engine->periodicSlowCallback();
	EXPECT_FALSE(hasConfigError());
}

// core producer: settings writes failing continuously (see flash_main.cpp)

TEST_F(ConfigErrorRefresh, settingsWriteFailureRaisesAfterThresholdAndSelfClears) {
	// first failed attempt starts the clock - not reported yet
	trackSettingsWriteResult(false);
	refreshConfigErrorState();
	EXPECT_FALSE(hasConfigError());

	// still failing past the threshold - reported
	advanceTimeUs(MS2US(11'000));
	trackSettingsWriteResult(false);
	refreshConfigErrorState();
	ASSERT_TRUE(hasConfigError());
	EXPECT_STREQ("Settings write keeps failing - your changes are NOT saved to flash", getConfigErrorMessage());

	// a later retry succeeds - condition goes away, message clears without any producer calling clear
	trackSettingsWriteResult(true);
	refreshConfigErrorState();
	EXPECT_FALSE(hasConfigError());
}

TEST_F(ConfigErrorRefresh, settingsWriteTransientFailureStaysQuiet) {
	trackSettingsWriteResult(false);
	advanceTimeUs(MS2US(5'000));
	// recovered below the threshold - never reported
	trackSettingsWriteResult(true);
	advanceTimeUs(MS2US(20'000));
	refreshConfigErrorState();
	EXPECT_FALSE(hasConfigError());
}

TEST_F(ConfigErrorRefresh, settingsWriteFailureOutranksBoardHook) {
	secondActive = true;
	trackSettingsWriteResult(false);
	advanceTimeUs(MS2US(11'000));
	refreshConfigErrorState();

	// core producer wins over the board hook
	ASSERT_TRUE(hasConfigError());
	EXPECT_STREQ("Settings write keeps failing - your changes are NOT saved to flash", getConfigErrorMessage());

	// once settings writes recover, the board condition surfaces
	trackSettingsWriteResult(true);
	refreshConfigErrorState();
	ASSERT_TRUE(hasConfigError());
	EXPECT_STREQ("second problem", getConfigErrorMessage());
}
