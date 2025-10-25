#include "pch.h"

#include "logicdata_csv_reader.h"

static void runCoyoteIntakeCam(bool invertPrimaryTriggerSignal, uint32_t warningCount, int rpm) {
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/ford-coyote-intake-cam.csv");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_VVT_FORD_COYOTE);
	extern bool unitTestTaskNoFastCallWhileAdvancingTimeHack;
	unitTestTaskNoFastCallWhileAdvancingTimeHack = true;

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

static void runCoyoteExhaustCam(bool invertPrimaryTriggerSignal, uint32_t warningCount, int rpm) {
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/ford-coyote-exhaust-cam.csv");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_VVT_FORD_COYOTE);
	extern bool unitTestTaskNoFastCallWhileAdvancingTimeHack;
	unitTestTaskNoFastCallWhileAdvancingTimeHack = true;

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
	extern bool unitTestTaskPrecisionHack;
	unitTestTaskPrecisionHack = true;
	// C9002, C9007, C9008
	runCoyoteExhaustCam(true, 3, 1046);
}
