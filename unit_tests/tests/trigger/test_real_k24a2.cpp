#include "pch.h"
#include "real_trigger_helper.h"

static void doTest(const char* testFile, int expectedRpm, float expectedInstantRpmRange) {

	RealTriggerHelper helper;
	helper.runTest(testFile, trigger_type_e::TT_HONDA_K_CRANK_12_1);

	ASSERT_EQ(0u, helper.eth.recentWarnings()->getCount()) << "warningCounter#vwRealCranking";
	ASSERT_EQ(expectedRpm, round(Sensor::getOrZero(SensorType::Rpm)));

	// real cranking is uneven: instant RPM range over the last complete engine
	// cycle captures how much the crank sped up/slowed down within that cycle.
	// No-plugs cranking has no compression oscillation, hence the small values.
	EXPECT_NEAR(expectedInstantRpmRange, engine->triggerCentral.instantRpm.getInstantRpmRange(), 0.1);
}

TEST(realk24, crankingNoPlugs1) {
	doTest("tests/trigger/resources/cranking_honda_k24a2_no_plugs.csv", 188, 2.71f);
}

TEST(realk24, crankingNoPlugs2) {
	doTest("tests/trigger/resources/cranking_honda_k24a2_no_plugs_2.csv", 186, 2.35f);
}
