/*
 * @file test_maf2map.cpp
 *
 * @date Jan 13, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "engine_test_helper.h"
#include "maf2map.h"

#define round_2d(x) efiRound(x, 0.01)

TEST(maf2map, util) {

	WITH_ENGINE_TEST_HELPER(FORD_ASPIRE_1996);

	engineConfiguration->fuelAlgorithm = LM_REAL_MAF;

	ASSERT_FLOAT_EQ(61.1, estimateMapByRpmAndMaf(8000, 0.6));

	ASSERT_FLOAT_EQ(1.4, estimateMapByRpmAndMaf(2000, 5.6));
}

TEST(maf2map, integrated) {

	WITH_ENGINE_TEST_HELPER(MIATA_NA6_VAF);

}
