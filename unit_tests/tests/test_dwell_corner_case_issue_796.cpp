/*
 * test_dwell_corner_case_issue_796.cpp
 *
 *  Created on: Jul 1, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "engine_test_helper.h"

TEST(scheduler, dwellIssue796) {

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth, IM_SEQUENTIAL);

	eth.fireTriggerEvents2(4 /* count */ , 600 /* ms */);

	// check if the mode is changed
	ASSERT_EQ(CRANKING, engine->rpmCalculator.getState());
	// due to isFasterEngineSpinUp=true, we should have already detected RPM!
	ASSERT_EQ( 100,  GET_RPM()) << "spinning-RPM#1";
	ASSERT_EQ(300000, ENGINE(rpmCalculator.oneDegreeUs) * 180);

	eth.fireRise(601);
	eth.fireFall(600);
	ASSERT_NEAR( 99,  GET_RPM(), EPS3D) << "spinning-RPM#2";
	ASSERT_EQ(303030, (int)(ENGINE(rpmCalculator.oneDegreeUs) * 180));


}
