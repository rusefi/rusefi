/*
 * @file test_hpfp_integrated.cpp
 *
 *  Created on: Jan 18, 2022
 * More integrated version of HPFP test
 */

#include "pch.h"
#include "high_pressure_fuel_pump.h"
using ::testing::_;

static void checkPendingHpfpStart(bool loseSync) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE, [](engine_configuration_s* cfg) {
		cfg->hpfpValvePin = Gpio::A2;
		cfg->hpfpCamLobes = 0;
	});
	engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 12;
	engineConfiguration->trigger.customSkippedToothCount = 1;
	engine->updateTriggerConfiguration();
	engine->rpmCalculator.setRpmValue(1000);
	engineConfiguration->hpfpCamLobes = 1;
	engineConfiguration->hpfpCam = HPFP_CAM_NONE;
	engineConfiguration->hpfpPumpVolume = 0.2f;
	engineConfiguration->hpfpPeakPos = 60;
	engineConfiguration->hpfpMinAngle = 1;
	engineConfiguration->hpfpActivationAngle = 20;
	setArrayValues(config->hpfpDeadtimeMS, 0);
	setArrayValues(config->hpfpLobeProfileAngle, 30);
	ASSERT_GT(engine->triggerCentral.triggerShape.getSize(), 6);
	auto& controller = engine->module<HpfpController>().unmock();
	controller.onFastCallback();
	ASSERT_TRUE(controller.isHpfpActive);
	ASSERT_FLOAT_EQ(30, controller.m_event.getAngle());
	engine->module<TriggerScheduler>()->scheduleEventsUntilNextTriggerTooth(
		1000, getTimeNowNt(), 0, 40);
	ASSERT_EQ(1, engine->scheduler.size());
	ASSERT_EQ(&controller.m_event.eventScheduling, engine->scheduler.getHead());
	const auto scheduledAction = engine->scheduler.getHead()->action;
	ASSERT_EQ(action_s::make<HpfpController::pinTurnOn>(&controller), scheduledAction);

	eth.moveTimeForwardUs(1000);
	if (loseSync) {
		engine->OnTriggerSynchronizationLost();
		controller.onFastCallback();
		ASSERT_TRUE(engine->rpmCalculator.isStopped());
		EXPECT_FALSE(controller.isHpfpActive);
		EXPECT_FLOAT_EQ(0, controller.m_requested_pump);
		EXPECT_EQ(1, engine->scheduler.size());
		EXPECT_EQ(scheduledAction, engine->scheduler.getHead()->action);
		EXPECT_TRUE(std::isnan(engine->rpmCalculator.getOneDegreeUs()));
		// Do not execute the stale pinTurnOn: it sets the valve high then calls
		// scheduleByAngle for its off event using NaN oneDegreeUs. That reaches
		// sumTickAndFloat's undefined float-to-int conversion. This test proves
		// the reachable callback and invalid timing premise without invoking UB.
		engine->scheduler.clear();
	} else {
		eth.moveTimeForwardUs(4000);
		ASSERT_EQ(1, eth.executeActions());
		EXPECT_TRUE(enginePins.hpfpValve.getLogicValue());
		eth.moveTimeForwardUs(3500);
		ASSERT_EQ(1, eth.executeActions());
		EXPECT_FALSE(enginePins.hpfpValve.getLogicValue());
		EXPECT_EQ(0, engine->scheduler.size());
	}
}

TEST(HPFP, SyncLossRetainsPendingStartWithInvalidOffTiming) {
	checkPendingHpfpStart(true);
}

TEST(HPFP, PendingStartClosesValveWithValidRpm) {
	checkPendingHpfpStart(false);
}

#if FUEL_RPM_COUNT == 16
static int hpfpTotalToggle = 0;

static void assertToggleCounterExtra(EngineTestHelper *eth, int extra, const char *msg) {
	eth->smartFireTriggerEvents2(/*count*/4, /*delay*/ 16);
	ASSERT_EQ(hpfpTotalToggle + extra, enginePins.hpfpValve.pinToggleCounter) << msg;
	hpfpTotalToggle += extra;
}

TEST(HPFP, IntegratedSchedule) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE, [](engine_configuration_s* cfg) {
		cfg->hpfpValvePin = Gpio::A2; // arbitrary
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
#endif //FUEL_RPM_COUNT == 16

