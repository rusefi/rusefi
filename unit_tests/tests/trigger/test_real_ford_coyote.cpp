#include "pch.h"
#include "real_trigger_helper.h"

static void runCoyoteIntakeCam(bool invertPrimaryTriggerSignal, uint32_t warningCount, int rpm) {
	RealTriggerHelper helper;


	printf("Reading intake %d...\n", invertPrimaryTriggerSignal);
	helper.runTest("tests/trigger/resources/ford-coyote-intake-cam.csv", trigger_type_e::TT_VVT_FORD_COYOTE, invertPrimaryTriggerSignal);

	ASSERT_EQ(warningCount, helper.eth.recentWarnings()->getCount()) << "warningCounter#intakeCam";
	ASSERT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_OBD_impossibly_short_INJECTION));
	ASSERT_EQ(rpm, round(Sensor::getOrZero(SensorType::Rpm)));
}

TEST(fordCoyote, intakeCam) {
	// todo: address later: one warning is a false positive CUSTOM_OBD_impossibly_short_INJECTION - injection events run with zero fuel mass in this test
	runCoyoteIntakeCam(false, 2, 1093);
}

static void runCoyoteExhaustCam(bool invertPrimaryTriggerSignal, uint32_t warningCount, int rpm) {
	RealTriggerHelper helper;

	printf("Reading exhaust...\n");
	helper.runTest("tests/trigger/resources/ford-coyote-exhaust-cam.csv", trigger_type_e::TT_VVT_FORD_COYOTE, invertPrimaryTriggerSignal);

	ASSERT_EQ(warningCount, helper.eth.recentWarnings()->getCount()) << "warningCounter#exhaustCam";
	ASSERT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_OBD_impossibly_short_INJECTION));
	ASSERT_EQ(rpm, round(Sensor::getOrZero(SensorType::Rpm)));
}

TEST(fordCoyote, exhaustCam) {
	// todo: address later: one warning is a false positive CUSTOM_OBD_impossibly_short_INJECTION - injection events run with zero fuel mass in this test
	runCoyoteExhaustCam(false, 2, 1093);
}

TEST(fordCoyote, exhaustCamInverted) {
	// C9002, C9007, C9008
	// todo: address later: +1 is a false positive CUSTOM_OBD_impossibly_short_INJECTION - injection events run with zero fuel mass in this test
	runCoyoteExhaustCam(true, 4, 1046);
}
