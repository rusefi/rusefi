/*
 * @file test_multispark.cpp
 *
 * @date Mar 15, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#include "pch.h"
#include "advance_map.h"

TEST(Multispark, DefaultConfiguration) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	EXPECT_EQ(0, getMultiSparkCount(0     PASS_ENGINE_PARAMETER_SUFFIX));
	EXPECT_EQ(0, getMultiSparkCount(100   PASS_ENGINE_PARAMETER_SUFFIX));
	EXPECT_EQ(0, getMultiSparkCount(200   PASS_ENGINE_PARAMETER_SUFFIX));
	EXPECT_EQ(0, getMultiSparkCount(500   PASS_ENGINE_PARAMETER_SUFFIX));
	EXPECT_EQ(0, getMultiSparkCount(1000   PASS_ENGINE_PARAMETER_SUFFIX));
	EXPECT_EQ(0, getMultiSparkCount(2000   PASS_ENGINE_PARAMETER_SUFFIX));
	EXPECT_EQ(0, getMultiSparkCount(5000   PASS_ENGINE_PARAMETER_SUFFIX));
	EXPECT_EQ(0, getMultiSparkCount(50000  PASS_ENGINE_PARAMETER_SUFFIX));
}

static void multisparkCfg(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// Turn it on!
	CONFIG(multisparkEnable) = true;

	// Fire up to 45 degrees worth of sparks...
	CONFIG(multisparkMaxSparkingAngle) = 45;

	// ...but limit to 10 additional sparks
	CONFIG(multisparkMaxExtraSparkCount) = 10;

	// 3ms period (spark + dwell)
	CONFIG(multisparkDwell) = 2000;
	CONFIG(multisparkSparkDuration) = 1000;
}

TEST(Multispark, EnabledNoMaxRpm) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	multisparkCfg(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Practically no RPM limit
	CONFIG(multisparkMaxRpm) = 65000;

	EXPECT_EQ(0,  getMultiSparkCount(0     PASS_ENGINE_PARAMETER_SUFFIX));
	EXPECT_EQ(10, getMultiSparkCount(150   PASS_ENGINE_PARAMETER_SUFFIX));
	EXPECT_EQ(10, getMultiSparkCount(250   PASS_ENGINE_PARAMETER_SUFFIX));
	EXPECT_EQ(4,  getMultiSparkCount(550   PASS_ENGINE_PARAMETER_SUFFIX));
	EXPECT_EQ(3,  getMultiSparkCount(800   PASS_ENGINE_PARAMETER_SUFFIX));
	EXPECT_EQ(2,  getMultiSparkCount(900   PASS_ENGINE_PARAMETER_SUFFIX));
	EXPECT_EQ(1,  getMultiSparkCount(1500   PASS_ENGINE_PARAMETER_SUFFIX));

	// 2500 is the threshold where we should get zero
	EXPECT_EQ(1,  getMultiSparkCount(2499   PASS_ENGINE_PARAMETER_SUFFIX));
	EXPECT_EQ(0,  getMultiSparkCount(2501   PASS_ENGINE_PARAMETER_SUFFIX));

	EXPECT_EQ(0,  getMultiSparkCount(5000   PASS_ENGINE_PARAMETER_SUFFIX));

	EXPECT_EQ(0,  getMultiSparkCount(50000  PASS_ENGINE_PARAMETER_SUFFIX));
}

TEST(Multispark, RpmLimit) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	multisparkCfg(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Disable at 800 rpm
	CONFIG(multisparkMaxRpm) = 800;

	EXPECT_EQ(3, getMultiSparkCount(795 PASS_ENGINE_PARAMETER_SUFFIX));
	EXPECT_EQ(0, getMultiSparkCount(805 PASS_ENGINE_PARAMETER_SUFFIX));
}
