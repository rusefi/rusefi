/**
 * @file	engine_test_helper.cpp
 *
 * @date Jun 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "engine_test_helper.h"
#include "stddef.h"
#include "trigger_decoder.h"

extern int timeNow;

EngineTestHelper::EngineTestHelper(engine_type_e engineType) {
	ec = &persistentConfig.engineConfiguration;

	configuration.engineConfiguration = ec;
	configuration.engineConfiguration2 = &ec2;

	resetConfigurationExt(NULL, FORD_INLINE_6_1995, ec, &ec2, &persistentConfig.engineConfiguration.bc);
}

void EngineTestHelper::fireTriggerEvents() {
	for (int i = 0; i < 24; i++) {
		timeNow += 5000; // 5ms
		triggerCentral.handleShaftSignal(&configuration, SHAFT_PRIMARY_UP, timeNow);
		timeNow += 5000;
		triggerCentral.handleShaftSignal(&configuration, SHAFT_PRIMARY_DOWN, timeNow);
	}
}

void EngineTestHelper::initTriggerShapeAndRpmCalculator() {
	initializeTriggerShape(NULL, ec, &ec2);
	incrementGlobalConfigurationVersion();

	triggerCentral.addEventListener((ShaftPositionListener) &rpmShaftPositionCallback, "rpm reporter", &rpmState);

}
