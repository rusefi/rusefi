#include "pch.h"

#include "engine_csv_reader.h"

TEST(realBQS, realHarleyCranking) {
  EngineCsvReader reader(/*triggerCount*/1, /* vvtCount */ 0);
  reader.setReadingOffset(1);
	reader.open("tests/ignition_injection/resources/hd-req-sync_3.csv");
  reader.setFlipOnRead(true);

	EngineTestHelper eth(engine_type_e::ET_BOSCH_QUICK_START);

	while (reader.haveMore()) {
 		reader.processLine(&eth);

		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if (reader.gotRpm) {
			  ASSERT_TRUE(Sensor::get(SensorType::Rpm).Valid);
		}
    reader.assertFirstRpm(184, 163);
  }
  ASSERT_TRUE(Sensor::get(SensorType::Rpm).Valid);

}

TEST(realBQS, readAsPrimarySensor) {
  EngineCsvReader reader(/*triggerCount*/1, /* vvtCount */ 0);
  reader.open("tests/trigger/resources/BQS-longer.csv");
  reader.setFlipOnRead(true);

	EngineTestHelper eth(engine_type_e::ET_BOSCH_QUICK_START);

	while (reader.haveMore()) {
 		reader.processLine(&eth);
 		reader.assertFirstRpm(2036, 13);
  }
  ASSERT_TRUE(Sensor::get(SensorType::Rpm).Valid);
  ASSERT_NEAR(Sensor::get(SensorType::Rpm).Value, 1874, 1);

}

TEST(realBQS, readAsCam) {
	EngineTestHelper eth(engine_type_e::ET_TEST_WITH_BOSCH_QUICK_START);

	hwHandleShaftSignal(0, true,  0);
	hwHandleShaftSignal(0, false, 1000000);
	hwHandleShaftSignal(0, true, 2000000);
	hwHandleShaftSignal(0, false, 3000000);
	ASSERT_EQ(3000, Sensor::getOrZero(SensorType::Rpm));

	  CsvReader reader(/*triggerCount*/0, /* vvtCount */ 1);
	  reader.open("tests/trigger/resources/BQS-longer.csv");
	  reader.flipVvtOnRead = true;

	TriggerCentral *tc = getTriggerCentral();
	ASSERT_TRUE(tc->triggerState.getShaftSynchronized());
	TriggerDecoderBase& vvtDecoder = tc->vvtState[0][0];

	bool gotVvt = false;
	while (reader.haveMore()) {
 		reader.processLine(&eth);

		if (!gotVvt && vvtDecoder.getShaftSynchronized()) {
			gotVvt = true;
			EXPECT_EQ(reader.lineIndex(), 13);
		}
	}
	ASSERT_DOUBLE_EQ(-247.03125, tc->getVVTPosition(0, 0));
	ASSERT_TRUE(vvtDecoder.getShaftSynchronized());
}
