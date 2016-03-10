/**
 * @file	test_accel_enrichment.cpp
 *
 *  Created on: apr 29, 2014
 *  	Author: Dmitry Sidin
 *      Author: Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"
#include "accel_enrichment.h"
#include "test_accel_enrichment.h"
#include "engine_configuration.h"
#include "engine_test_helper.h"

void testAccelEnrichment(void) {
	printf("*************************************************** testAccelEnrichment\r\n");

	EngineTestHelper eth(FORD_ASPIRE_1996);
	EXPAND_EngineTestHelper;

	engine->rpmCalculator.setRpmValue(600);
	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_F);

	assertEqualsM("eventsCount", 4, engine->engineConfiguration2->injectionEvents->eventsCount);

	engine->tpsAccelEnrichment.setLength(4);

	engine->tpsAccelEnrichment.onNewValue(0 PASS_ENGINE_PARAMETER);
	engine->tpsAccelEnrichment.onNewValue(10 PASS_ENGINE_PARAMETER);
	engine->tpsAccelEnrichment.onNewValue(30 PASS_ENGINE_PARAMETER);
	assertEqualsM("maxDelta", 80, engine->tpsAccelEnrichment.getMaxDelta());

	engine->tpsAccelEnrichment.onNewValue(0 PASS_ENGINE_PARAMETER);
	engine->tpsAccelEnrichment.onNewValue(0 PASS_ENGINE_PARAMETER);
	engine->tpsAccelEnrichment.onNewValue(0 PASS_ENGINE_PARAMETER);
	engine->tpsAccelEnrichment.onNewValue(0 PASS_ENGINE_PARAMETER);
	assertEqualsM("maxDelta", 0, engine->tpsAccelEnrichment.getMaxDelta());
}
