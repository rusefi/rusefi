#include "pch.h"
#include "real_trigger_helper.h"

TEST(realJeepEva, cranking) {
	RealTriggerHelper helper;
	helper.runTest("tests/trigger/resources/jeep_36_2_2_crank_only.csv", trigger_type_e::TT_JEEP_EVD_36_2_2);
}
