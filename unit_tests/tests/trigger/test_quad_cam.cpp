/**
 * @file test_quad_cam.cpp
 *
 */

#include "engine_test_helper.h"

TEST(trigger, testQuadCam) {
	// setting some weird engine
	WITH_ENGINE_TEST_HELPER(FORD_ESCORT_GT);

	// changing to 'ONE TOOTH' trigger on CRANK with CAM/VVT
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	engineConfiguration->vvtMode[0] = VVT_2JZ;
	engineConfiguration->vvtMode[1] = VVT_MIATA_NB2;

	engineConfiguration->camInputs[0] = GPIOA_10; // we just need to indicate that we have CAM

	// this crank trigger would be easier to test, crank shape is less important for this test
	eth.setTriggerType(TT_ONE PASS_ENGINE_PARAMETER_SUFFIX);

	engineConfiguration->useOnlyRisingEdgeForTrigger = true;

	ASSERT_EQ( 0,  GET_RPM()) << "testQuadCam RPM no sync";
	for (int i = 0; i < 1;i++) {
		eth.fireRise(25);
		ASSERT_EQ( 0,  GET_RPM()) << "testQuadCam RPM still no sync";
	}
	eth.fireRise(25);
	// first time we have RPM
	ASSERT_EQ(4800,  GET_RPM()) << "testQuadCam RPM";

	int totalRevolutionCountBeforeVvtSync = 4;
	// need to be out of VVT sync to see VVT sync in action
	eth.fireRise(25);
	eth.fireRise(25);
	ASSERT_EQ(totalRevolutionCountBeforeVvtSync, engine->triggerCentral.triggerState.getTotalRevolutionCounter());

	eth.moveTimeForwardUs(MS2US(3)); // shifting VVT phase a few angles

	float d = 4;

	int firstCam = 0;
	int secondCam = 1;

	// this would be ignored since we only consume the other kind of fronts here
	hwHandleVvtCamSignal(TV_FALL, getTimeNowNt(), secondCam PASS_ENGINE_PARAMETER_SUFFIX);
	eth.moveTimeForwardUs(MS2US(20 / d));
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), firstCam PASS_ENGINE_PARAMETER_SUFFIX);
	// this would be be first VVT signal - gap duration would be calculated against 'DEEP_IN_THE_PAST_SECONDS' initial value
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), secondCam PASS_ENGINE_PARAMETER_SUFFIX);

	eth.moveTimeForwardUs(MS2US(20 / d));
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), secondCam PASS_ENGINE_PARAMETER_SUFFIX);
	// this second important front would give us first real VVT gap duration
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), secondCam PASS_ENGINE_PARAMETER_SUFFIX);

	ASSERT_FLOAT_EQ(0, engine->triggerCentral.getVVTPosition(0, firstCam));
	ASSERT_EQ(totalRevolutionCountBeforeVvtSync, engine->triggerCentral.triggerState.getTotalRevolutionCounter());

	eth.moveTimeForwardUs(MS2US(130 / d));
	// this third important front would give us first comparison between two real gaps
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), secondCam PASS_ENGINE_PARAMETER_SUFFIX);

	ASSERT_NEAR(0, engine->triggerCentral.getVVTPosition(0, secondCam), EPS3D);
	// actually position based on VVT!
	ASSERT_EQ(totalRevolutionCountBeforeVvtSync, engine->triggerCentral.triggerState.getTotalRevolutionCounter());


	int secondBank = 1;
	int secondCamSecondBank = secondBank * CAMS_PER_BANK + secondCam;

	// this would be ignored since we only consume the other kind of fronts here
	hwHandleVvtCamSignal(TV_FALL, getTimeNowNt(), secondCamSecondBank PASS_ENGINE_PARAMETER_SUFFIX);
	eth.moveTimeForwardUs(MS2US(20 / d));
	// this would be be first VVT signal - gap duration would be calculated against 'DEEP_IN_THE_PAST_SECONDS' initial value
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), secondCamSecondBank PASS_ENGINE_PARAMETER_SUFFIX);

	eth.moveTimeForwardUs(MS2US(20 / d));
	// this second important front would give us first real VVT gap duration
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), secondCamSecondBank PASS_ENGINE_PARAMETER_SUFFIX);

	ASSERT_FLOAT_EQ(0, engine->triggerCentral.getVVTPosition(0, firstCam));
	ASSERT_EQ(totalRevolutionCountBeforeVvtSync, engine->triggerCentral.triggerState.getTotalRevolutionCounter());

	eth.moveTimeForwardUs(MS2US(130 / d));
	// this third important front would give us first comparison between two real gaps
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), secondCamSecondBank PASS_ENGINE_PARAMETER_SUFFIX);

	ASSERT_NEAR(308.6, engine->triggerCentral.getVVTPosition(secondBank, secondCam), EPS3D);
	// actually position based on VVT!
	ASSERT_EQ(totalRevolutionCountBeforeVvtSync, engine->triggerCentral.triggerState.getTotalRevolutionCounter());

}
