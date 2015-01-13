/*
 * @file	test_engine_math.c
 *
 *  Created on: Nov 14, 2013
 *      Author: Andrey Belomutskiy, (c) 2012-2015
 */

#include "test_engine_math.h"
#include "main.h"
#include "engine_math.h"
#include "engine_configuration.h"
#include "map.h"
#include "speed_density.h"

extern engine_configuration_s *engineConfiguration;
extern engine_configuration2_s *engineConfiguration2;

void testEngineMath(void) {
	printf("*************************************************** testEngineMath\r\n");

	engineConfiguration->rpmMultiplier = 0.5;

	assertEqualsM("600 RPM", 50, getOneDegreeTimeMs(600) * 180);
	assertEqualsM("6000 RPM", 5, getOneDegreeTimeMs(6000) * 180);


	assertEquals(312.5, getTCharge(1000, 0, 300, 350));
	assertEquals(313.5833, getTCharge(1000, 50, 300, 350));
	assertEquals(314.6667, getTCharge(1000, 100, 300, 350));


	assertEquals(312.5, getTCharge(4000, 0, 300, 350));
	assertEquals(320.0833, getTCharge(4000, 50, 300, 350));
	assertEquals(327.6667, getTCharge(4000, 100, 300, 350));

}

float getMap(void) {
	return 0;
}
