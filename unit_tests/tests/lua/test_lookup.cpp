#include "pch.h"
#include "value_lookup.h"

TEST(LuaBasic, configLookup) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	{
		const char * name = "ignitionDwellForCrankingMs";
		setConfigValueByName(name, 1.6);
		ASSERT_NEAR(1.6, getConfigValueByName(name), EPS3D);
	}

	{
		const char * name = "launchRpm";
		engineConfiguration->launchRpm = 100;
		ASSERT_EQ(100.0, getConfigValueByName(name));
		setConfigValueByName(name, 160);
		ASSERT_EQ(160.0, getConfigValueByName(name));
	}

	{
		const char * name = "maxAcTps";
		engineConfiguration->maxAcTps = 40;
		ASSERT_EQ(40.0, getConfigValueByName(name));
		setConfigValueByName(name, 103);
		ASSERT_EQ(103.0, getConfigValueByName(name));
	}

	{
		const char * name = "multisparkMaxSparkingAngle";
		setConfigValueByName(name, 13);
		ASSERT_EQ(13.0, getConfigValueByName(name));
	}

	{
		const char * bit0Name = "devBit0";
		const char * bit1Name = "devBit1";
		setConfigValueByName(bit0Name, 1);
		setConfigValueByName(bit1Name, 0);
		ASSERT_EQ(1.0, getConfigValueByName(bit0Name));
		ASSERT_EQ(0.0, getConfigValueByName(bit1Name));

		setConfigValueByName(bit0Name, 0);
		setConfigValueByName(bit1Name, 1);
		ASSERT_EQ(0.0, getConfigValueByName(bit0Name));
		ASSERT_EQ(1.0, getConfigValueByName(bit1Name));
	}
}
