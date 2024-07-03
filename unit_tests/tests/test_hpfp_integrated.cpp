/*
 * @file test_hpfp_integrated.cpp
 *
 *  Created on: Jan 18, 2022
 * More integrated version of HPFP test
 */

#include "pch.h"

TEST(HPFP, IntegratedSchedule) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE, [](engine_configuration_s* cfg) {
		cfg->hpfpValvePin = Gpio::A2; // arbitrary
	});

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->hpfpCamLobes = 3;
	engineConfiguration->hpfpPumpVolume = 0.2; // cc/lobe


	engineConfiguration->trigger.customTotalToothCount = 16;
	engineConfiguration->trigger.customSkippedToothCount = 0;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL);
    setCamOperationMode();
	engineConfiguration->isFasterEngineSpinUpEnabled = true;


	eth.smartFireTriggerEvents2(/*count*/40, /*delay*/ 4);
	ASSERT_EQ(937, round(Sensor::getOrZero(SensorType::Rpm)));

	for (int i = 0; i < 100; i++) {
		eth.smartFireTriggerEvents2(/*count*/1, /*delay*/ 4);
		engine->periodicFastCallback();
	}
	/**
	 * overall this is a pretty lame test but helps to know that the whole on/off/on dance does in fact happen for HPFP
	 */
	ASSERT_EQ(31, enginePins.hpfpValve.unitTestTurnedOnCounter);
}


