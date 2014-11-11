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
		triggerCentral.handleShaftSignal(&engine, SHAFT_PRIMARY_UP);
		timeNow += 5000;
		triggerCentral.handleShaftSignal(&engine, SHAFT_PRIMARY_DOWN);
	}
}

void EngineTestHelper::initTriggerShapeAndRpmCalculator() {
	initializeTriggerShape(NULL, ec, &ec2);
	incrementGlobalConfigurationVersion();

	triggerCentral.addEventListener((ShaftPositionListener) &rpmShaftPositionCallback, "rpm reporter", &engine);

}
