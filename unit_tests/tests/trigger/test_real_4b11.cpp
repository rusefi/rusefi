// Mitsubishi 4B11 trigger pattern
// Crank: 36-2-1
// Cam: Single tooth (half moon)

#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(real4b11, running) {
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/4b11-running.csv");
	EngineTestHelper eth(TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	eth.setTriggerType(TT_36_2_1);

	int eventCount = 0;
	bool gotRpm = false;

	while (reader.haveMore()) {
		reader.processLine(&eth);
		eventCount++;
		engine->rpmCalculator.onSlowCallback();

		// Expect that all teeth are in the correct spot
		float angleError = getTriggerCentral()->triggerToothAngleError;
		EXPECT_TRUE(angleError < 3 && angleError > -3) << "tooth angle of " << angleError << " at timestamp " << (getTimeNowNt() / 1e8);

		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if (!gotRpm && rpm) {
			gotRpm = true;

			// We should get first RPM on exactly the first sync point - this means the instant RPM pre-sync event copy all worked OK
			EXPECT_EQ(eventCount, 30);
			EXPECT_NEAR(rpm, 1436.23f, 0.1);
		}
	}

	ASSERT_EQ(0, eth.recentWarnings()->getCount());
}
