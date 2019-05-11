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
	eth.setTriggerType(TT_ONE PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ( 0,  GET_RPM()) << "testNoStartUpWarnings RPM";

	// for this test we need a trigger with isSynchronizationNeeded=true

	eth.fireTriggerEvents2(/*count*/10, /*duration*/50);
	ASSERT_EQ(1200,  GET_RPM()) << "testNoStartUpWarnings RPM";
	ASSERT_EQ( 0,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testNoStartUpWarnings";
	// now let's post invalid shape
	eth.fireRise(50);
	eth.fireRise(50);
	eth.fireRise(50);
	eth.fireRise(50);
	ASSERT_EQ( 0,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testNoStartUpWarnings CUSTOM_SYNC_COUNT_MISMATCH expected";
//	ASSERT_EQ(CUSTOM_SYNC_COUNT_MISMATCH, unitTestWarningCodeState.recentWarnings.get(0));
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

	eth.firePrimaryTriggerRise();
	eth.firePrimaryTriggerFall();
	eth.firePrimaryTriggerRise();
	eth.firePrimaryTriggerFall();
	eth.firePrimaryTriggerRise();
	eth.firePrimaryTriggerFall();
	eth.firePrimaryTriggerRise();
	eth.firePrimaryTriggerFall();
	// error condition since events happened too quick while time does not move
	ASSERT_EQ(NOISY_RPM,  GET_RPM()) << "testCamInput RPM should be noisy";

	// todo: open question what are these warnings about?
	ASSERT_EQ( 2,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testCamInput";
	unitTestWarningCodeState.recentWarnings.clear();

	eth.fireRise(50);
	eth.fireRise(50);
	eth.fireRise(50);
	eth.fireRise(50);
	eth.fireRise(50);
	ASSERT_EQ(1200,  GET_RPM()) << "testCamInput RPM";
	ASSERT_EQ(2,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testCamInput";

	for (int i = 0; i < 100;i++)
		eth.fireRise(50);

}
