/*
 * test_cam_vvt_input.cpp
 *
 *  Created on: Jan 13, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

extern WarningCodeState unitTestWarningCodeState;

#include "engine_sniffer.h"
extern WaveChart waveChart;

TEST(trigger, testNoStartUpWarningsNoSyncronizationTrigger) {
	EngineTestHelper eth(TEST_ENGINE);
	// one tooth does not need synchronization it just counts tooth
	eth.setTriggerType(TT_ONE);
	ASSERT_EQ( 0,  round(Sensor::getOrZero(SensorType::Rpm))) << "testNoStartUpWarnings RPM";

	eth.fireTriggerEvents2(/*count*/10, /*duration*/50);
	ASSERT_EQ(1200,  round(Sensor::getOrZero(SensorType::Rpm))) << "testNoStartUpWarnings RPM";
	ASSERT_EQ( 0,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testNoStartUpWarningsNoSyncronizationTrigger";
}

TEST(trigger, testNoStartUpWarnings) {
	EngineTestHelper eth(TEST_ENGINE);
	// for this test we need a trigger with isSynchronizationNeeded=true
	engineConfiguration->trigger.customTotalToothCount = 3;
	engineConfiguration->trigger.customSkippedToothCount = 1;
	eth.setTriggerType(TT_TOOTHED_WHEEL);
	ASSERT_EQ( 0,  round(Sensor::getOrZero(SensorType::Rpm))) << "testNoStartUpWarnings RPM";

	for (int i = 0;i < 10;i++) {
		eth.fireRise(50);
		eth.fireFall(50);
		eth.fireRise(50);
		eth.fireFall(150);
	}

	ASSERT_EQ(400,  round(Sensor::getOrZero(SensorType::Rpm))) << "testNoStartUpWarnings RPM";
	ASSERT_EQ( 0,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testNoStartUpWarnings";
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
	EXPECT_EQ( 1,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testNoStartUpWarnings CUSTOM_SYNC_COUNT_MISMATCH expected";
	EXPECT_EQ(CUSTOM_SYNC_ERROR, unitTestWarningCodeState.recentWarnings.get(0).Code);
}

TEST(trigger, testNoisyInput) {
	EngineTestHelper eth(TEST_ENGINE);

	ASSERT_EQ( 0,  round(Sensor::getOrZero(SensorType::Rpm))) << "testNoisyInput RPM";

	eth.firePrimaryTriggerRise();
	eth.firePrimaryTriggerFall();
	eth.firePrimaryTriggerRise();
	eth.firePrimaryTriggerFall();
	eth.firePrimaryTriggerRise();
	eth.firePrimaryTriggerFall();
	eth.firePrimaryTriggerRise();
	eth.firePrimaryTriggerFall();
	// error condition since events happened too quick while time does not move
	ASSERT_EQ(NOISY_RPM,  Sensor::getOrZero(SensorType::Rpm)) << "testNoisyInput RPM should be noisy";

	ASSERT_EQ( 2,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testNoisyInput";
	ASSERT_EQ(CUSTOM_SYNC_COUNT_MISMATCH, unitTestWarningCodeState.recentWarnings.get(0).Code) << "@0";
	ASSERT_EQ(OBD_Crankshaft_Position_Sensor_A_Circuit_Malfunction, unitTestWarningCodeState.recentWarnings.get(1).Code) << "@0";
}

TEST(trigger, testCamInput) {
	// setting some weird engine
	EngineTestHelper eth(FORD_ESCORT_GT);

	// changing to 'ONE TOOTH' trigger on CRANK with CAM/VVT
	setCrankOperationMode();
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	engineConfiguration->vvtMode[0] = VVT_FIRST_HALF;
	engineConfiguration->vvtOffsets[0] = 360;
	eth.setTriggerType(TT_ONE);
	engineConfiguration->camInputs[0] = Gpio::A10; // we just need to indicate that we have CAM

	ASSERT_EQ( 0,  round(Sensor::getOrZero(SensorType::Rpm))) << "testCamInput RPM";

	for (int i = 0; i < 5;i++) {
		eth.fireRise(50);
	}

	ASSERT_EQ(1200,  round(Sensor::getOrZero(SensorType::Rpm))) << "testCamInput RPM";
	ASSERT_EQ(0,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testCamInput";

	for (int i = 0; i < 600;i++) {
		eth.fireRise(50);
	}

	// asserting that lack of camshaft signal would be detecting
	ASSERT_EQ(1,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testCamInput #2";
	ASSERT_EQ(OBD_Camshaft_Position_Sensor_Circuit_Range_Performance, unitTestWarningCodeState.recentWarnings.get(0).Code) << "@0";
	unitTestWarningCodeState.recentWarnings.clear();

	for (int i = 0; i < 600;i++) {
		eth.moveTimeForwardUs(MS2US(10));
		hwHandleVvtCamSignal(TV_FALL, getTimeNowNt(), 0);
		eth.moveTimeForwardUs(MS2US(40));
		eth.firePrimaryTriggerRise();
	}

	// asserting that error code has cleared
	ASSERT_EQ(0,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testCamInput #3";
	EXPECT_NEAR_M3(-181, engine->triggerCentral.getVVTPosition(0, 0));
}

TEST(trigger, testNB2CamInput) {
	EngineTestHelper eth(FRANKENSO_MAZDA_MIATA_2003);

	engineConfiguration->isFasterEngineSpinUpEnabled = false;

	engineConfiguration->useOnlyRisingEdgeForTrigger = true;

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
	ASSERT_EQ(totalRevolutionCountBeforeVvtSync, engine->triggerCentral.triggerState.getTotalRevolutionCounter());
	ASSERT_TRUE((totalRevolutionCountBeforeVvtSync % SYMMETRICAL_CRANK_SENSOR_DIVIDER) != 0);

	eth.moveTimeForwardUs(MS2US(3)); // shifting VVT phase a few angles

	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), 0);

	// first gap - long

	eth.moveTimeForwardUs(MS2US(130));
	hwHandleVvtCamSignal(TV_FALL, getTimeNowNt(), 0);
	eth.moveTimeForwardUs(MS2US( 30));
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), 0);

	// second gap - short

	eth.moveTimeForwardUs(MS2US(10));
	hwHandleVvtCamSignal(TV_FALL, getTimeNowNt(), 0);
	eth.moveTimeForwardUs(MS2US(10));
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), 0);

	ASSERT_FLOAT_EQ(0, engine->triggerCentral.getVVTPosition(0, 0));
	ASSERT_EQ(totalRevolutionCountBeforeVvtSync, engine->triggerCentral.triggerState.getTotalRevolutionCounter());

	// Third gap - long

	eth.moveTimeForwardUs(MS2US(130));
	hwHandleVvtCamSignal(TV_FALL, getTimeNowNt(), 0);
	eth.moveTimeForwardUs(MS2US( 30));
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), 0);

	EXPECT_NEAR(290.5f, engine->triggerCentral.getVVTPosition(0, 0), EPS2D);
	// actually position based on VVT!
	ASSERT_EQ(totalRevolutionCountBeforeVvtSync + 3, engine->triggerCentral.triggerState.getTotalRevolutionCounter());

	EXPECT_EQ(40, waveChart.getSize());
}
