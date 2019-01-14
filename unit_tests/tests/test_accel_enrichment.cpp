/**
 * @file	test_accel_enrichment.cpp
 *
 *  Created on: apr 29, 2014
 *  	Author: Dmitry Sidin
 *      Author: Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
#include "accel_enrichment.h"
#include "engine_configuration.h"
#include "engine_test_helper.h"

TEST(big, testAccelEnrichment) {
	printf("====================================================================================== testAccelEnrichment\r\n");

	EngineTestHelper eth(FORD_ASPIRE_1996);
	EXPAND_EngineTestHelper;

	engine->rpmCalculator.setRpmValue(600 PASS_ENGINE_PARAMETER_SUFFIX);
	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	engine->tpsAccelEnrichment.setLength(4);

	engine->tpsAccelEnrichment.onNewValue(0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("maxDelta", 0, engine->tpsAccelEnrichment.getMaxDelta(PASS_ENGINE_PARAMETER_SIGNATURE));
	engine->tpsAccelEnrichment.onNewValue(10 PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("maxDelta#1", 40, engine->tpsAccelEnrichment.getMaxDelta(PASS_ENGINE_PARAMETER_SIGNATURE));
	engine->tpsAccelEnrichment.onNewValue(30 PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("maxDelta#2", 80, engine->tpsAccelEnrichment.getMaxDelta(PASS_ENGINE_PARAMETER_SIGNATURE));

	engine->tpsAccelEnrichment.onNewValue(0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("maxDelta#3", 80, engine->tpsAccelEnrichment.getMaxDelta(PASS_ENGINE_PARAMETER_SIGNATURE));
	engine->tpsAccelEnrichment.onNewValue(0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("maxDelta#4", 80, engine->tpsAccelEnrichment.getMaxDelta(PASS_ENGINE_PARAMETER_SIGNATURE));
	engine->tpsAccelEnrichment.onNewValue(0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("maxDelta#5", 0, engine->tpsAccelEnrichment.getMaxDelta(PASS_ENGINE_PARAMETER_SIGNATURE));
	engine->tpsAccelEnrichment.onNewValue(0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("maxDelta", 0, engine->tpsAccelEnrichment.getMaxDelta(PASS_ENGINE_PARAMETER_SIGNATURE));
}
