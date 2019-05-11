/*
 * test_cam_vtt_input.cpp
 *
 *  Created on: Jan 13, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "engine_test_helper.h"

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
	eth.firePrimaryTriggerRise();
	eth.firePrimaryTriggerRise();
	eth.firePrimaryTriggerRise();
	// error condition since two events happened too quick
	ASSERT_EQ(NOISY_RPM,  GET_RPM()) << "testCamInput RPM should be noisy";


	eth.fireRise(50);
	eth.fireRise(50);
	eth.fireRise(50);
	eth.fireRise(50);
	eth.fireRise(50);
	ASSERT_EQ(1200,  GET_RPM()) << "testCamInput RPM";
}
