/*
 * test_startOfCrankingPrimingPulse.cpp
 *
 *  Created on: Mar 4, 2018
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "test_startOfCrankingPrimingPulse.h"
#include "test_trigger_decoder.h"

void testPlainCrankingWithoutAdvancedFeatures() {
	// this is just a reference unit test implementation
	printf("*************************************************** testPlainCrankingWithoutAdvancedFeatures\r\n");

	EngineTestHelper eth(TEST_ENGINE);
	EXPAND_EngineTestHelper

	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
	assertEqualsM("RPM=0", 0, engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));


}


void testStartOfCrankingPrimingPulse() {

}

