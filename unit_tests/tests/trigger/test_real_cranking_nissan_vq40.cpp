/*
 * @file test_real_cranking_nissan_vq40.cpp
 *
 * @date Jul 21, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "engine_test_helper.h"
#include "logicdata_csv_reader.h"

TEST(realCrankingVQ40, normalCranking) {
	CsvReader reader(1, /* vvtCount */ 2);
	int indeces[] = {0};

	reader.open("tests/trigger/resources/nissan_vq40_cranking-1.csv", indeces);
	WITH_ENGINE_TEST_HELPER (HELLEN_121_NISSAN);

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	EXPECT_NEAR(engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/0), -46.817, 1e-2);
	EXPECT_NEAR(engine->triggerCentral.getVVTPosition(/*bankIndex*/1, /*camIndex*/0), -47.411, 1e-2);
	ASSERT_EQ(241, GET_RPM())<< reader.lineIndex();

	// TODO: why warnings?
	ASSERT_EQ(2, eth.recentWarnings()->getCount());
}
