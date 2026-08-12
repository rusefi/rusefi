/*
 * @file test_multispark.cpp
 *
 * @date Mar 15, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#include "pch.h"

using ::testing::_;
using ::testing::Return;

TEST(Multispark, DefaultConfiguration) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	EXPECT_EQ(0u, getMultiSparkCount(0    ));
	EXPECT_EQ(0u, getMultiSparkCount(100  ));
	EXPECT_EQ(0u, getMultiSparkCount(200  ));
	EXPECT_EQ(0u, getMultiSparkCount(500  ));
	EXPECT_EQ(0u, getMultiSparkCount(1000  ));
	EXPECT_EQ(0u, getMultiSparkCount(2000  ));
	EXPECT_EQ(0u, getMultiSparkCount(5000  ));
	EXPECT_EQ(0u, getMultiSparkCount(50000 ));
}

static void multisparkCfg() {
	// Turn it on!
	engineConfiguration->multisparkEnable = true;

	// Fire up to 45 degrees worth of sparks...
	engineConfiguration->multisparkMaxSparkingAngle = 45;

	// ...but limit to 10 additional sparks
	engineConfiguration->multisparkMaxExtraSparkCount = 10;

	// 3ms period (spark + dwell)
	engineConfiguration->multisparkDwell = 2;
	engineConfiguration->multisparkSparkDuration = 1;
}

TEST(Multispark, EnabledNoMaxRpm) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	multisparkCfg();

	// Practically no RPM limit
	engineConfiguration->multisparkMaxRpm = 12500;

	EXPECT_EQ(0u,  getMultiSparkCount(0    ));
	EXPECT_EQ(10u, getMultiSparkCount(150  ));
	EXPECT_EQ(10u, getMultiSparkCount(250  ));
	EXPECT_EQ(4u,  getMultiSparkCount(550  ));
	EXPECT_EQ(3u,  getMultiSparkCount(800  ));
	EXPECT_EQ(2u,  getMultiSparkCount(900  ));
	EXPECT_EQ(1u,  getMultiSparkCount(1500  ));

	// 2500 is the threshold where we should get zero
	EXPECT_EQ(1u,  getMultiSparkCount(2499  ));
	EXPECT_EQ(0u,  getMultiSparkCount(2501  ));

	EXPECT_EQ(0u,  getMultiSparkCount(5000  ));

	EXPECT_EQ(0u,  getMultiSparkCount(50000 ));
}

TEST(Multispark, RpmLimit) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	multisparkCfg();

	// Disable at 800 rpm
	engineConfiguration->multisparkMaxRpm = 800;

	EXPECT_EQ(3u, getMultiSparkCount(795));
	EXPECT_EQ(0u, getMultiSparkCount(805));
}

TEST(Multispark, IntegratedCoilRestrikes) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = false;

	EXPECT_CALL(*eth.mockAirmass, getAirmass(_, _))
		.WillRepeatedly(Return(AirmassResult{0.1008f, 50.0f}));

	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
	engineConfiguration->cylindersCount = 1;
	engineConfiguration->firingOrder = FO_1;
	engineConfiguration->isInjectionEnabled = false;
	engineConfiguration->isIgnitionEnabled = true;

	setWholeTimingTable(0);

	engineConfiguration->multisparkEnable = true;
	engineConfiguration->multisparkMaxSparkingAngle = 60;
	engineConfiguration->multisparkMaxRpm = 3000;
	// 0.5ms spark + 1ms dwell = 1.5ms per restrike: sparking window is not the limit,
	// so the count below stays at the maximum regardless of momentary RPM during spin-up
	engineConfiguration->multisparkSparkDuration = 0.5f;
	engineConfiguration->multisparkDwell = 1;
	engineConfiguration->multisparkMaxExtraSparkCount = 3;

	// Spin at 600 RPM - slow enough that multispark is active
	eth.fireTriggerEventsWithDuration(100);
	eth.fireTriggerEventsWithDuration(100);
	ASSERT_EQ(600, Sensor::getOrZero(SensorType::Rpm)) << "RPM";

	// one more cycle so a dwell gets scheduled with the multispark count already at steady state
	eth.smartFireTriggerEvents2(1, 100);
	engine->periodicFastCallback();

	// 60 degrees at 600 RPM is 16.7ms - fits 11 restrikes of 1.5ms, capped at 3
	ASSERT_EQ(3u, engine->engineState.multispark.count);

	eth.smartFireRise(100);

	// main dwell in progress
	EXPECT_EQ(enginePins.coils[0].getLogicValue(), true);

	eth.executeActions();

	// main spark just fired
	EXPECT_EQ(enginePins.coils[0].getLogicValue(), false);

	uint32_t outOfOrderBefore = engine->engineState.sparkOutOfOrderCounter;

	// Each restrike: coil re-charges multisparkSparkDuration (0.5ms) after the previous firing,
	// then fires again after multisparkDwell (1ms) of charging
	for (int i = 0; i < 3; i++) {
		eth.moveTimeForwardMs(0.6f);
		eth.executeActions();
		EXPECT_EQ(enginePins.coils[0].getLogicValue(), true) << "restrike dwell #" << i;

		eth.moveTimeForwardMs(1.0f);
		eth.executeActions();
		EXPECT_EQ(enginePins.coils[0].getLogicValue(), false) << "restrike fire #" << i;
	}

	// no restrike dwell was rejected by the [tag] #6349 out-of-order guard
	EXPECT_EQ(outOfOrderBefore, engine->engineState.sparkOutOfOrderCounter);

	// all restrikes are spent: coil must stay low
	eth.moveTimeForwardMs(5);
	eth.executeActions();
	EXPECT_EQ(enginePins.coils[0].getLogicValue(), false);
}
