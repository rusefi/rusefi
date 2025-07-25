/*
 * test_cam_vvt_input.cpp
 *
 *  Created on: Jan 13, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "engine_sniffer.h"
extern WaveChart waveChart;

TEST(trigger, testNoStartUpWarningsNoSynchronizationTrigger) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// one tooth does not need synchronization it just counts tooth
	eth.setTriggerType(trigger_type_e::TT_HALF_MOON);
	ASSERT_EQ( 0,  round(Sensor::getOrZero(SensorType::Rpm))) << "testNoStartUpWarnings RPM";

	eth.fireTriggerEvents2(/*count*/10, /*duration*/50);
	ASSERT_EQ(1200,  round(Sensor::getOrZero(SensorType::Rpm))) << "testNoStartUpWarnings RPM";
	ASSERT_EQ( 0u,  getRecentWarnings()->getCount()) << "warningCounter#testNoStartUpWarningsNoSynchronizationTrigger";
}

TEST(trigger, testNoStartUpWarnings) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// for this test we need a trigger with isSynchronizationNeeded=true
	engineConfiguration->trigger.customTotalToothCount = 3;
	engineConfiguration->trigger.customSkippedToothCount = 1;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL);
	ASSERT_EQ( 0,  round(Sensor::getOrZero(SensorType::Rpm))) << "testNoStartUpWarnings RPM";

	for (int i = 0;i < 10;i++) {
		eth.fireRise(50);
		eth.fireFall(50);
		eth.fireRise(50);
		eth.fireFall(150);
	}

	ASSERT_EQ(400,  round(Sensor::getOrZero(SensorType::Rpm))) << "testNoStartUpWarnings RPM";
	ASSERT_EQ( 0u,  getRecentWarnings()->getCount()) << "warningCounter#testNoStartUpWarnings";
	// now let's post something unneeded
	eth.fireRise(50);
	eth.fireFall(50);
	eth.fireRise(50); // this is noise
	eth.fireFall(50); // this is noise
	eth.fireRise(50);
	eth.fireFall(150);
	for (int i = 0;i < 1;i++) {
		eth.fireRise(50);
		eth.fireFall(50);
		eth.fireRise(50);
		eth.fireFall(150);
	}
	EXPECT_EQ( 1u,  getRecentWarnings()->getCount()) << "warningCounter#testNoStartUpWarnings CUSTOM_SYNC_COUNT_MISMATCH expected";
	EXPECT_EQ(ObdCode::CUSTOM_PRIMARY_TOO_MANY_TEETH, getRecentWarnings()->get(0).Code);
}

TEST(trigger, testNoisyInput) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	ASSERT_EQ(0, Sensor::getOrZero(SensorType::Rpm));

	eth.firePrimaryTriggerRise();
	eth.firePrimaryTriggerFall();
	eth.firePrimaryTriggerRise();
	eth.firePrimaryTriggerFall();
	eth.firePrimaryTriggerRise();
	eth.firePrimaryTriggerFall();
	eth.firePrimaryTriggerRise();
	eth.firePrimaryTriggerFall();
	ASSERT_EQ(0, Sensor::getOrZero(SensorType::Rpm));

	EXPECT_EQ(1u, getRecentWarnings()->getCount());
	EXPECT_EQ(ObdCode::CUSTOM_PRIMARY_NOT_ENOUGH_TEETH, getRecentWarnings()->get(0).Code);
}

