#include "pch.h"

#include "engine_csv_reader.h"

TEST(realJeepEva, cranking) {
	CsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/jeep_36_2_2_crank_only.csv");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	//setVerboseTrigger(true);
	eth.setTriggerType(trigger_type_e::TT_JEEP_EVD_36_2_2);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	//engineConfiguration->isFasterEngineSpinUpEnabled = false;
	//engineConfiguration->alwaysInstantRpm = true;
	//engineConfiguration->invertPrimaryTriggerSignal = false;

	int n = 0;
	while (reader.haveMore()) {
		reader.processLine(&eth);
		//printf("%5d: RPM %f\n", n++, Sensor::getOrZero(SensorType::Rpm));
		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		//if (rpm) {
		//	printf("Got first RPM %f, at %d\n", rpm, n);
		//}
		n++;
	}
}
