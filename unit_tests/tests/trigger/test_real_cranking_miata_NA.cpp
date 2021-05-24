/*
 * @file test_real_cranking_miata_NA.cpp
 *
 * @date May 22, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "engine_test_helper.h"




TEST(cramking, realNA) {
	FILE * fp = fopen("tests/trigger/recourses/cranking_na_3.csv", "r");

	ASSERT_TRUE(fp != nullptr);
}
