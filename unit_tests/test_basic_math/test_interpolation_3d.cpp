/*
 * @file    test_interpolation_3d.cpp
 *
 *  Created on: Oct 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include <stdlib.h>

#include "interpolation.h"
#include "global.h"
#include "unit_test_framework.h"

float rpmBins[5] = { 100, 200, 300, 400, 500 };
float mafBins[4] = { 1, 2, 3, 4 };

float map[4][5] = {
	{ 1, 2, 3, 4, 4},
	{ 2, 3, 4, 200, 200 },
	{ 3, 4, 200, 500, 500 },
	{ 4, 5, 300, 600, 600 },
};

static float getValue(float rpm, float maf) {
	Map3D<5, 4, float, float> x;
	x.init(map, mafBins, rpmBins);

	return x.getValue(rpm, maf);
}

static void newTestToComfirmInterpolation() {
// here's how the table loos like:
//
//__RPM_
//__300_|_10|200|
//__200_|__3|__4|
//______|__2|__3|_LOAD

	map[1][2] = 10;


	// let's start by testing corners
	ASSERT_EQ(3, getValue(/*rpm*/200, 2));
	ASSERT_NEAR( 4,  getValue(/*rpm*/200, 3), EPS4D) << "low rpm high load";

	ASSERT_EQ(10, getValue(/*rpm*/300, 2));
	ASSERT_EQ(200, getValue(/*rpm*/300, 3));

	// now testing middles of cell sides
	assertEqualsM("low rpm middle", 3.5, getValue(/*rpm*/200, 2.5));
	ASSERT_EQ( 105,  getValue(/*rpm*/300, 2.5)) << "high rpm      ";

	assertEqualsM("low load middle", 6.5, getValue(/*rpm*/250, 2));
	ASSERT_NEAR( 102,  getValue(/*rpm*/250, 3), EPS4D) << "               ";

	// slowly go from middle side towards center
	assertEqualsM("middle @ 2.1  ",16.05, getValue(/*rpm*/250, 2.1));
	assertEqualsM("middle @ 2.2   ",25.6, getValue(/*rpm*/250, 2.2));
	assertEqualsM("middle @ 2.3  ",35.15, getValue(/*rpm*/250, 2.3));

	assertEqualsM("middle cell    ", 54.25, getValue(/*rpm*/250, 2.5));

	// issue #604: interpolation outside of the table
	// X above the range
	assertEqualsM("800 @ 2.1  ",230, getValue(/*rpm*/800, 2.1));
	assertEqualsM("800 @ 2.3  ",290, getValue(/*rpm*/800, 2.3));
	assertEqualsM("800 @ 3.3  ",530, getValue(/*rpm*/800, 3.3));

	// X below the range
	assertEqualsM("-810 @ 2.1  ",2.1, getValue(/*rpm*/-810, 2.1));
	assertEqualsM("-820 @ 2.3  ",2.3, getValue(/*rpm*/-820, 2.3));

	// Y above the range
	ASSERT_EQ( 330,  getValue(/*rpm*/310, 12.1)) << "310 @ 12.1  ";
	ASSERT_EQ( 360,  getValue(/*rpm*/320, 12.3)) << "320 @ 12.3  ";

	// Y below the range
	assertEqualsM("310 @ -12.1  ", 3.1, getValue(/*rpm*/310, -12.1));
	assertEqualsM("320 @ -12.3  ", 3.2, getValue(/*rpm*/320, -12.3));
}

TEST(misc, testInterpolate3d) {
	printf("*** no interpolation here 1\r\n");
	ASSERT_FLOAT_EQ(2, getValue(100, 2));

	printf("*** no interpolation here 2\r\n");
	ASSERT_NEAR(5, getValue(200, 4), EPS4D);

	printf("*** rpm interpolated value expected1\r\n");
	ASSERT_FLOAT_EQ(2.5, getValue(150, 2));

	printf("*** rpm interpolated value expected2\r\n");
	ASSERT_FLOAT_EQ(102, getValue(250, 3));

	printf("*** both rpm and maf interpolated value expected\r\n");
	ASSERT_FLOAT_EQ(361, getValue(335.3, 3.551));

	printf("*** both rpm and maf interpolated value expected 2\r\n");
	ASSERT_FLOAT_EQ(203.6, getValue(410.01, 2.012));

	printf("*** both rpm and maf interpolated value expected 3\r\n");
	ASSERT_FLOAT_EQ(600, getValue(1000000, 1000));

	printf("*** both rpm and maf interpolated value expected 4\r\n");

	ASSERT_FLOAT_EQ(4, getValue(410.01, -1));


	ASSERT_FLOAT_EQ(1, getValue(-1, -1));

	newTestToComfirmInterpolation();

}
