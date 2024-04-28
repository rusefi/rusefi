#include "pch.h"
#include "logicdata_csv_reader.h"

static void testNoOverdwell(const char* file, bool instantRpm, const int expectedBailedOnDwellCount) {
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open(file);
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = instantRpm;

	engineConfiguration->cylindersCount = 6;
	engineConfiguration->firingOrder = FO_1_5_3_6_2_4;
	engineConfiguration->globalTriggerAngleOffset = 155;
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;

	// engineConfiguration->cylindersCount = 1;
	// engineConfiguration->firingOrder = FO_1;
	// engineConfiguration->globalTriggerAngleOffset = 35;

	engineConfiguration->timingMode = TM_FIXED;
	engineConfiguration->fixedTiming = 14.69;

	engineConfiguration->trigger.customTotalToothCount = 36;
	engineConfiguration->trigger.customSkippedToothCount = 2;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL);

	engineConfiguration->isIgnitionEnabled = true;
	engineConfiguration->isInjectionEnabled = false;

	std::vector<efitick_t> coilStartTimes(12);

	engine->onIgnitionEvent = [&](IgnitionEvent* event, bool state) {
	    efitick_t startTime = coilStartTimes[event->coilIndex];

		if (state) {
			coilStartTimes[event->coilIndex] = getTimeNowNt();
		} else if (startTime == 0) {
		  printf("Interesting: goes LOW without ever going HIGH %d\n", event->coilIndex);
		} else {
			auto actualDwell = 1e-3 * NT2USF(getTimeNowNt() - startTime);
			coilStartTimes[event->coilIndex] = 0;

			EXPECT_LT(actualDwell, 50) << "Overdwell on cylinder " << (int)event->coilIndex << " of " << actualDwell << " ms";
		}
	};

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	ASSERT_EQ(expectedBailedOnDwellCount, engine->getBailedOnDwellCount())
		<< "Please check if our dwell algorithm have really got better.";
}

// noisy-trigger-min.csv contains lines 53-61 from noisy-trigger-1.csv - minimal subset to reproduce the single
// bailed-on dwell
TEST(RealNoisyTrigger, AvoidOverdwellMinNoInstant) {
	testNoOverdwell("tests/trigger/resources/noisy-trigger-min.csv", false, 1);
}

TEST(RealNoisyTrigger, AvoidOverdwellMinWithInstant) {
	testNoOverdwell("tests/trigger/resources/noisy-trigger-min.csv", true, 1);
}

TEST(RealNoisyTrigger, AvoidOverdwell1NoInstant) {
	testNoOverdwell("tests/trigger/resources/noisy-trigger-1.csv", false, 25);
}

TEST(RealNoisyTrigger, AvoidOverdwell1WithInstant) {
	testNoOverdwell("tests/trigger/resources/noisy-trigger-1.csv", true, 23);
}

TEST(RealNoisyTrigger, AvoidOverdwell2NoInstant) {
	testNoOverdwell("tests/trigger/resources/noisy-trigger-2.csv", false, 89);
}

TEST(RealNoisyTrigger, AvoidOverdwell2WithInstant) {
	testNoOverdwell("tests/trigger/resources/noisy-trigger-2.csv", true, 85);
}

TEST(RealNoisyTrigger, AvoidOverdwell3NoInstant) {
	testNoOverdwell("tests/trigger/resources/noisy-trigger-3.csv", false, 25);
}

TEST(RealNoisyTrigger, AvoidOverdwell3WithInstant) {
	testNoOverdwell("tests/trigger/resources/noisy-trigger-3.csv", true, 22);
}
