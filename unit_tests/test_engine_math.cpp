/*
 * @file	test_engine_math.c
 *
 *  Created on: Nov 14, 2013
 *      Author: Andrey Belomutskiy, (c) 2012-2013
 */

#include "test_engine_math.h"
#include "main.h"
#include "engine_math.h"
#include "engine_configuration.h"
#include "ec2.h"
#include "map.h"

extern engine_configuration_s *engineConfiguration;
extern engine_configuration2_s *engineConfiguration2;

void testEngineMath(void) {
	printf("*************************************************** testEngineMath\r\n");

	engineConfiguration->rpmMultiplier = 0.5;

	assertEqualsM("600 RPM", 50, getOneDegreeTimeMs(600) * 180);
	assertEqualsM("6000 RPM", 5, getOneDegreeTimeMs(6000) * 180);
}

float getMap(void) {
	return 0;
}
