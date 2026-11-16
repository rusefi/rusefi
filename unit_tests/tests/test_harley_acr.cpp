#include "pch.h"
#include "board_overrides.h"

#if EFI_HD_ACR
class HarleyAcrFirstTrigger : public ::testing::Test {
protected:
	decltype(custom_board_holdAcr) previousHold = custom_board_holdAcr;
	decltype(custom_board_getAcrOutputs) previousOutputs = custom_board_getAcrOutputs;
	decltype(custom_board_getAcrState) previousState = custom_board_getAcrState;
	decltype(custom_board_boardAllowTriggerActions) previousAllow = custom_board_boardAllowTriggerActions;
	void checkFirstTrigger(bool allowMovement);

	void TearDown() override {
		custom_board_holdAcr = previousHold;
		custom_board_getAcrOutputs = previousOutputs;
		custom_board_getAcrState = previousState;
		custom_board_boardAllowTriggerActions = previousAllow;
	}
};

void HarleyAcrFirstTrigger::checkFirstTrigger(bool allowMovement) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// Exercise the standard ACR strategy without a board's CAN pre-arm or outputs.
	custom_board_holdAcr = std::nullopt;
	custom_board_getAcrOutputs = std::nullopt;
	custom_board_getAcrState = std::nullopt;
	custom_board_boardAllowTriggerActions = allowMovement
		? +[] { return true; } : +[] { return false; };
	engineConfiguration->acrPin = Gpio::A0;
	engineConfiguration->acrPin2 = Gpio::A1;
	engineConfiguration->acrRevolutions = 4;
	engineConfiguration->triggerEventsTimeoutMs = 250;
	engineConfiguration->useNoiselessTriggerDecoder = false;
	engineConfiguration->trigger.customTotalToothCount = 32;
	engineConfiguration->trigger.customSkippedToothCount = 2;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL);
	auto* trigger = getTriggerCentral();
	auto& acr = engine->module<HarleyAcr>();

	// Repeat after another stop to verify the silence test uses the old timer.
	for (int attempt = 0; attempt < 2; attempt++) {
		advanceTimeUs(2'000'000);
		acr->onSlowCallback();
		ASSERT_FALSE(trigger->engineMovedRecently());
		ASSERT_FALSE(enginePins.harleyAcr.getLogicValue());
		ASSERT_FALSE(enginePins.harleyAcr2.getLogicValue());

		eth.firePrimaryTriggerRise();

		// No periodic callback between the first tooth and these assertions.
		EXPECT_EQ(allowMovement, trigger->engineMovedRecently());
		EXPECT_EQ(allowMovement, enginePins.harleyAcr.getLogicValue());
		EXPECT_EQ(allowMovement, enginePins.harleyAcr2.getLogicValue());
		EXPECT_EQ(allowMovement, engine->engineState.acrActive);
	}
}

TEST_F(HarleyAcrFirstTrigger, OpensImmediatelyOnFirstToothAfterStop) {
	checkFirstTrigger(true);
}

TEST_F(HarleyAcrFirstTrigger, DoesNotOpenWhenTriggerActionsAreBlocked) {
	checkFirstTrigger(false);
}
#endif
