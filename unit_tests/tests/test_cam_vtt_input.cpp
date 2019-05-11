/*
 * test_cam_vtt_input.cpp
 *
 *  Created on: Jan 13, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "engine_test_helper.h"
extern WarningCodeState unitTestWarningCodeState;

TEST(sensors, testNoStartUpWarningsNoSyncronizationTrigger) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	// one tooth does not need synchronization it just counts tooth
	eth.setTriggerType(TT_ONE PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ( 0,  GET_RPM()) << "testNoStartUpWarnings RPM";

	eth.fireTriggerEvents2(/*count*/10, /*duration*/50);
	ASSERT_EQ(1200,  GET_RPM()) << "testNoStartUpWarnings RPM";
	ASSERT_EQ( 0,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testNoStartUpWarningsNoSyncronizationTrigger";
}

TEST(sensors, testNoStartUpWarnings) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	// for this test we need a trigger with isSynchronizationNeeded=true
	engineConfiguration->trigger.customTotalToothCount = 3;
	engineConfiguration->trigger.customSkippedToothCount = 1;
	eth.setTriggerType(TT_TOOTHED_WHEEL PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ( 0,  GET_RPM()) << "testNoStartUpWarnings RPM";

	for (int i = 0;i < 10;i++) {
		eth.fireRise(50);
		eth.fireFall(50);
		eth.fireRise(50);
		eth.fireFall(150);
	}

	ASSERT_EQ(400,  GET_RPM()) << "testNoStartUpWarnings RPM";
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
	ASSERT_EQ( 2,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testNoStartUpWarnings CUSTOM_SYNC_COUNT_MISMATCH expected";
	ASSERT_EQ(CUSTOM_SYNC_ERROR, unitTestWarningCodeState.recentWarnings.get(0));
	ASSERT_EQ(CUSTOM_SYNC_COUNT_MISMATCH, unitTestWarningCodeState.recentWarnings.get(1));
}

TEST(sensors, testNoisyInput) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	ASSERT_EQ( 0,  GET_RPM()) << "testNoisyInput RPM";

	eth.firePrimaryTriggerRise();
	eth.firePrimaryTriggerFall();
	eth.firePrimaryTriggerRise();
	eth.firePrimaryTriggerFall();
	eth.firePrimaryTriggerRise();
	eth.firePrimaryTriggerFall();
	eth.firePrimaryTriggerRise();
	eth.firePrimaryTriggerFall();
	// error condition since events happened too quick while time does not move
	ASSERT_EQ(NOISY_RPM,  GET_RPM()) << "testNoisyInput RPM should be noisy";

	ASSERT_EQ( 2,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testNoisyInput";
	ASSERT_EQ(CUSTOM_SYNC_COUNT_MISMATCH, unitTestWarningCodeState.recentWarnings.get(0)) << "@0";
	ASSERT_EQ(OBD_Camshaft_Position_Sensor_Circuit_Range_Performance, unitTestWarningCodeState.recentWarnings.get(1)) << "@0";
}

TEST(sensors, testCamInput) {
	// setting some weird engine
	WITH_ENGINE_TEST_HELPER(FORD_ESCORT_GT);

	// and now changing to ONE trigger on CRANK with CAM/VVT

	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	eth.setTriggerType(TT_ONE PASS_ENGINE_PARAMETER_SUFFIX);
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	engineConfiguration->camInput = GPIOA_10;

	ASSERT_EQ( 0,  GET_RPM()) << "testCamInput RPM";

	eth.fireRise(50);
	eth.fireRise(50);
	eth.fireRise(50);
	eth.fireRise(50);
	eth.fireRise(50);
	ASSERT_EQ(1200,  GET_RPM()) << "testCamInput RPM";
	ASSERT_EQ(1,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testCamInput";

	for (int i = 0; i < 100;i++)
		eth.fireRise(50);

}
