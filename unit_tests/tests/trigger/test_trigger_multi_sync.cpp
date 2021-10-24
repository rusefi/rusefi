/*
 * @file test_trigger_multi_sync.cpp
 *
 * @date Feb 2, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "trigger_mazda.h"

TEST(trigger, miataNA) {
	TriggerWaveform naShape;
	initializeMazdaMiataNaShape(&naShape);

	WITH_ENGINE_TEST_HELPER(FRANKENSO_MIATA_NA6_MAP);
	// todo: https://github.com/rusefi/rusefi/issues/679
}
