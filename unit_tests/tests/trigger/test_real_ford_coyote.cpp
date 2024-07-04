#include "pch.h"

#include "logicdata_csv_reader.h"

static void runCoyoteIntakeCam(bool invertPrimaryTriggerSignal, int warningCount, int rpm) {
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/ford-coyote-intake-cam.csv");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(invertPrimaryTriggerSignal ? trigger_type_e::TT_DEV : trigger_type_e::TT_VVT_FORD_COYOTE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;
	reader.flipOnRead = invertPrimaryTriggerSignal;


  printf("Reading intake %d...\n", invertPrimaryTriggerSignal);
	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	ASSERT_EQ( warningCount, eth.recentWarnings()->getCount())<< "warningCounter#intakeCam";
	ASSERT_EQ( rpm, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
}

TEST(fordCoyote, intakeCam) {
  runCoyoteIntakeCam(false, 1, 1093);
}

TEST(fordCoyote, intakeCamInverted) {
  runCoyoteIntakeCam(true, 3, 1018);
}

static void runCoyoteExhaustCam(bool invertPrimaryTriggerSignal, int warningCount, int rpm) {
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/ford-coyote-exhaust-cam.csv");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(invertPrimaryTriggerSignal ? trigger_type_e::TT_DEV : trigger_type_e::TT_VVT_FORD_COYOTE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	reader.flipOnRead = invertPrimaryTriggerSignal;
	engineConfiguration->alwaysInstantRpm = true;

	  printf("Reading exhaust...\n");
	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	ASSERT_EQ(warningCount, eth.recentWarnings()->getCount())<< "warningCounter#exhaustCam";
	ASSERT_EQ(rpm, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
}

TEST(fordCoyote, exhaustCam) {
  runCoyoteExhaustCam(false, 1, 1093);
}

TEST(fordCoyote, exhaustCamInverted) {
  runCoyoteExhaustCam(true, 2, 1046);
}
