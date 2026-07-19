#include "pch.h"
#include "real_trigger_helper.h"

TEST(arctic, realStartFromFile) {

	RealTriggerHelper helper;
	helper.runTest("tests/trigger/resources/arctic-cat.csv", trigger_type_e::TT_ARCTIC_CAT);

	// C9354 and C9351
	// C9007 and C9008
	// todo: address later: +1 is a false positive CUSTOM_OBD_impossibly_short_INJECTION - injection events run with zero fuel mass in this test
	ASSERT_EQ(5u, helper.eth.recentWarnings()->getCount()) << "warningCounter#arcticRealCranking";
	ASSERT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_OBD_impossibly_short_INJECTION));
	ASSERT_EQ(2165, round(Sensor::getOrZero(SensorType::Rpm)));
}
