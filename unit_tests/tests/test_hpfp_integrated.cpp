/*
 * @file test_hpfp_integrated.cpp
 *
 *  Created on: Jan 18, 2022
 * More integrated version of HPFP test
 */

#include "pch.h"

TEST(HPFP, IntegratedSchedule) {
	EngineTestHelper eth(TEST_ENGINE, [](engine_configuration_s* engineConfiguration) {
		engineConfiguration->hpfpValvePin = GPIOA_2; // arbitrary
	});

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->hpfpCamLobes = 3;
	engineConfiguration->hpfpPumpVolume = 0.2; // cc/lobe


	engineConfiguration->trigger.customTotalToothCount = 16;
	engineConfiguration->trigger.customSkippedToothCount = 0;
	eth.setTriggerType(TT_TOOTHED_WHEEL);
	engineConfiguration->ambiguousOperationMode = FOUR_STROKE_CAM_SENSOR;
	engineConfiguration->isFasterEngineSpinUpEnabled = true;


	eth.smartFireTriggerEvents2(/*count*/40, /*delay*/ 4);
	ASSERT_EQ(937, GET_RPM());

	for (int i = 0;i<100;i++) {
		eth.smartFireTriggerEvents2(/*count*/1, /*delay*/ 4);
		engine->periodicFastCallback();
	}
	/**
	 * overall this is a pretty lame test but helps to know that the whole on/off/on dance does in fact happen for HPFP
	 */
	ASSERT_EQ(31, enginePins.hpfpValve.unitTestTurnedOnCounter);
}


