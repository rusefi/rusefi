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

EngineTestHelper::EngineTestHelper(engine_type_e engineType) : engine (&persistentConfig) {
	engineConfiguration = ec = &persistentConfig.engineConfiguration;
	config = &persistentConfig;

	engine_configuration_s *engineConfiguration = ec;

	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, -40, 1.5);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, -30, 1.5);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, -20, 1.42);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, -10, 1.36);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 0, 1.28);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 10, 1.19);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 20, 1.12);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 30, 1.10);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 40, 1.06);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 50, 1.06);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 60, 1.03);
	setTableValue(engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE, 70, 1.01);


	engine.engineConfiguration2 = &ec2;

	Engine *engine = &this->engine;
	prepareFuelMap(engine->engineConfiguration);


	initSpeedDensity(&persistentConfig);

	resetConfigurationExt(NULL, engineType PASS_ENGINE_PARAMETER);
	prepareShapes(PASS_ENGINE_PARAMETER_F);
	engine->engineConfiguration->mafAdcChannel = (adc_channel_e)TEST_MAF_CHANNEL;
}

void EngineTestHelper::fireTriggerEvents() {
	Engine *engine = &this->engine;
	for (int i = 0; i < 24; i++) {
		timeNow += 5000; // 5ms
		triggerCentral.handleShaftSignal(SHAFT_PRIMARY_UP PASS_ENGINE_PARAMETER);
		timeNow += 5000;
		triggerCentral.handleShaftSignal(SHAFT_PRIMARY_DOWN PASS_ENGINE_PARAMETER);
	}
}

void EngineTestHelper::initTriggerShapeAndRpmCalculator() {
	initializeTriggerShape(NULL, ec, &engine);
	incrementGlobalConfigurationVersion();

	triggerCentral.addEventListener(rpmShaftPositionCallback, "rpm reporter", &engine);

}
