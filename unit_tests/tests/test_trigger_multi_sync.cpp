/*
 * @file test_trigger_multi_sync.cpp
 *
 * @date Feb 2, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "engine_test_helper.h"
#include "trigger_mazda.h"

TEST(trigger, miataNA) {
	TriggerShape naShape;
	initializeMazdaMiataNaShape(&naShape);


	WITH_ENGINE_TEST_HELPER(MIATA_NA6_MAP);
}
