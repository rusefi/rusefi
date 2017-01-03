/*
 *  Created on: Oct 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

/**
 * @file    test_interpolation_3d.c
 */

#include "test_interpolation_3d.h"
#include <stdio.h>
#include <stdlib.h>

#include "interpolation.h"
#include "main.h"

float rpmBins[5] = { 100, 200, 300, 400, 500 };
float mafBins[4] = { 1, 2, 3, 4 };

float map0[4] = { 1, 2, 3, 4 };
float map1[4] = { 2, 3, 4, 5 };
float map2[4] = { 3, 4, 200, 300 };
float map3[4] = { 4, 200, 500, 600 };
float map4[4] = { 4, 200, 500, 600 };

float *map[5] = { map0, map1, map2, map3, map4 };


static float getValue(float rpm, float maf) {
	return interpolate3d(rpm, rpmBins, 5, maf, mafBins, 4, map);
}

void testInterpolate3d(void) {
	printf("*************************************************** testInterpolate3d\r\n");
	float dwell;
	printf("*** no interpolation here 1\r\n");
	dwell = getValue(100, 2);
	assertEquals(2, dwell);

	printf("*** no interpolation here 2\r\n");
	dwell = getValue(200, 4);
	assertEquals(5, dwell);

	printf("*** rpm interpolated value expected1\r\n");
	dwell = getValue(150, 2);
	assertEquals(2.5, dwell);

	printf("*** rpm interpolated value expected2\r\n");
	dwell = getValue(250, 3);
	assertEquals(102, dwell);

	printf("*** both rpm and maf interpolated value expected\r\n");
	dwell = getValue(335.3, 3.551);
	assertEquals(361, dwell);

	printf("*** both rpm and maf interpolated value expected 2\r\n");
	dwell = getValue(410.01, 2.012);
	assertEquals(203.6, dwell);

	printf("*** both rpm and maf interpolated value expected 3\r\n");
	dwell = getValue(1000000, 1000);
	assertEquals(600, dwell);

	printf("*** both rpm and maf interpolated value expected 4\r\n");
	dwell = getValue(410.01, -1);
	assertEquals(4, dwell);

	dwell = getValue(-1, -1);
	assertEquals(1, dwell);
}
