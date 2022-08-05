/*
 * test_find_index.cpp
 *
 *  Created on: Oct 30, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "interpolation.h"

static void testIndex(const int expected, const float array[], int size, float value) {
	ASSERT_EQ(expected, findIndex(array, size, value));
	ASSERT_EQ(expected, findIndex2(array, size, value));
}

TEST(misc, testFindIndex) {
	printf("*************************************************** testFindIndex\r\n");
/*
	{
		// in real life we only use powers of 2 for sizes
		float array5[] = { 1, 2, 3, 4, 5 };
		const int size5 = 5;
		printf("To the right5\r\n");
		testIndex(4, array5, size5, 10.0);

		printf("Middle2\r\n");
		testIndex(3, array5, size5, 4);

		printf("Middle2\r\n");
		testIndex(2, array5, size5, 3.1);

	}
*/
	{
		float array4[] = { 1, 2, 3, 4 };
		const int size4 = 4;

		printf("To the left\r\n");
		testIndex(-1, array4, size4, -1.0);

		printf("To the right4\r\n");
		testIndex(3, array4, size4, 10.0);

		printf("On the edge\r\n");
		testIndex(3, array4, size4, 4.0);

		printf("Another1\r\n");
		testIndex(2, array4, size4, 3.9);

		printf("Another2\r\n");
		testIndex(3, array4, size4, 4.1);

		printf("Another3\r\n");
		testIndex(1, array4, size4, 2);

		printf("Left edge1\r\n");
		testIndex(0, array4, size4, 1);

		printf("Left edge2\r\n");
		testIndex(0, array4, size4, 1.1);

		printf("Middle\r\n");
		testIndex(2, array4, size4, 3);
	}
}

TEST(misc, testSetTableValue) {
	printf("*************************************************** testSetTableValue\r\n");

	persistent_config_s config;

	for (int i = 0; i < CLT_CURVE_SIZE; i++) {
		config.cltFuelCorrBins[i] = -40 + i * 10;
		config.cltFuelCorr[i] = 1;
	}

	ASSERT_EQ(1, config.cltFuelCorr[0]);

	setCurveValue(config.cltFuelCorrBins, config.cltFuelCorr, CLT_CURVE_SIZE, -40, 1.5);
	ASSERT_FLOAT_EQ(1.5, config.cltFuelCorr[0]);

	setCurveValue(config.cltFuelCorrBins, config.cltFuelCorr, CLT_CURVE_SIZE, -50, 1.4);
	ASSERT_FLOAT_EQ(1.4, config.cltFuelCorr[0]);

}
