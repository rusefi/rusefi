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
#include "accel_enrichment.h"
#include "thermistors.h"
#include "advance_map.h"
#include "event_queue.h"

extern int timeNow;
extern EnginePins enginePins;
extern EventQueue schedulingQueue;
extern int unitTestWarningCounter;

EngineTestHelper::EngineTestHelper(engine_type_e engineType) : engine (&persistentConfig) {
	ec = &persistentConfig.engineConfiguration;
	unitTestWarningCounter = 0;

	schedulingQueue.clear();
	enginePins.reset();

	engineConfiguration = ec;
	board_configuration_s * boardConfiguration = &engineConfiguration->bc;
	persistent_config_s *config = &persistentConfig;

	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, -40, 1.5);
	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, -30, 1.5);
	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, -20, 1.42);
	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, -10, 1.36);
	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 0, 1.28);
	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 10, 1.19);
	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 20, 1.12);
	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 30, 1.10);
	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 40, 1.06);
	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 50, 1.06);
	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 60, 1.03);
	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 70, 1.01);

	Engine *engine = &this->engine;
	prepareFuelMap(PASS_ENGINE_PARAMETER_F);

	initAccelEnrichment(NULL PASS_ENGINE_PARAMETER);

	initSpeedDensity(PASS_ENGINE_PARAMETER_F);

	resetConfigurationExt(NULL, engineType PASS_ENGINE_PARAMETER);
	prepareShapes(PASS_ENGINE_PARAMETER_F);
	engine->engineConfiguration->mafAdcChannel = (adc_channel_e)TEST_MAF_CHANNEL;

	initThermistors(NULL PASS_ENGINE_PARAMETER);
	// this is needed to have valid CLT and IAT.
	engine->updateSlowSensors(PASS_ENGINE_PARAMETER_F);
	prepareTimingMap(PASS_ENGINE_PARAMETER_F);

	engine_configuration_s *engineConfiguration = engine->engineConfiguration;

	engine->triggerCentral.triggerShape.initializeTriggerShape(NULL PASS_ENGINE_PARAMETER);
	engine->triggerCentral.addEventListener(rpmShaftPositionCallback, "rpm reporter", engine);
	engine->triggerCentral.addEventListener(mainTriggerCallback, "main loop", engine);
}

void EngineTestHelper::firePrimaryTriggerRise() {
	board_configuration_s * boardConfiguration = &engine.engineConfiguration->bc;
	engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING, &engine, engine.engineConfiguration, &persistentConfig, boardConfiguration);
}

void EngineTestHelper::firePrimaryTriggerFall() {
	board_configuration_s * boardConfiguration = &engine.engineConfiguration->bc;
	engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING, &engine, engine.engineConfiguration, &persistentConfig, boardConfiguration);
}

void EngineTestHelper::fireTriggerEvents2(int count, int duration) {
	for (int i = 0; i < count; i++) {
		timeNow += duration;
		firePrimaryTriggerRise();
		timeNow += duration;
		firePrimaryTriggerFall();
	}
}

void EngineTestHelper::fireTriggerEvents(int count) {
	fireTriggerEvents2(count, 5000); // 5ms
}

void EngineTestHelper::applyTriggerShape() {
	Engine *engine = &this->engine;
	engine_configuration_s *engineConfiguration = engine->engineConfiguration;
	persistent_config_s *config = engine->config;
	board_configuration_s *boardConfiguration = &engineConfiguration->bc;

	engine->triggerCentral.triggerShape.initializeTriggerShape(NULL PASS_ENGINE_PARAMETER);

	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_F);
}
