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
