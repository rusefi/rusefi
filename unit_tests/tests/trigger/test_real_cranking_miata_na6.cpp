/*
 * test_real_cranking_miata_na6.cpp
 *
 * @date May 11, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "logicdata_csv_reader.h"

extern int timeNowUs;
extern WarningCodeState unitTestWarningCodeState;

static void fireTriggerEvent(EngineTestHelper*eth, double timestampS, trigger_wheel_e channel, bool isFall) {
	trigger_event_e event;
	// in this trigger data file second channel is the primary
	// interesting how logic analyzer and trigger are flipped - logical '1' from logicdata file seems to be 'falling'
	// for trigger definition?!
	if (channel == T_PRIMARY && isFall == false) {
		event = SHAFT_PRIMARY_RISING;
	} else if (channel == T_PRIMARY && isFall == true) {
		event = SHAFT_PRIMARY_FALLING;
	} else 	if (channel == T_SECONDARY && isFall == false) {
		event = SHAFT_SECONDARY_RISING;
	} else {
		event = SHAFT_SECONDARY_FALLING;
	}

	Engine *engine = &eth->engine;
	EXPAND_Engine;
	timeNowUs = 1'000'000 * timestampS;
	printf("MIATANA: posting time=%d event=%d\n", timeNowUs, event);
	hwHandleShaftSignal((int)channel, !isFall, getTimeNowNt() PASS_ENGINE_PARAMETER_SUFFIX);
}

TEST(cranking, hardcodedRealCranking) {
	WITH_ENGINE_TEST_HELPER(FRANKENSO_MIATA_NA6_VAF);

#define EVENT(timestamp, channel, value) { fireTriggerEvent(&eth, timestamp, channel, value); }

	/*
	 * Generated using logicdata2c tool
	 */
	// looks like na_cranking4.logicdata file

	/* 3 */ EVENT(/* timestamp*/0.597303, T_SECONDARY, /*value*/false);
	/* 4 */ EVENT(/* timestamp*/0.69078225, T_SECONDARY, /*value*/true);
	/* 5 */ EVENT(/* timestamp*/0.753783, T_PRIMARY, /*value*/true);
	/* 6 */ EVENT(/* timestamp*/0.79334275, T_SECONDARY, /*value*/false);
	/* 7 */ EVENT(/* timestamp*/0.85266675, T_SECONDARY, /*value*/true);
	/* 8 */ EVENT(/* timestamp*/0.93424725, T_PRIMARY, /*value*/true);
	/* 9 */ EVENT(/* timestamp*/0.9441, T_SECONDARY, /*value*/false);
	/* 10 */ EVENT(/* timestamp*/0.98209725, T_PRIMARY, /*value*/false);
	/* 11 */ EVENT(/* timestamp*/0.99523975, T_SECONDARY, /*value*/true);
	/* 12 */ EVENT(/* timestamp*/1.076422, T_SECONDARY, /*value*/false);
	/* 13 */ EVENT(/* timestamp*/1.125428, T_SECONDARY, /*value*/true);
	/* 14 */ EVENT(/* timestamp*/1.194742, T_PRIMARY, /*value*/true);
	/* 15 */ EVENT(/* timestamp*/1.20417975, T_SECONDARY, /*value*/false);
	/* 16 */ EVENT(/* timestamp*/1.25380075, T_SECONDARY, /*value*/true);
	/* 17 */ EVENT(/* timestamp*/1.30114225, T_PRIMARY, /*value*/true);
	/* 18 */ EVENT(/* timestamp*/1.3341915, T_SECONDARY, /*value*/false);
	/* 19 */ EVENT(/* timestamp*/1.383534, T_SECONDARY, /*value*/true);
	/* 22 */ EVENT(/* timestamp*/1.45352675, T_PRIMARY, /*value*/true);
	/* 23 */ EVENT(/* timestamp*/1.46291525, T_SECONDARY, /*value*/false);
	EXPECT_EQ(0, GET_RPM());
	// ^^ All teeth above are pre-sync ^^

	// THIS TOOTH IS SYNC POINT!
	// Should get instantRpm from here on
	/* 25 */ EVENT(/* timestamp*/1.49939025, T_PRIMARY, /*value*/false);
	EXPECT_EQ(239, GET_RPM());
	/* 27 */ EVENT(/* timestamp*/1.511785, T_SECONDARY, /*value*/true);
	EXPECT_EQ(234, GET_RPM());
	/* 28 */ EVENT(/* timestamp*/1.5908545, T_SECONDARY, /*value*/false);
	EXPECT_EQ(231, GET_RPM());
	/* 31 */ EVENT(/* timestamp*/1.6399845, T_SECONDARY, /*value*/true);
	EXPECT_EQ(234, GET_RPM());
	/* 32 */ EVENT(/* timestamp*/1.70975875, T_PRIMARY, /*value*/true);
	EXPECT_EQ(225, GET_RPM());
	/* 33 */ EVENT(/* timestamp*/1.7194455, T_SECONDARY, /*value*/false);
	EXPECT_EQ(231, GET_RPM());
	/* 36 */ EVENT(/* timestamp*/1.7697125, T_SECONDARY, /*value*/true);
	EXPECT_EQ(231, GET_RPM());
	/* 37 */ EVENT(/* timestamp*/1.817179, T_PRIMARY, /*value*/true);
	EXPECT_EQ(217, GET_RPM());
	/* 38 */ EVENT(/* timestamp*/1.8511055, T_SECONDARY, /*value*/false);
	EXPECT_EQ(225, GET_RPM());
	/* 41 */ EVENT(/* timestamp*/1.9011835, T_SECONDARY, /*value*/true);
	EXPECT_EQ(243, GET_RPM());
	/* 42 */ EVENT(/* timestamp*/1.97691675, T_PRIMARY, /*value*/true);
	EXPECT_EQ(207, GET_RPM());
	/* 43 */ EVENT(/* timestamp*/1.9822455, T_SECONDARY, /*value*/false);
	EXPECT_EQ(226, GET_RPM());

	// Second sync point, should transition to non-instant RPM
	/* 44 */ EVENT(/* timestamp*/2.001249, T_PRIMARY, /*value*/false);
	EXPECT_EQ(239, GET_RPM());
	/* 45 */ EVENT(/* timestamp*/2.0070235, T_SECONDARY, /*value*/true);
	/* 48 */ EVENT(/* timestamp*/2.04448175, T_SECONDARY, /*value*/false);
	/* 49 */ EVENT(/* timestamp*/2.06135875, T_SECONDARY, /*value*/true);
	/* 52 */ EVENT(/* timestamp*/2.08529325, T_PRIMARY, /*value*/true);
	/* 53 */ EVENT(/* timestamp*/2.089132, T_SECONDARY, /*value*/false);
	/* 54 */ EVENT(/* timestamp*/2.107152, T_SECONDARY, /*value*/true);
	/* 56 */ EVENT(/* timestamp*/2.12253625, T_PRIMARY, /*value*/true);
	/* 58 */ EVENT(/* timestamp*/2.136714, T_SECONDARY, /*value*/false);
	/* 59 */ EVENT(/* timestamp*/2.1560195, T_SECONDARY, /*value*/true);
	/* 60 */ EVENT(/* timestamp*/2.18365925, T_PRIMARY, /*value*/true);
	/* 61 */ EVENT(/* timestamp*/2.188138, T_SECONDARY, /*value*/false);

	// rpm should now only update at sync point
	EXPECT_EQ(239, GET_RPM());
	// Third sync point
	/* 62 */ EVENT(/* timestamp*/2.20460875, T_PRIMARY, /*value*/false);
	EXPECT_EQ(590, GET_RPM());

	/* 63 */ EVENT(/* timestamp*/2.20940075, T_SECONDARY, /*value*/true);
	/* 64 */ EVENT(/* timestamp*/2.2446445, T_SECONDARY, /*value*/false);
	/* 65 */ EVENT(/* timestamp*/2.26826475, T_SECONDARY, /*value*/true);
	/* 66 */ EVENT(/* timestamp*/2.30276125, T_PRIMARY, /*value*/true);
	/* 67 */ EVENT(/* timestamp*/2.308357, T_SECONDARY, /*value*/false);
	/* 68 */ EVENT(/* timestamp*/2.33551175, T_SECONDARY, /*value*/true);
	/* 69 */ EVENT(/* timestamp*/2.3601205, T_PRIMARY, /*value*/true);
	/* 70 */ EVENT(/* timestamp*/2.382952, T_SECONDARY, /*value*/false);
	/* 71 */ EVENT(/* timestamp*/2.41535525, T_SECONDARY, /*value*/true);
	/* 74 */ EVENT(/* timestamp*/2.45321725, T_PRIMARY, /*value*/true);
	/* 75 */ EVENT(/* timestamp*/2.4577005, T_SECONDARY, /*value*/false);
	/* 76 */ EVENT(/* timestamp*/2.47348125, T_PRIMARY, /*value*/false);
	/* 77 */ EVENT(/* timestamp*/2.47796125, T_SECONDARY, /*value*/true);
	/* 80 */ EVENT(/* timestamp*/2.51159275, T_SECONDARY, /*value*/false);
	/* 81 */ EVENT(/* timestamp*/2.533664, T_SECONDARY, /*value*/true);
	/* 84 */ EVENT(/* timestamp*/2.56564675, T_PRIMARY, /*value*/true);
	/* 85 */ EVENT(/* timestamp*/2.57087425, T_SECONDARY, /*value*/false);
	/* 86 */ EVENT(/* timestamp*/2.59612075, T_SECONDARY, /*value*/true);
	/* 89 */ EVENT(/* timestamp*/2.61853375, T_PRIMARY, /*value*/true);
	/* 90 */ EVENT(/* timestamp*/2.6365195, T_SECONDARY, /*value*/false);
	/* 93 */ EVENT(/* timestamp*/2.6573605, T_SECONDARY, /*value*/true);
	/* 94 */ EVENT(/* timestamp*/2.68447975, T_PRIMARY, /*value*/true);
	/* 95 */ EVENT(/* timestamp*/2.68829075, T_SECONDARY, /*value*/false);
	/* 96 */ EVENT(/* timestamp*/2.701797, T_PRIMARY, /*value*/false);
	/* 98 */ EVENT(/* timestamp*/2.705553, T_SECONDARY, /*value*/true);
	/* 100 */ EVENT(/* timestamp*/2.73171175, T_SECONDARY, /*value*/false);
	/* 101 */ EVENT(/* timestamp*/2.7466575, T_SECONDARY, /*value*/true);
	/* 104 */ EVENT(/* timestamp*/2.76736375, T_PRIMARY, /*value*/true);
	/* 105 */ EVENT(/* timestamp*/2.770612, T_SECONDARY, /*value*/false);
	/* 106 */ EVENT(/* timestamp*/2.7848735, T_SECONDARY, /*value*/true);
	/* 109 */ EVENT(/* timestamp*/2.79669, T_PRIMARY, /*value*/true);
	/* 110 */ EVENT(/* timestamp*/2.808129, T_SECONDARY, /*value*/false);
	/* 113 */ EVENT(/* timestamp*/2.8232045, T_SECONDARY, /*value*/true);
	/* 114 */ EVENT(/* timestamp*/2.84438125, T_PRIMARY, /*value*/true);
	/* 115 */ EVENT(/* timestamp*/2.847948, T_SECONDARY, /*value*/false);
	/* 117 */ EVENT(/* timestamp*/2.86067075, T_PRIMARY, /*value*/false);
	/* 119 */ EVENT(/* timestamp*/2.8642345, T_SECONDARY, /*value*/true);
	/* 120 */ EVENT(/* timestamp*/2.89112225, T_SECONDARY, /*value*/false);
	/* 123 */ EVENT(/* timestamp*/2.9089625, T_SECONDARY, /*value*/true);
	/* 124 */ EVENT(/* timestamp*/2.93429275, T_PRIMARY, /*value*/true);
	/* 125 */ EVENT(/* timestamp*/2.93850475, T_SECONDARY, /*value*/false);
	/* 128 */ EVENT(/* timestamp*/2.958108, T_SECONDARY, /*value*/true);
	/* 129 */ EVENT(/* timestamp*/2.974461, T_PRIMARY, /*value*/true);
	/* 130 */ EVENT(/* timestamp*/2.98846325, T_SECONDARY, /*value*/false);
	/* 133 */ EVENT(/* timestamp*/3.00650825, T_SECONDARY, /*value*/true);
	/* 134 */ EVENT(/* timestamp*/3.031735, T_PRIMARY, /*value*/true);

	EXPECT_EQ( 0,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#realCranking";

	EXPECT_EQ(755,  GET_RPM()) << "RPM at the end";
}

TEST(cranking, naCrankFromFile) {
	CsvReader reader(2, /* vvtCount */ 0);
	int indeces[2] = {1, 0};
	reader.open("tests/trigger/resources/cranking_na_4.csv", indeces);

	WITH_ENGINE_TEST_HELPER(FRANKENSO_MIATA_NA6_VAF);

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	EXPECT_EQ(0, eth.recentWarnings()->getCount());
	EXPECT_EQ(698, GET_RPM());
}
