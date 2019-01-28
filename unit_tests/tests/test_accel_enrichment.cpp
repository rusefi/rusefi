/**
 * @file	test_accel_enrichment.cpp
 *
 *  Created on: apr 29, 2014
 *  	Author: Dmitry Sidin
 *      Author: Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
#include "engine_configuration.h"
#include "accel_enrichment.h"
#include "engine_test_helper.h"

TEST(big, testAccelEnrichment) {
	printf("====================================================================================== testAccelEnrichment\r\n");

	WITH_ENGINE_TEST_HELPER(FORD_ASPIRE_1996);

	engine->rpmCalculator.setRpmValue(600 PASS_ENGINE_PARAMETER_SUFFIX);
	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	engine->tpsAccelEnrichment.setLength(4);

	engine->tpsAccelEnrichment.onNewValue(0 PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ( 0,  engine->tpsAccelEnrichment.getMaxDelta(PASS_ENGINE_PARAMETER_SIGNATURE)) << "maxDelta";
	engine->tpsAccelEnrichment.onNewValue(10 PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ( 40,  engine->tpsAccelEnrichment.getMaxDelta(PASS_ENGINE_PARAMETER_SIGNATURE)) << "maxDelta#1";
	engine->tpsAccelEnrichment.onNewValue(30 PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ( 80,  engine->tpsAccelEnrichment.getMaxDelta(PASS_ENGINE_PARAMETER_SIGNATURE)) << "maxDelta#2";

	engine->tpsAccelEnrichment.onNewValue(0 PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ( 80,  engine->tpsAccelEnrichment.getMaxDelta(PASS_ENGINE_PARAMETER_SIGNATURE)) << "maxDelta#3";
	engine->tpsAccelEnrichment.onNewValue(0 PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ( 80,  engine->tpsAccelEnrichment.getMaxDelta(PASS_ENGINE_PARAMETER_SIGNATURE)) << "maxDelta#4";
	engine->tpsAccelEnrichment.onNewValue(0 PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ( 0,  engine->tpsAccelEnrichment.getMaxDelta(PASS_ENGINE_PARAMETER_SIGNATURE)) << "maxDelta#5";
	engine->tpsAccelEnrichment.onNewValue(0 PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ( 0,  engine->tpsAccelEnrichment.getMaxDelta(PASS_ENGINE_PARAMETER_SIGNATURE)) << "maxDelta";
}
