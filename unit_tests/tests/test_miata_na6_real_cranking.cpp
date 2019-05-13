/*
 * test_miata_na6_real_cranking.cpp
 *
 * @date May 11, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "engine_test_helper.h"

extern int timeNowUs;
extern WarningCodeState unitTestWarningCodeState;

static void fireTriggerEvent(EngineTestHelper*eth, double timestampS, int channel, bool isRise) {
	trigger_event_e event;
	if (channel == 0 && isRise == true) {
		event = SHAFT_PRIMARY_RISING;
	} else if (channel == 0 && isRise == false){
		event = SHAFT_PRIMARY_FALLING;
	} else 	if (channel == 0 && isRise == true) {
		event = SHAFT_SECONDARY_RISING;
	} else {
		event = SHAFT_SECONDARY_FALLING;
	}

	Engine *engine = &eth->engine;
	EXPAND_Engine;
	timeNowUs = 1000000 * timestampS;
	engine->triggerCentral.handleShaftSignal(event, engine, engine->engineConfigurationPtr, &eth->persistentConfig, boardConfiguration);
}

TEST(miataNA6, realCranking) {
	WITH_ENGINE_TEST_HELPER(MIATA_NA6_VAF);

#define EVENT(timestamp, channel, value) { fireTriggerEvent(&eth, timestamp, channel, value); }

	/*
	 * generates using logicdata2c tool
	 *
	 */

	/* 3 */ EVENT(/* timestamp*/0.597303, /*index*/0, /*value*/false);
	/* 4 */ EVENT(/* timestamp*/0.69078225, /*index*/0, /*value*/true);
	/* 5 */ EVENT(/* timestamp*/0.753783, /*index*/1, /*value*/true);
	/* 6 */ EVENT(/* timestamp*/0.79334275, /*index*/0, /*value*/false);
	/* 7 */ EVENT(/* timestamp*/0.85266675, /*index*/0, /*value*/true);
	/* 8 */ EVENT(/* timestamp*/0.93424725, /*index*/1, /*value*/true);
	/* 9 */ EVENT(/* timestamp*/0.9441, /*index*/0, /*value*/false);
	/* 10 */ EVENT(/* timestamp*/0.98209725, /*index*/1, /*value*/false);
	/* 11 */ EVENT(/* timestamp*/0.99523975, /*index*/0, /*value*/true);
	/* 12 */ EVENT(/* timestamp*/1.076422, /*index*/0, /*value*/false);
	/* 13 */ EVENT(/* timestamp*/1.125428, /*index*/0, /*value*/true);
	/* 14 */ EVENT(/* timestamp*/1.194742, /*index*/1, /*value*/true);
	/* 15 */ EVENT(/* timestamp*/1.20417975, /*index*/0, /*value*/false);
	/* 16 */ EVENT(/* timestamp*/1.25380075, /*index*/0, /*value*/true);
	/* 17 */ EVENT(/* timestamp*/1.30114225, /*index*/1, /*value*/true);
	/* 18 */ EVENT(/* timestamp*/1.3341915, /*index*/0, /*value*/false);
	/* 19 */ EVENT(/* timestamp*/1.383534, /*index*/0, /*value*/true);
	/* 22 */ EVENT(/* timestamp*/1.45352675, /*index*/1, /*value*/true);
	/* 23 */ EVENT(/* timestamp*/1.46291525, /*index*/0, /*value*/false);
	/* 25 */ EVENT(/* timestamp*/1.49939025, /*index*/1, /*value*/false);
	/* 27 */ EVENT(/* timestamp*/1.511785, /*index*/0, /*value*/true);
	/* 28 */ EVENT(/* timestamp*/1.5908545, /*index*/0, /*value*/false);
	/* 31 */ EVENT(/* timestamp*/1.6399845, /*index*/0, /*value*/true);
	/* 32 */ EVENT(/* timestamp*/1.70975875, /*index*/1, /*value*/true);
	/* 33 */ EVENT(/* timestamp*/1.7194455, /*index*/0, /*value*/false);
	/* 36 */ EVENT(/* timestamp*/1.7697125, /*index*/0, /*value*/true);
	/* 37 */ EVENT(/* timestamp*/1.817179, /*index*/1, /*value*/true);
	/* 38 */ EVENT(/* timestamp*/1.8511055, /*index*/0, /*value*/false);
	/* 41 */ EVENT(/* timestamp*/1.9011835, /*index*/0, /*value*/true);
	/* 42 */ EVENT(/* timestamp*/1.97691675, /*index*/1, /*value*/true);
	/* 43 */ EVENT(/* timestamp*/1.9822455, /*index*/0, /*value*/false);
	/* 44 */ EVENT(/* timestamp*/2.001249, /*index*/1, /*value*/false);
	/* 45 */ EVENT(/* timestamp*/2.0070235, /*index*/0, /*value*/true);
	/* 48 */ EVENT(/* timestamp*/2.04448175, /*index*/0, /*value*/false);
	/* 49 */ EVENT(/* timestamp*/2.06135875, /*index*/0, /*value*/true);
	/* 52 */ EVENT(/* timestamp*/2.08529325, /*index*/1, /*value*/true);
	/* 53 */ EVENT(/* timestamp*/2.089132, /*index*/0, /*value*/false);
	/* 54 */ EVENT(/* timestamp*/2.107152, /*index*/0, /*value*/true);
	/* 56 */ EVENT(/* timestamp*/2.12253625, /*index*/1, /*value*/true);
	/* 58 */ EVENT(/* timestamp*/2.136714, /*index*/0, /*value*/false);
	/* 59 */ EVENT(/* timestamp*/2.1560195, /*index*/0, /*value*/true);
	/* 60 */ EVENT(/* timestamp*/2.18365925, /*index*/1, /*value*/true);
	/* 61 */ EVENT(/* timestamp*/2.188138, /*index*/0, /*value*/false);
	/* 62 */ EVENT(/* timestamp*/2.20460875, /*index*/1, /*value*/false);
	/* 63 */ EVENT(/* timestamp*/2.20940075, /*index*/0, /*value*/true);
	/* 64 */ EVENT(/* timestamp*/2.2446445, /*index*/0, /*value*/false);
	/* 65 */ EVENT(/* timestamp*/2.26826475, /*index*/0, /*value*/true);
	/* 66 */ EVENT(/* timestamp*/2.30276125, /*index*/1, /*value*/true);
	/* 67 */ EVENT(/* timestamp*/2.308357, /*index*/0, /*value*/false);
	/* 68 */ EVENT(/* timestamp*/2.33551175, /*index*/0, /*value*/true);
	/* 69 */ EVENT(/* timestamp*/2.3601205, /*index*/1, /*value*/true);
	/* 70 */ EVENT(/* timestamp*/2.382952, /*index*/0, /*value*/false);
	/* 71 */ EVENT(/* timestamp*/2.41535525, /*index*/0, /*value*/true);
	/* 74 */ EVENT(/* timestamp*/2.45321725, /*index*/1, /*value*/true);
	/* 75 */ EVENT(/* timestamp*/2.4577005, /*index*/0, /*value*/false);
	/* 76 */ EVENT(/* timestamp*/2.47348125, /*index*/1, /*value*/false);
	/* 77 */ EVENT(/* timestamp*/2.47796125, /*index*/0, /*value*/true);
	/* 80 */ EVENT(/* timestamp*/2.51159275, /*index*/0, /*value*/false);
	/* 81 */ EVENT(/* timestamp*/2.533664, /*index*/0, /*value*/true);
	/* 84 */ EVENT(/* timestamp*/2.56564675, /*index*/1, /*value*/true);
	/* 85 */ EVENT(/* timestamp*/2.57087425, /*index*/0, /*value*/false);
	/* 86 */ EVENT(/* timestamp*/2.59612075, /*index*/0, /*value*/true);
	/* 89 */ EVENT(/* timestamp*/2.61853375, /*index*/1, /*value*/true);
	/* 90 */ EVENT(/* timestamp*/2.6365195, /*index*/0, /*value*/false);
	/* 93 */ EVENT(/* timestamp*/2.6573605, /*index*/0, /*value*/true);
	/* 94 */ EVENT(/* timestamp*/2.68447975, /*index*/1, /*value*/true);
	/* 95 */ EVENT(/* timestamp*/2.68829075, /*index*/0, /*value*/false);
	/* 96 */ EVENT(/* timestamp*/2.701797, /*index*/1, /*value*/false);
	/* 98 */ EVENT(/* timestamp*/2.705553, /*index*/0, /*value*/true);
	/* 100 */ EVENT(/* timestamp*/2.73171175, /*index*/0, /*value*/false);
	/* 101 */ EVENT(/* timestamp*/2.7466575, /*index*/0, /*value*/true);
	/* 104 */ EVENT(/* timestamp*/2.76736375, /*index*/1, /*value*/true);
	/* 105 */ EVENT(/* timestamp*/2.770612, /*index*/0, /*value*/false);
	/* 106 */ EVENT(/* timestamp*/2.7848735, /*index*/0, /*value*/true);
	/* 109 */ EVENT(/* timestamp*/2.79669, /*index*/1, /*value*/true);
	/* 110 */ EVENT(/* timestamp*/2.808129, /*index*/0, /*value*/false);
	/* 113 */ EVENT(/* timestamp*/2.8232045, /*index*/0, /*value*/true);
	/* 114 */ EVENT(/* timestamp*/2.84438125, /*index*/1, /*value*/true);
	/* 115 */ EVENT(/* timestamp*/2.847948, /*index*/0, /*value*/false);
	/* 117 */ EVENT(/* timestamp*/2.86067075, /*index*/1, /*value*/false);
	/* 119 */ EVENT(/* timestamp*/2.8642345, /*index*/0, /*value*/true);
	/* 120 */ EVENT(/* timestamp*/2.89112225, /*index*/0, /*value*/false);
	/* 123 */ EVENT(/* timestamp*/2.9089625, /*index*/0, /*value*/true);
	/* 124 */ EVENT(/* timestamp*/2.93429275, /*index*/1, /*value*/true);
	/* 125 */ EVENT(/* timestamp*/2.93850475, /*index*/0, /*value*/false);
	/* 128 */ EVENT(/* timestamp*/2.958108, /*index*/0, /*value*/true);
	/* 129 */ EVENT(/* timestamp*/2.974461, /*index*/1, /*value*/true);
	/* 130 */ EVENT(/* timestamp*/2.98846325, /*index*/0, /*value*/false);
	/* 133 */ EVENT(/* timestamp*/3.00650825, /*index*/0, /*value*/true);
	/* 134 */ EVENT(/* timestamp*/3.031735, /*index*/1, /*value*/true);


	ASSERT_EQ( 3,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#realCranking";
//	ASSERT_EQ(CUSTOM_SYNC_COUNT_MISMATCH, unitTestWarningCodeState.recentWarnings.get(0)) << "@0";


	ASSERT_EQ( 2401,  GET_RPM()) << "RPM at the end";

}

