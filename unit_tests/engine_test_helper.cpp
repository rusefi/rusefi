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
	engine.engineConfiguration = &persistentConfig.engineConfiguration;
	ec2.engineConfiguration = &persistentConfig.engineConfiguration;

	engine.engineConfiguration2 = &ec2;

	prepareFuelMap(engine.engineConfiguration);


	initSpeedDensity(ec);

	resetConfigurationExt(NULL, engineType, &engine);
	prepareShapes(&engine);
	engine.engineConfiguration->mafAdcChannel = (adc_channel_e)TEST_MAF_CHANNEL;
}

void EngineTestHelper::fireTriggerEvents() {
	for (int i = 0; i < 24; i++) {
		timeNow += 5000; // 5ms
		triggerCentral.handleShaftSignal(SHAFT_PRIMARY_UP, &engine, engine.engineConfiguration);
		timeNow += 5000;
		triggerCentral.handleShaftSignal(SHAFT_PRIMARY_DOWN, &engine, engine.engineConfiguration);
	}
}

void EngineTestHelper::initTriggerShapeAndRpmCalculator() {
	initializeTriggerShape(NULL, ec, &engine);
	incrementGlobalConfigurationVersion();

	triggerCentral.addEventListener(rpmShaftPositionCallback, "rpm reporter", &engine);

}
