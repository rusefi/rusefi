/**
 * @file test_quad_cam.cpp
 *
 */

#include "engine_test_helper.h"

TEST(trigger, testQuadCam) {
	// setting some weird engine
	WITH_ENGINE_TEST_HELPER(FORD_ESCORT_GT);

	// changing to 'ONE TOOTH' trigger on CRANK with CAM/VVT
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	engineConfiguration->vvtMode = VVT_2JZ;
	engineConfiguration->secondVvtMode = VVT_MIATA_NB2;

	eth.setTriggerType(TT_ONE PASS_ENGINE_PARAMETER_SUFFIX);
	engineConfiguration->camInputs[0] = GPIOA_10; // we just need to indicate that we have CAM

	ASSERT_EQ( 0,  GET_RPM()) << "testCamInput RPM";

}
