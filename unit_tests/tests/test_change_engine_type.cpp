/*
 * @file test_change_engine_type.cpp
 *
 * @date Jul 14, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

extern engine_configuration_s & activeConfiguration;

TEST(EngineType, MiataNcPreset) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// The simulator exports this preset at startup without a supported hardware board.
	ASSERT_NO_THROW(resetConfigurationExt(engine_type_e::MAZDA_MIATA_NC));
	EXPECT_EQ(engine_type_e::MAZDA_MIATA_NC, engineConfiguration->engineType);
	EXPECT_STREQ("Miata NC", engineConfiguration->vehicleName);
	EXPECT_STREQ("LF-VE", engineConfiguration->engineCode);
	EXPECT_FLOAT_EQ(2.0f, engineConfiguration->displacement);
	EXPECT_EQ(trigger_type_e::TT_36_2_2_2, engineConfiguration->trigger.type);
	EXPECT_EQ(VVT_MAZDA_L, engineConfiguration->vvtMode[0]);
}

TEST(misc, changeEngineType) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE, [](engine_configuration_s* cfg) {
		// Exercise HPFP pin shutdown without depending on a preset's other hardware requirements.
		cfg->hpfpValvePin = Gpio::A2;
	});

	brain_pin_e brainPin = engineConfiguration->hpfpValvePin;
	ASSERT_TRUE(brainPin != Gpio::Unassigned);
	ASSERT_TRUE(activeConfiguration.hpfpValvePin != Gpio::Unassigned);

	ASSERT_TRUE(isBrainPinValid(brainPin));

	// above we have asserted that hpfpValvePin is in fact used
	// now let's change into engine type without hpfpValvePin and assert shut down

	setEngineType((int)engine_type_e::DODGE_NEON_1995);
	ASSERT_TRUE(activeConfiguration.hpfpValvePin == Gpio::Unassigned);

	ASSERT_TRUE(engineConfiguration->hpfpValvePin == Gpio::Unassigned);

	ASSERT_FALSE(isBrainPinValid(engineConfiguration->hpfpValvePin));
}

