#include "pch.h"

#include "trigger_decoder.h"

TEST(trigger, testCustomSkipped) {
	EngineTestHelper eth(TEST_ENGINE);

	engineConfiguration->trigger.customTotalToothCount = 24;
	engineConfiguration->trigger.customSkippedToothCount = 2;
	eth.setTriggerType(TT_TOOTHED_WHEEL);

}
