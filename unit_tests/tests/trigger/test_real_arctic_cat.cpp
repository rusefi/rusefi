#include "pch.h"
#include "real_trigger_helper.h"

TEST(arctic, realStartFromFile) {

	RealTriggerHelper helper;
	helper.runTest("tests/trigger/resources/arctic-cat.csv", trigger_type_e::TT_ARCTIC_CAT);

	// Tooth timing warnings remain, but the pending dwell plan no longer
	// produces spurious coil-overcharge warnings as the global dwell changes.
	ASSERT_EQ(2u, helper.eth.recentWarnings()->getCount()) << "warningCounter#arcticRealCranking";
	EXPECT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_PRIMARY_BAD_TOOTH_TIMING_EARLY));
	EXPECT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_PRIMARY_BAD_TOOTH_TIMING_LATE));
	EXPECT_FALSE(hasRecentWarningCode(ObdCode::CUSTOM_Ignition_Coil_Overcharge_1));
	EXPECT_FALSE(hasRecentWarningCode(ObdCode::CUSTOM_Ignition_Coil_Overcharge_4));
	ASSERT_EQ(2165, round(Sensor::getOrZero(SensorType::Rpm)));
}
