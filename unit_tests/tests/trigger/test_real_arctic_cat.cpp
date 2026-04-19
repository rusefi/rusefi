#include "pch.h"
#include "real_trigger_helper.h"

TEST(arctic, realStartFromFile) {
	extern bool unitTestTaskPrecisionHack;
	unitTestTaskPrecisionHack = true;

	RealTriggerHelper helper;
	helper.runTest("tests/trigger/resources/arctic-cat.csv", trigger_type_e::TT_ARCTIC_CAT);

	// C9354 and C9351
	// C9007 and C9008
	ASSERT_EQ(4u, helper.eth.recentWarnings()->getCount()) << "warningCounter#arcticRealCranking";
	ASSERT_EQ(2165, round(Sensor::getOrZero(SensorType::Rpm)));
}
