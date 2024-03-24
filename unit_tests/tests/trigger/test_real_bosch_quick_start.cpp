#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(realBQS, readAsPrimarySensor) {
  CsvReader reader(/*triggerCount*/1, /* vvtCount */ 0);
  reader.open("tests/trigger/resources/BQS-longer.csv");
  reader.flipOnRead = true;

	EngineTestHelper eth(engine_type_e::ET_BOSCH_QUICK_START);

	int eventCount = 0;
	bool gotRpm = false;
	while (reader.haveMore()) {
 		reader.processLine(&eth);
		eventCount++;
		engine->rpmCalculator.onSlowCallback();

		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if (!gotRpm && rpm) {
			gotRpm = true;

			EXPECT_EQ(eventCount, 13);
			EXPECT_NEAR(rpm, 2035.53466f, 0.1);
			break;
		}

  }
  ASSERT_TRUE(Sensor::get(SensorType::Rpm).Valid);
  ASSERT_FLOAT_EQ(Sensor::get(SensorType::Rpm).Value, 2035.53467);

}

TEST(realBQS, readAsCam) {
	EngineTestHelper eth(engine_type_e::ET_TEST_WITH_BOSCH_QUICK_START);

	hwHandleShaftSignal(0, true,  0);
	hwHandleShaftSignal(0, false, 1000000);
	hwHandleShaftSignal(0, true, 2000000);
	hwHandleShaftSignal(0, false, 3000000);
	eth.assertRpm(3000);

	int eventCount = 0;
	  CsvReader reader(/*triggerCount*/0, /* vvtCount */ 1);
	  reader.open("tests/trigger/resources/BQS-longer.csv");
	  reader.flipVvtOnRead = true;

	TriggerCentral *tc = getTriggerCentral();
	ASSERT_TRUE(tc->triggerState.getShaftSynchronized());
	TriggerDecoderBase& vvtDecoder = tc->vvtState[0][0];

	bool gotVvt = false;
	while (reader.haveMore()) {
 		reader.processLine(&eth);
		eventCount++;

		if (!gotVvt && vvtDecoder.getShaftSynchronized()) {
			gotVvt = true;
			EXPECT_EQ(eventCount, 13);
		}
	}
	ASSERT_DOUBLE_EQ(-247.03125, tc->getVVTPosition(0, 0));
	ASSERT_TRUE(vvtDecoder.getShaftSynchronized());
}
