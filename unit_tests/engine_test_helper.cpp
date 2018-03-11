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

extern int timeNowUs;
extern EnginePins enginePins;
extern EventQueue schedulingQueue;
extern int unitTestWarningCounter;
extern float testMafValue;
extern engine_configuration_s activeConfiguration;

EngineTestHelper::EngineTestHelper(engine_type_e engineType) : engine (&persistentConfig) {
	ec = &persistentConfig.engineConfiguration;
	unitTestWarningCounter = 0;

	testMafValue = 0;
	memset(&activeConfiguration, 0, sizeof(activeConfiguration));

	schedulingQueue.clear();
	enginePins.reset();

	engineConfiguration = ec;
	board_configuration_s * boardConfiguration = &engineConfiguration->bc;
	persistent_config_s *config = &persistentConfig;

	setCurveValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, -40, 1.5);
	setCurveValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, -30, 1.5);
	setCurveValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, -20, 1.42);
	setCurveValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, -10, 1.36);
	setCurveValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 0, 1.28);
	setCurveValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 10, 1.19);
	setCurveValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 20, 1.12);
	setCurveValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 30, 1.10);
	setCurveValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 40, 1.06);
	setCurveValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 50, 1.06);
	setCurveValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 60, 1.03);
	setCurveValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 70, 1.01);

	Engine *engine = &this->engine;
	prepareFuelMap(PASS_ENGINE_PARAMETER_SIGNATURE);

	initAccelEnrichment(NULL PASS_ENGINE_PARAMETER_SUFFIX);

	initSpeedDensity(PASS_ENGINE_PARAMETER_SIGNATURE);

	resetConfigurationExt(NULL, engineType PASS_ENGINE_PARAMETER_SUFFIX);
	prepareShapes(PASS_ENGINE_PARAMETER_SIGNATURE);
	engine->engineConfiguration->mafAdcChannel = (adc_channel_e)TEST_MAF_CHANNEL;

	initThermistors(NULL PASS_ENGINE_PARAMETER_SUFFIX);
	// this is needed to have valid CLT and IAT.
	engine->updateSlowSensors(PASS_ENGINE_PARAMETER_SIGNATURE);
	prepareTimingMap(PASS_ENGINE_PARAMETER_SIGNATURE);

	engine_configuration_s *engineConfiguration = engine->engineConfiguration;

	engine->triggerCentral.triggerShape.initializeTriggerShape(NULL PASS_ENGINE_PARAMETER_SUFFIX);
	engine->triggerCentral.addEventListener(rpmShaftPositionCallback, "rpm reporter", engine);
	engine->triggerCentral.addEventListener(mainTriggerCallback, "main loop", engine);
	resetTriggerConfigChangedForUnitTest();
}

void EngineTestHelper::firePrimaryTriggerRise() {
	board_configuration_s * boardConfiguration = &engine.engineConfiguration->bc;
	engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING, &engine, engine.engineConfiguration, &persistentConfig, boardConfiguration);
}

void EngineTestHelper::firePrimaryTriggerFall() {
	board_configuration_s * boardConfiguration = &engine.engineConfiguration->bc;
	engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING, &engine, engine.engineConfiguration, &persistentConfig, boardConfiguration);
}

/**
 * Sends specified number of rise/fall trigger events, with specified amount of time between those.
 *
 * This is helpful for TT_ONE trigger wheel decoder and probably other decoders as well.
 */
void EngineTestHelper::fireTriggerEvents2(int count, int durationUs) {
	for (int i = 0; i < count; i++) {
		timeNowUs += durationUs;
		firePrimaryTriggerRise();
		timeNowUs += durationUs;
		firePrimaryTriggerFall();
	}
}

void EngineTestHelper::clearQueue() {
	schedulingQueue.executeAll(99999999); // this is needed to clear 'isScheduled' flag
	assertEqualsM("queue size/0", 0, schedulingQueue.size());
	engine.iHead = NULL; // let's drop whatever was scheduled just to start from a clean state
}

void EngineTestHelper::fireTriggerEvents(int count) {
	fireTriggerEvents2(count, MS2US(5)); // 5ms
}

void EngineTestHelper::applyTriggerShape() {
	Engine *engine = &this->engine;
	engine_configuration_s *engineConfiguration = engine->engineConfiguration;
	persistent_config_s *config = engine->config;
	board_configuration_s *boardConfiguration = &engineConfiguration->bc;

	engine->triggerCentral.triggerShape.initializeTriggerShape(NULL PASS_ENGINE_PARAMETER_SUFFIX);

	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
}
