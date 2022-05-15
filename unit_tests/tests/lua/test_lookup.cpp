#include "pch.h"
#include "value_lookup.h"

TEST(LuaBasic, configLookup) {
	EngineTestHelper eth(FORD_ESCORT_GT);

	engineConfiguration->launchRpm = 100;
	ASSERT_EQ(100.0, getConfigValueByName("launchRpm"));

	engineConfiguration->maxAcTps = 40;
	ASSERT_EQ(40.0, getConfigValueByName("maxAcTps"));

}
