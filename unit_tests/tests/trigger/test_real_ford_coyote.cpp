#include "pch.h"
#include "real_trigger_helper.h"

static void runCoyoteIntakeCam(bool invertPrimaryTriggerSignal, uint32_t warningCount, int rpm) {
	RealTriggerHelper helper;
	extern bool unitTestTaskNoFastCallWhileAdvancingTimeHack;
	unitTestTaskNoFastCallWhileAdvancingTimeHack = true;

	printf("Reading intake %d...\n", invertPrimaryTriggerSignal);
	helper.runTest("tests/trigger/resources/ford-coyote-intake-cam.csv", trigger_type_e::TT_VVT_FORD_COYOTE, invertPrimaryTriggerSignal);

	ASSERT_EQ(warningCount, helper.eth.recentWarnings()->getCount()) << "warningCounter#intakeCam";
	ASSERT_EQ(rpm, round(Sensor::getOrZero(SensorType::Rpm)));
}

TEST(fordCoyote, intakeCam) {
	runCoyoteIntakeCam(false, 1, 1093);
}

static void runCoyoteExhaustCam(bool invertPrimaryTriggerSignal, uint32_t warningCount, int rpm) {
	RealTriggerHelper helper;
	extern bool unitTestTaskNoFastCallWhileAdvancingTimeHack;
	unitTestTaskNoFastCallWhileAdvancingTimeHack = true;

	printf("Reading exhaust...\n");
	helper.runTest("tests/trigger/resources/ford-coyote-exhaust-cam.csv", trigger_type_e::TT_VVT_FORD_COYOTE, invertPrimaryTriggerSignal);

	ASSERT_EQ(warningCount, helper.eth.recentWarnings()->getCount()) << "warningCounter#exhaustCam";
	ASSERT_EQ(rpm, round(Sensor::getOrZero(SensorType::Rpm)));
}

TEST(fordCoyote, exhaustCam) {
	runCoyoteExhaustCam(false, 1, 1093);
}

TEST(fordCoyote, exhaustCamInverted) {
	extern bool unitTestTaskPrecisionHack;
	unitTestTaskPrecisionHack = true;
	// C9002, C9007, C9008
	runCoyoteExhaustCam(true, 3, 1046);
}
