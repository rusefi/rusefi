/*
 * @file test_hpfp_integrated.cpp
 *
 *  Created on: Jan 18, 2022
 * More integrated version of HPFP test
 */

#include "pch.h"
using ::testing::_;

static size_t hpfpTotalToggle = 0;

static void assertToggleCounterExtra(EngineTestHelper *eth, size_t extra, const char *msg) {
	eth->smartFireTriggerEvents2(/*count*/4, /*delay*/ 16);
	ASSERT_EQ(hpfpTotalToggle + extra, enginePins.hpfpValve.pinToggleCounter) << msg;
	hpfpTotalToggle += extra;
}

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
	engineConfiguration->hpfpCam = HPFP_CAM_IN1;

	eth.smartFireTriggerEvents2(/*count*/8, /*delay*/ 16);
	ASSERT_EQ(937, round(Sensor::getOrZero(SensorType::Rpm)));


	hpfpTotalToggle = 6;
	/**
	 * overall this is a pretty lame test but helps to know that the whole on/off/on dance does in fact happen for HPFP
	 */
	ASSERT_EQ(hpfpTotalToggle, enginePins.hpfpValve.pinToggleCounter);

	assertToggleCounterExtra(&eth, 4, "#1");

	engine->triggerCentral.vvtPosition[0][0] = -100; // Bank 0

	assertToggleCounterExtra(&eth, 7, "#2");

	assertToggleCounterExtra(&eth, 6, "#3");

	assertToggleCounterExtra(&eth, 6, "#4");

	assertToggleCounterExtra(&eth, 6, "#5");
}