TEST(trigger, testCamInput) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->globalTriggerAngleOffset = -37;
	engineConfiguration->crankingTimingAngle = 3;
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;

	// changing to 'ONE TOOTH' trigger on CRANK with CAM/VVT
	setCrankOperationMode();
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;
	engineConfiguration->vvtOffsets[0] = 360;
	eth.setTriggerType(trigger_type_e::TT_HALF_MOON);
	engineConfiguration->camInputs[0] = Gpio::A10; // we just need to indicate that we have CAM

	ASSERT_EQ( 0,  round(Sensor::getOrZero(SensorType::Rpm))) << "testCamInput RPM";

	for (int i = 0; i < 5;i++) {
		eth.fireRise(25);
		eth.fireFall(25);
	}

	ASSERT_EQ(1200,  round(Sensor::getOrZero(SensorType::Rpm)));
	ASSERT_EQ(0u,  getRecentWarnings()->getCount()) << "warningCounter#testCamInput";

	for (int i = 0; i < 600;i++) {
		eth.fireRise(25);
		eth.fireFall(25);
	}

	// asserting that lack of camshaft signal would be detecting
	ASSERT_EQ(1u,  getRecentWarnings()->getCount()) << "warningCounter#testCamInput #2";
	ASSERT_EQ(ObdCode::OBD_Camshaft_Position_Sensor_Circuit_Range_Performance, getRecentWarnings()->get(0).Code) << "@0";
	getRecentWarnings()->clear();

	for (int i = 0; i < 600;i++) {
		eth.moveTimeForwardUs(MS2US(10));

		// cam comes every other crank rev
		if (i % 2 == 0) {
			hwHandleVvtCamSignal(TriggerValue::RISE, getTimeNowNt(), 0);
		}

		eth.moveTimeForwardUs(MS2US(15));

		eth.firePrimaryTriggerRise();
		EXPECT_EQ(1200,  round(Sensor::getOrZero(SensorType::Rpm)));

		eth.moveTimeForwardUs(MS2US(25));
		eth.firePrimaryTriggerFall();
	}

	// asserting that error code has cleared
	ASSERT_EQ(0u, getRecentWarnings()->getCount()) << "warningCounter#testCamInput #3";
	EXPECT_NEAR_M3(71, engine->triggerCentral.getVVTPosition(0, 0));
}

TEST(trigger, testNB2CamInput) {
	EngineTestHelper eth(engine_type_e::MAZDA_MIATA_NB2);

	engineConfiguration->isFasterEngineSpinUpEnabled = false;

	ASSERT_EQ( 0,  round(Sensor::getOrZero(SensorType::Rpm)));
	for (int i = 0; i < 6;i++) {
		eth.fireRise(25 * 70 / 180);
		eth.fireRise(25 * 110 / 180);
		ASSERT_EQ( 0,  round(Sensor::getOrZero(SensorType::Rpm)));
	}
	eth.fireRise(25 * 70 / 180);
	eth.fireRise(25 * 110 / 180);
	// first time we have RPM
	ASSERT_EQ(1250,  round(Sensor::getOrZero(SensorType::Rpm)));

	int totalRevolutionCountBeforeVvtSync = 5;
	// need to be out of VVT sync to see VVT sync in action
	eth.fireRise(25 * 70 / 180);
	eth.fireRise(25 * 110 / 180);
	ASSERT_EQ(totalRevolutionCountBeforeVvtSync, engine->triggerCentral.triggerState.getSynchronizationCounter());
	ASSERT_TRUE((totalRevolutionCountBeforeVvtSync % SYMMETRICAL_CRANK_SENSOR_DIVIDER) != 0);

	eth.moveTimeForwardUs(MS2US(3)); // shifting VVT phase a few angles

	hwHandleVvtCamSignal(true, getTimeNowNt(), 0);

	// first gap - long

	eth.moveTimeForwardUs(MS2US(130));
	hwHandleVvtCamSignal(false, getTimeNowNt(), 0);
	eth.moveTimeForwardUs(MS2US( 30));
	hwHandleVvtCamSignal(true, getTimeNowNt(), 0);

	// second gap - short

	eth.moveTimeForwardUs(MS2US(10));
	hwHandleVvtCamSignal(false, getTimeNowNt(), 0);
	eth.moveTimeForwardUs(MS2US(10));
	hwHandleVvtCamSignal(true, getTimeNowNt(), 0);

	ASSERT_FLOAT_EQ(0, engine->triggerCentral.getVVTPosition(0, 0));
	ASSERT_EQ(totalRevolutionCountBeforeVvtSync, engine->triggerCentral.triggerState.getSynchronizationCounter());

	// Third gap - long

	eth.moveTimeForwardUs(MS2US(130));
	hwHandleVvtCamSignal(false, getTimeNowNt(), 0);
	eth.moveTimeForwardUs(MS2US( 30));
	hwHandleVvtCamSignal(true, getTimeNowNt(), 0);

	EXPECT_NEAR(290.5f, engine->triggerCentral.getVVTPosition(0, 0), EPS2D);
	// actually position based on VVT!
	ASSERT_EQ(totalRevolutionCountBeforeVvtSync + 3, engine->triggerCentral.triggerState.getSynchronizationCounter());

	EXPECT_EQ(40, waveChart.getSize());
}
