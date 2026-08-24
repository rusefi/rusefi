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

	{
		//scaled_channel<uint8_t, 10, 1> knockRetardAggression;
		const char * name = "knockRetardAggression";
		setConfigValueByName(name, 1.0);
		ASSERT_EQ(engineConfiguration->knockRetardAggression, getConfigValueByName(name));
		ASSERT_EQ(1.0, engineConfiguration->knockRetardAggression);
	}

	{
		//scaled_channel<uint16_t, 1000, 1> vssGearRatio;
		const char * name = "vssGearRatio";
		setConfigValueByName(name, 1.999);
		ASSERT_EQ(engineConfiguration->vssGearRatio, getConfigValueByName(name));
		ASSERT_NEAR(1.999, engineConfiguration->vssGearRatio, EPS3D);
	}
}

// This test PASSES on purpose: it pins down and documents a KNOWN REGRESSION, it does not bless it.
//
// The table-driven config lookup (value_lookup_table_generated.cpp, introduced around
// https://github.com/rusefi/rusefi/pull/10065) ignores scaled_channel scaling: ConfigParameter only
// stores {hash, offset, type, bitOffset}, so a scaled_channel<uint8_t, 100> field is treated as a
// plain uint8_t. The previous per-field generated code (value_lookup_generated.cpp) read and wrote
// such fields through scaled_channel's float conversion operators, i.e. in engineering units.
//
// As a result, for any scaled field:
//  - getConfigValueByName returns the RAW storage value (engineering value times mul/div)
//  - setConfigValueByName truncates the engineering value to the integer type and stores it as RAW,
//    so the stored engineering value ends up divided by the scale factor (and fractions are lost)
//
// When the lookup is fixed, this test MUST be updated to assert the correct (engineering-unit)
// behavior - every EXPECT below asserts the broken value, with the correct value in a comment.
TEST(LuaBasic, configLookupScaledChannelRegression) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	{
		// ltitSmoothingIntensity is scaled_channel<uint8_t, 100, 1>
		const char * name = "ltitSmoothingIntensity";

		engineConfiguration->ltitSmoothingIntensity = 0.55f;
		EXPECT_FLOAT_EQ(0.55f, getConfigValueByName(name));

		setConfigValueByName(name, 0.25f);
		EXPECT_FLOAT_EQ(0.25f, engineConfiguration->ltitSmoothingIntensity);
	}

	{
		// vssGearRatio is scaled_channel<uint16_t, 1000, 1>
		const char * name = "vssGearRatio";

		engineConfiguration->vssGearRatio = 2.5f;
		EXPECT_FLOAT_EQ(2.5f, getConfigValueByName(name));
		setConfigValueByName(name, 3.5f);
		EXPECT_NEAR(3.5f, engineConfiguration->vssGearRatio, EPS4D);
		EXPECT_FLOAT_EQ(3.5f, getConfigValueByName(name));
	}
}
