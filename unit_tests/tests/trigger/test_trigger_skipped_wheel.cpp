#include "pch.h"

#include "trigger_decoder.h"

TEST(trigger, testCustomSkipped) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->trigger.customTotalToothCount = 24;
	engineConfiguration->trigger.customSkippedToothCount = 2;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL);

}
