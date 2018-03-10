/*
 *  Created on: Oct 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

/**
 * @file    test_interpolation_3d.c
 */

#include "test_interpolation_3d.h"
#include <stdlib.h>

#include "interpolation.h"
#include "main.h"
#include "unit_test_framework.h"

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

static void newTestToComfirmInterpolation() {
// here's how the table loos like:
//
//__RPM_
//__300_|_10|200|
//__200_|__3|__4|
//______|__2|__3|_LOAD

	map2[1] = 10;


	// let's start by testing corners
	assertEquals(3, getValue(/*rpm*/200, 2));
	assertEqualsM("low rpm high load", 4, getValue(/*rpm*/200, 3));

	assertEquals(10, getValue(/*rpm*/300, 2));
	assertEquals(200, getValue(/*rpm*/300, 3));

	// now testing middles of cell sides
	assertEqualsM("low rpm middle", 3.5, getValue(/*rpm*/200, 2.5));
	assertEqualsM("high rpm      ", 105, getValue(/*rpm*/300, 2.5));

	assertEqualsM("low load middle", 6.5, getValue(/*rpm*/250, 2));
	assertEqualsM("               ", 102, getValue(/*rpm*/250, 3));

	// slowly go from middle side towards center
	assertEqualsM("middle @ 2.1  ",16.05, getValue(/*rpm*/250, 2.1));
	assertEqualsM("middle @ 2.2   ",25.6, getValue(/*rpm*/250, 2.2));
	assertEqualsM("middle @ 2.3  ",35.15, getValue(/*rpm*/250, 2.3));

	assertEqualsM("middle cell    ", 54.25, getValue(/*rpm*/250, 2.5));
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

	newTestToComfirmInterpolation();

}
