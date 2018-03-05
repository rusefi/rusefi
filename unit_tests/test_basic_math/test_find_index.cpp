/*
 * test_find_index.cpp
 *
 *  Created on: Oct 30, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
#include "engine_math.h"
#include "interpolation.h"
#include "unit_test_framework.h"

static int testIndex(const int expected, const float array[], int size, float value) {
	assertEquals(expected, findIndex(array, size, value));
	assertEquals(expected, findIndex2(array, size, value));
}

void testFindIndex(void) {
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

//static float getValue2(float key, float maf) {
//
//}

void testInterpolate2d(void) {
	printf("*************************************************** testInterpolate2d\r\n");

	float bins4[] = { 1, 2, 3, 4 };
	float values4[] = { 1, 20, 30, 400 };
	int size = 4;

	int result;

	printf("Left size\r\n");
	result = interpolate2d("t", 0, bins4, values4, size);
	assertEquals(1, result);

	printf("Right size\r\n");
	result = interpolate2d("t", 10, bins4, values4, size);
	assertEquals(400, result);

	printf("Middle1\r\n");
	result = interpolate2d("t", 3, bins4, values4, size);
	assertEquals(30, result);

	printf("Middle1\r\n");
	result = interpolate2d("t", 3.5, bins4, values4, size);
	assertEquals(215, result);
}

void testSetTableValue(void) {
	printf("*************************************************** testSetTableValue\r\n");

	persistent_config_s config;
//	memset()

	for (int i = 0; i < CLT_CURVE_SIZE; i++) {
		config.cltFuelCorrBins[i] = -40 + i * 10;
		config.cltFuelCorr[i] = 1;
	}

	assertEquals(1, config.cltFuelCorr[0]);

	setCurveValue(config.cltFuelCorrBins, config.cltFuelCorr, CLT_CURVE_SIZE, -40, 1.5);
	assertEquals(1.5, config.cltFuelCorr[0]);

	setCurveValue(config.cltFuelCorrBins, config.cltFuelCorr, CLT_CURVE_SIZE, -50, 1.4);
	assertEquals(1.4, config.cltFuelCorr[0]);

}
