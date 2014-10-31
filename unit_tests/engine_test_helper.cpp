/**
 * @file	engine_test_helper.cpp
 *
 * @date Jun 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "engine_test_helper.h"
#include "stddef.h"
#include "trigger_decoder.h"
#include "speed_density.h"
#include "fuel_math.h"

extern int timeNow;

EngineTestHelper::EngineTestHelper(engine_type_e engineType) {
	ec = &persistentConfig.engineConfiguration;

	engine.engineConfiguration = ec;

	configuration.engineConfiguration = ec;
	configuration.engineConfiguration2 = &ec2;
	engine.engineConfiguration2 = &ec2;
	ec2.engineConfiguration = ec;

	prepareFuelMap(engine.engineConfiguration);


	initSpeedDensity(ec);

	resetConfigurationExt(NULL, FORD_INLINE_6_1995, &engine);
	ec->mafAdcChannel = (adc_channel_e)TEST_MAF_CHANNEL;
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
