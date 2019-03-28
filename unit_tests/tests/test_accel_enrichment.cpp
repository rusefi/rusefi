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

static void doFractionalTpsIteration(int period, int divisor, int numCycles, std::vector<floatms_t> &tpsEnrich DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// every cycle
	engineConfiguration->tpsAccelFractionPeriod = period;
	// split into 2 portions
	engineConfiguration->tpsAccelFractionDivisor = divisor;

	engine->tpsAccelEnrichment.reset();
	engine->tpsAccelEnrichment.onNewValue(0 PASS_ENGINE_PARAMETER_SUFFIX);
	for (int i = 0; i < numCycles; i++) {
		engine->tpsAccelEnrichment.onNewValue(10 PASS_ENGINE_PARAMETER_SUFFIX);
		engine->tpsAccelEnrichment.onEngineCycleTps(PASS_ENGINE_PARAMETER_SIGNATURE);
		tpsEnrich[i] = engine->tpsAccelEnrichment.getTpsEnrichment(PASS_ENGINE_PARAMETER_SIGNATURE);
	}
}

TEST(big, testAccelEnrichmentFractionalTps) {
	printf("====================================================================================== testAccelEnrichmentFractionalTps\r\n");

	WITH_ENGINE_TEST_HELPER(FORD_ASPIRE_1996);

	// setup
	engineConfiguration->tpsAccelEnrichmentThreshold = 5;

	// fill tps2tps map (todo: there should be a better way?)
	static const float tpsTpsConst = 1.0f;
	for (int loadIndex = 0; loadIndex < TPS_TPS_ACCEL_TABLE; loadIndex++) {
		for (int rpmIndex = 0; rpmIndex < TPS_TPS_ACCEL_TABLE; rpmIndex++) {
			config->tpsTpsAccelTable[loadIndex][rpmIndex] = tpsTpsConst;
		}
	}

	Logging logger;
	initAccelEnrichment(&logger PASS_ENGINE_PARAMETER_SUFFIX);

	engine->rpmCalculator.setRpmValue(600 PASS_ENGINE_PARAMETER_SUFFIX);
	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	engine->tpsAccelEnrichment.setLength(2);


	const int numCycles = 4;
	std::vector<floatms_t> tpsEnrich(numCycles);

	// first, test the default behavior without fractional division
	doFractionalTpsIteration(1, 1, numCycles, tpsEnrich PASS_ENGINE_PARAMETER_SUFFIX);
	// the portion for the first cycle is full-enriched and there's no enrichment for next cycles
	EXPECT_THAT(tpsEnrich, testing::ElementsAre(1.0f, 0.0f, 0.0f, 0.0f)) << "fractionalTps#1";

	// divide into 2 each cycle
	doFractionalTpsIteration(1, 2, numCycles, tpsEnrich PASS_ENGINE_PARAMETER_SUFFIX);
	// we have half-portion for the first cycle, then 1/4-th and 1/8th and so on...
	EXPECT_THAT(tpsEnrich, testing::ElementsAre(0.5f, 0.25f, 0.125f, 0.0625f)) << "fractionalTps#2";

	// split every portion into 3 sub-portions (so the whole enrichment takes longer)
	doFractionalTpsIteration(3, 1, numCycles, tpsEnrich PASS_ENGINE_PARAMETER_SUFFIX);
	// we have 1/3rd-portion for the first three cycles
	const float th = (1.0f / 3.0f);
	EXPECT_THAT(tpsEnrich, testing::ElementsAre(testing::FloatEq(th), testing::FloatEq(th), testing::FloatEq(th), 0.0f)) << "fractionalTps#3";

	// split every divided portion into 2 sub-portions (so the whole enrichment takes longer)
	doFractionalTpsIteration(2, 2, numCycles, tpsEnrich PASS_ENGINE_PARAMETER_SUFFIX);
	// we have half-portion for the first two cycles, and 1/4-th portion for the next 2 cycles, and so on...
	EXPECT_THAT(tpsEnrich, testing::ElementsAre(0.25f, 0.25f, 0.125f, 0.125f)) << "fractionalTps#4";

}
