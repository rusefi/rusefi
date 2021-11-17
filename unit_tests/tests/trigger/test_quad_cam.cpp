/**
 * @file test_quad_cam.cpp
 *
 */

#include "pch.h"

TEST(trigger, testQuadCam) {
	// setting some weird engine
	EngineTestHelper eth(FORD_ESCORT_GT);

	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);

	// changing to 'ONE TOOTH' trigger on CRANK with CAM/VVT
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	engineConfiguration->vvtMode[0] = VVT_FIRST_HALF;
	engineConfiguration->vvtMode[1] = VVT_FIRST_HALF;

	engineConfiguration->camInputs[0] = GPIOA_10; // we just need to indicate that we have CAM

	// this crank trigger would be easier to test, crank shape is less important for this test
	eth.setTriggerType(TT_ONE);

	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	engineConfiguration->vvtCamSensorUseRise = true;

	ASSERT_EQ(0, GET_RPM());
	for (int i = 0; i < 3;i++) {
		eth.fireRise(25);
		ASSERT_EQ( 0,  GET_RPM());
	}
	eth.fireRise(25);
	// first time we have RPM
	ASSERT_EQ(2400, GET_RPM());

	// need to be out of VVT sync to see VVT sync in action
	eth.fireRise(25);
	eth.fireRise(25);

	eth.moveTimeForwardUs(MS2US(3)); // shifting VVT phase a few angles

	float d = 4;

	int firstCam = 0;
	int secondCam = 1;

	int firstBank = 0;
	int secondBank = 1;

	int firstCamSecondBank = secondBank * CAMS_PER_BANK + firstCam;
	int secondCamSecondBank = secondBank * CAMS_PER_BANK + secondCam;

	// Cams should have no position yet
	ASSERT_EQ(0, engine->triggerCentral.getVVTPosition(firstBank, firstCam));
	ASSERT_EQ(0, engine->triggerCentral.getVVTPosition(firstBank, secondCam));
	ASSERT_EQ(0, engine->triggerCentral.getVVTPosition(secondBank, firstCam));
	ASSERT_EQ(0, engine->triggerCentral.getVVTPosition(secondBank, secondCam));

	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), firstCam);
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), secondCam);
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), firstCamSecondBank);
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), secondCamSecondBank);

	float basePos = -80.2f;

	// All four cams should now have the same position
	EXPECT_NEAR(360 + basePos, engine->triggerCentral.getVVTPosition(firstBank, firstCam), EPS3D);
	EXPECT_NEAR(basePos, engine->triggerCentral.getVVTPosition(firstBank, secondCam), EPS3D);
	EXPECT_NEAR(basePos, engine->triggerCentral.getVVTPosition(secondBank, firstCam), EPS3D);
	EXPECT_NEAR(basePos, engine->triggerCentral.getVVTPosition(secondBank, secondCam), EPS3D);

	// Now fire cam events again, but with time gaps between each
	eth.moveTimeForwardMs(1);
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), firstCam);
	eth.moveTimeForwardMs(1);
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), secondCam);
	eth.moveTimeForwardMs(1);
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), firstCamSecondBank);
	eth.moveTimeForwardMs(1);
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), secondCamSecondBank);

	// All four cams should have different positions, each retarded by 1ms from the last
	float oneMsDegrees = 1000 / engine->rpmCalculator.oneDegreeUs;
	EXPECT_NEAR(basePos - oneMsDegrees * 1, engine->triggerCentral.getVVTPosition(firstBank, firstCam), EPS3D);
	EXPECT_NEAR(basePos - oneMsDegrees * 2, engine->triggerCentral.getVVTPosition(firstBank, secondCam), EPS3D);
	EXPECT_NEAR(basePos - oneMsDegrees * 3, engine->triggerCentral.getVVTPosition(secondBank, firstCam), EPS3D);
	EXPECT_NEAR(basePos - oneMsDegrees * 4, engine->triggerCentral.getVVTPosition(secondBank, secondCam), EPS3D);
}
