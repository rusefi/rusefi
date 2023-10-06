/*
 * @file test_hpfp_integrated.cpp
 *
 *  Created on: Jan 18, 2022
 * More integrated version of HPFP test
 */

#include "pch.h"
using ::testing::_;

TEST(HPFP, IntegratedSchedule) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE, [](engine_configuration_s* engineConfiguration) {
		engineConfiguration->hpfpValvePin = Gpio::A2; // arbitrary
	});

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->hpfpCamLobes = 3;
	engineConfiguration->hpfpPumpVolume = 0.2; // cc/lobe

	EXPECT_CALL(*eth.mockAirmass, getAirmass(_, _))
		.WillRepeatedly(Return(AirmassResult{/*airmass*/1, /*load*/50.0f}));

	engineConfiguration->trigger.customTotalToothCount = 4;
	engineConfiguration->trigger.customSkippedToothCount = 0;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL);
    setCamOperationMode();
	engineConfiguration->isFasterEngineSpinUpEnabled = true;


	eth.smartFireTriggerEvents2(/*count*/8, /*delay*/ 16);
	ASSERT_EQ(937, round(Sensor::getOrZero(SensorType::Rpm)));

	/**
	 * overall this is a pretty lame test but helps to know that the whole on/off/on dance does in fact happen for HPFP
	 */
	ASSERT_EQ(10, enginePins.hpfpValve.pinToggleCounter);
}


