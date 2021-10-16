/*
 * @file test_2jz_vvt.cpp
 *
 *  Created on: May 10, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

TEST(sensors, test2jz) {

	WITH_ENGINE_TEST_HELPER(TOYOTA_2JZ_GTE_VVTi);


	// this crank trigger would be easier to test, crank shape is less important for this test
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	eth.setTriggerType(TT_ONE PASS_ENGINE_PARAMETER_SUFFIX);

	ASSERT_EQ( 0,  GET_RPM()) << "test2jz RPM";
	for (int i = 0; i < 3;i++) {
		eth.fireRise(25);
		ASSERT_EQ( 0,  GET_RPM()) << "test2jz RPM at " << i;
	}
	eth.fireRise(25);
	// first time we have RPM
	ASSERT_EQ(2400,  GET_RPM()) << "test2jz RPM";


	eth.moveTimeForwardUs(MS2US(3)); // shifting VVT phase a few angles

	hwHandleVvtCamSignal(TV_FALL, getTimeNowNt(), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt(), 0 PASS_ENGINE_PARAMETER_SUFFIX);

	// currentPosition
	ASSERT_NEAR(608.2 - 720, engine->triggerCentral.currentVVTEventPosition[0][0], EPS3D);
}
