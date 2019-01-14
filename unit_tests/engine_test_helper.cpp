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
#include "allsensors.h"
#include "advance_map.h"
#include "algo.h"

extern int timeNowUs;
extern EnginePins enginePins;
extern WarningCodeState unitTestWarningCodeState;
extern float testMafValue;
extern float testCltValue;
extern float testIatValue;
extern engine_configuration_s activeConfiguration;

EngineTestHelperBase::EngineTestHelperBase() { 
	// todo: make this not a global variable, we need currentTimeProvider interface on engine
	timeNowUs = 0; 
}

EngineTestHelper::EngineTestHelper(engine_type_e engineType) : engine (&persistentConfig) {
	unitTestWarningCodeState.clear();

	testMafValue = 0;
	memset(&activeConfiguration, 0, sizeof(activeConfiguration));

	enginePins.reset();

	persistent_config_s *config = &persistentConfig;
	Engine *engine = &this->engine;
	engine->setConfig(config);
	engine_configuration_s *engineConfiguration = engine->engineConfigurationPtr;
	board_configuration_s * boardConfiguration = &persistentConfig.engineConfiguration.bc;

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

	initDataStructures(PASS_ENGINE_PARAMETER_SIGNATURE);

	initAccelEnrichment(NULL PASS_ENGINE_PARAMETER_SUFFIX);
	initSensors(NULL PASS_ENGINE_PARAMETER_SUFFIX);

	resetConfigurationExt(NULL, engineType PASS_ENGINE_PARAMETER_SUFFIX);
	prepareShapes(PASS_ENGINE_PARAMETER_SIGNATURE);
	engine->engineConfigurationPtr->mafAdcChannel = (adc_channel_e)TEST_MAF_CHANNEL;
	engine->engineConfigurationPtr->clt.adcChannel = (adc_channel_e)TEST_CLT_CHANNEL;
	engine->engineConfigurationPtr->iat.adcChannel = (adc_channel_e)TEST_IAT_CHANNEL;
	testCltValue = 1.492964;
	testIatValue = 4.03646;

	// this is needed to have valid CLT and IAT.
//todo: reuse 	initPeriodicEvents(PASS_ENGINE_PARAMETER_SIGNATURE) method
	engine->periodicSlowCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	engine->initializeTriggerShape(NULL PASS_ENGINE_PARAMETER_SUFFIX);
	engine->triggerCentral.addEventListener(rpmShaftPositionCallback, "rpm reporter", engine);
	engine->triggerCentral.addEventListener(mainTriggerCallback, "main loop", engine);
	resetTriggerConfigChangedForUnitTest();
}

/**
 * mock a change of time and fire single RISE front event
 */
void EngineTestHelper::fireRise(int delayMs) {
	moveTimeForwardUs(MS2US(delayMs));
	firePrimaryTriggerRise();
}

/**
 * fire single RISE front event
 */
void EngineTestHelper::firePrimaryTriggerRise() {
	board_configuration_s * boardConfiguration = &engine.engineConfigurationPtr->bc;
	engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING, &engine, engine.engineConfigurationPtr, &persistentConfig, boardConfiguration);
}

void EngineTestHelper::fireFall(int delayMs) {
	moveTimeForwardUs(MS2US(delayMs));
	firePrimaryTriggerFall();
}

void EngineTestHelper::firePrimaryTriggerFall() {
	board_configuration_s * boardConfiguration = &engine.engineConfigurationPtr->bc;
	engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING, &engine, engine.engineConfigurationPtr, &persistentConfig, boardConfiguration);
}

void EngineTestHelper::fireTriggerEventsWithDuration(int durationMs) {
	fireTriggerEvents2(/*count*/1, durationMs);
}

/**
 * Sends specified number of rise/fall trigger events, with specified amount of time between those.
 *
 * This is helpful for TT_ONE trigger wheel decoder and probably other decoders as well.
 */
void EngineTestHelper::fireTriggerEvents2(int count, int durationMs) {
	for (int i = 0; i < count; i++) {
		fireRise(durationMs);
		fireFall(durationMs);
	}
}

void EngineTestHelper::clearQueue() {
	engine.executor.executeAll(99999999); // this is needed to clear 'isScheduled' flag
	assertEqualsM("queue size/0", 0, engine.executor.size());
	engine.iHead = NULL; // let's drop whatever was scheduled just to start from a clean state
}

int EngineTestHelper::executeActions() {
	return engine.executor.executeAll(timeNowUs);
}

void EngineTestHelper::moveTimeForwardUs(int deltaTimeUs) {
	timeNowUs += deltaTimeUs;
}

efitimeus_t EngineTestHelper::getTimeNowUs(void) {
	return timeNowUs;
}

void EngineTestHelper::fireTriggerEvents(int count) {
	fireTriggerEvents2(count, 5); // 5ms
}

void EngineTestHelper::assertInjectorUpEvent(const char *msg, int eventIndex, efitime_t momentX, long injectorIndex) {
	InjectionSignalPair *pair = &engine.fuelActuators[injectorIndex];
	assertEvent(&engine.executor, msg, eventIndex, (void*)seTurnPinHigh, getTimeNowUs(), momentX, (long)pair);
}

void EngineTestHelper::assertInjectorDownEvent(const char *msg, int eventIndex, efitime_t momentX, long injectorIndex) {
	InjectionSignalPair *pair = &engine.fuelActuators[injectorIndex];
	assertEvent(&engine.executor, msg, eventIndex, (void*)seTurnPinLow, getTimeNowUs(), momentX, (long)pair);
}

scheduling_s * EngineTestHelper::assertEvent5(TestExecutor *executor, const char *msg, int index, void *callback, efitime_t start, efitime_t momentX) {
	assertTrueM(msg, executor->size() > index);
	scheduling_s *event = executor->getForUnitTest(index);
	assertEqualsM4(msg, " up/down", (void*)event->callback == (void*) callback, 1);
	assertEqualsM(msg, momentX, event->momentX - start);
	return event;
}

void EngineTestHelper::assertEvent(TestExecutor *executor, const char *msg, int index, void *callback, efitime_t start, efitime_t momentX, long param) {
	scheduling_s *event = assertEvent5(executor, msg, index, callback, start, momentX);

	InjectionSignalPair *eventPair = (InjectionSignalPair *)event->param;

	InjectionSignalPair *expectedPair = (InjectionSignalPair *)param;

	assertEqualsLM(msg, expectedPair->outputs[0], (long)eventPair->outputs[0]);
// but this would not work	assertEqualsLM(msg, expectedPair, (long)eventPair);
}


void EngineTestHelper::applyTriggerShape() {
	Engine *engine = &this->engine;
	EXPAND_Engine


	ENGINE(initializeTriggerShape(NULL PASS_ENGINE_PARAMETER_SUFFIX));

	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
}
