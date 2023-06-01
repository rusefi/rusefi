#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(crankingGm24x, gmRealCrankingFromFile) {
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/gm_24x_cranking.csv");
	EngineTestHelper eth(TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	eth.setTriggerType(trigger_type_e::TT_GM_24x);

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
			EXPECT_EQ(eventCount, 23);
			EXPECT_NEAR(rpm, 77.0f, 0.1);
		}
	}

	ASSERT_EQ( 0, eth.recentWarnings()->getCount())<< "warningCounter#vwRealCranking";
	ASSERT_EQ( 139, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
}
