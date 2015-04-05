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
#include "engine_test_helper.h"
#include "maf.h"

void testIgnitionPlanning(void) {
	printf("*************************************************** testIgnitionPlanning\r\n");
	EngineTestHelper eth(FORD_ESCORT_GT);
	EXPAND_EngineTestHelper;

	assertEquals(IM_BATCH, engineConfiguration->injectionMode);
}

void testEngineMath(void) {
	printf("*************************************************** testEngineMath\r\n");

	EngineTestHelper eth(FORD_ESCORT_GT);
	Engine * engine = &eth.engine;
	engine_configuration_s *engineConfiguration = engine->engineConfiguration;

	engineConfiguration->operationMode = FOUR_STROKE_CAM_SENSOR;

	assertEqualsM("600 RPM", 50, getOneDegreeTimeMs(600) * 180);
	assertEqualsM("6000 RPM", 5, getOneDegreeTimeMs(6000) * 180);


	assertEquals(312.5, getTCharge(1000, 0, 300, 350));
	assertEquals(313.5833, getTCharge(1000, 50, 300, 350));
	assertEquals(314.6667, getTCharge(1000, 100, 300, 350));


	assertEquals(312.5, getTCharge(4000, 0, 300, 350));
	assertEquals(320.0833, getTCharge(4000, 50, 300, 350));
	assertEquals(327.6667, getTCharge(4000, 100, 300, 350));
}

void testMafLookup(void) {
	printf("*************************************************** testMafLookup\r\n");

	EngineTestHelper eth(FORD_ESCORT_GT);
	EXPAND_EngineTestHelper;

	setBosch0280218037(config);
	engine->preCalculate();

	assertEqualsM("@0", -34.5000, engine->mafDecodingLookup[0]);
	assertEqualsM("@1", -33.7875, engine->mafDecodingLookup[1]);
	assertEqualsM("@2", -33.0750, engine->mafDecodingLookup[2]);
	assertEqualsM("@200", 313.8826, engine->mafDecodingLookup[200]);
	assertEqualsM("@255", 738, engine->mafDecodingLookup[255]);
}

float getMap(void) {
	return 0;
}
