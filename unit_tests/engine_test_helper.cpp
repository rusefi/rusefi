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
#include "engine_controller.h"
#include "advance_map.h"

extern int timeNowUs;
extern WarningCodeState unitTestWarningCodeState;
extern float testMafValue;
extern float testCltValue;
extern float testIatValue;
extern engine_configuration_s & activeConfiguration;

EngineTestHelperBase::EngineTestHelperBase() { 
	// todo: make this not a global variable, we need currentTimeProvider interface on engine
	timeNowUs = 0; 
}

EngineTestHelper::EngineTestHelper(engine_type_e engineType, configuration_callback_t boardCallback) {
	unitTestWarningCodeState.clear();

	testMafValue = 0;
	memset(&activeConfiguration, 0, sizeof(activeConfiguration));

	enginePins.reset();

	persistent_config_s *config = &persistentConfig;
	Engine *engine = &this->engine;
	engine->setConfig(config);
	engine_configuration_s *engineConfiguration = engine->engineConfigurationPtr;

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

	resetConfigurationExt(NULL, boardCallback, engineType PASS_ENGINE_PARAMETER_SUFFIX);

	commonInitEngineController(NULL PASS_ENGINE_PARAMETER_SUFFIX);

	engine->engineConfigurationPtr->mafAdcChannel = TEST_MAF_CHANNEL;
	engine->engineConfigurationPtr->clt.adcChannel = TEST_CLT_CHANNEL;
	engine->engineConfigurationPtr->iat.adcChannel = TEST_IAT_CHANNEL;
	// magic voltage to get nice CLT
	testCltValue = 1.492964;
	//todosetMockCltVoltage(1.492964 PASS_ENGINE_PARAMETER_SUFFIX);
	// magic voltage to get nice IAT
	testIatValue = 4.03646;

	// this is needed to have valid CLT and IAT.
//todo: reuse 	initPeriodicEvents(PASS_ENGINE_PARAMETER_SIGNATURE) method
	engine->periodicSlowCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

}

EngineTestHelper::EngineTestHelper(engine_type_e engineType) : EngineTestHelper(engineType, &emptyCallbackWithConfiguration) {
}

/**
 * mock a change of time and fire single RISE front event
 */
void EngineTestHelper::fireRise(float delayMs) {
	moveTimeForwardUs(MS2US(delayMs));
	firePrimaryTriggerRise();
}

/**
 * fire single RISE front event
 */
void EngineTestHelper::firePrimaryTriggerRise() {
	engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING, getTimeNowNt(), &engine, engine.engineConfigurationPtr, &persistentConfig);
}

void EngineTestHelper::fireFall(float delayMs) {
	moveTimeForwardUs(MS2US(delayMs));
	firePrimaryTriggerFall();
}

void EngineTestHelper::firePrimaryTriggerFall() {
	engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING, getTimeNowNt(), &engine, engine.engineConfigurationPtr, &persistentConfig);
}

void EngineTestHelper::fireTriggerEventsWithDuration(float durationMs) {
	fireTriggerEvents2(/*count*/1, durationMs);
}

/**
 * Sends specified number of rise/fall trigger events, with specified amount of time between those.
 *
 * This is helpful for TT_ONE trigger wheel decoder and probably other decoders as well.
 */
void EngineTestHelper::fireTriggerEvents2(int count, float durationMs) {
	for (int i = 0; i < count; i++) {
		fireRise(durationMs);
		fireFall(durationMs);
	}
}

void EngineTestHelper::clearQueue() {
	engine.executor.executeAll(99999999); // this is needed to clear 'isScheduled' flag
	ASSERT_EQ( 0,  engine.executor.size()) << "Failed to clearQueue";
}

int EngineTestHelper::executeActions() {
	return engine.executor.executeAll(timeNowUs);
}

void EngineTestHelper::moveTimeForwardMs(float deltaTimeMs) {
	moveTimeForwardUs(MS2US(deltaTimeMs));
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
	InjectionEvent *event = &engine.injectionEvents.elements[injectorIndex];
	assertEvent(msg, eventIndex, (void*)turnInjectionPinHigh, momentX, event);
}

void EngineTestHelper::assertInjectorDownEvent(const char *msg, int eventIndex, efitime_t momentX, long injectorIndex) {
	InjectionEvent *event = &engine.injectionEvents.elements[injectorIndex];
	assertEvent(msg, eventIndex, (void*)turnInjectionPinLow, momentX, event);
}

scheduling_s * EngineTestHelper::assertEvent5(const char *msg, int index, void *callback, efitime_t expectedTimestamp) {
	TestExecutor *executor = &engine.executor;
	EXPECT_TRUE(executor->size() > index) << msg << " valid index";
	scheduling_s *event = executor->getForUnitTest(index);
	assertEqualsM4(msg, " callback up/down", (void*)event->action.getCallback() == (void*) callback, 1);
	efitime_t start = getTimeNowUs();
	assertEqualsM4(msg, " timestamp", expectedTimestamp, event->momentX - start);
	return event;
}

// todo: reduce code duplication with another 'getElementAtIndexForUnitText'
static AngleBasedEvent * getElementAtIndexForUnitText(int index, Engine *engine) {
	AngleBasedEvent * current;

	LL_FOREACH2(engine->angleBasedEventsHead, current, nextToothEvent)
	{
		if (index == 0)
			return current;
		index--;
	}
#if EFI_UNIT_TEST
	firmwareError(OBD_PCM_Processor_Fault, "getForUnitText: null");
#endif /* EFI_UNIT_TEST */
	return NULL;
}

AngleBasedEvent * EngineTestHelper::assertTriggerEvent(const char *msg,
		int index, AngleBasedEvent *expected,
		void *callback,
		int triggerEventIndex, angle_t angleOffsetFromTriggerEvent) {
	AngleBasedEvent * event = getElementAtIndexForUnitText(index, &engine);

	assertEqualsM4(msg, " callback up/down", (void*)event->action.getCallback() == (void*) callback, 1);

	assertEqualsM4(msg, " trigger", triggerEventIndex, event->position.triggerEventIndex);
	assertEqualsM4(msg, " angle", angleOffsetFromTriggerEvent, event->position.angleOffsetFromTriggerEvent);
	return event;
}

scheduling_s * EngineTestHelper::assertScheduling(const char *msg, int index, scheduling_s *expected, void *callback, efitime_t expectedTimestamp) {
	scheduling_s * actual = assertEvent5(msg, index, callback, expectedTimestamp);
	return actual;
}

void EngineTestHelper::assertEvent(const char *msg, int index, void *callback, efitime_t momentX, InjectionEvent *expectedEvent) {
	scheduling_s *event = assertEvent5(msg, index, callback, momentX);

	InjectionEvent *actualEvent = (InjectionEvent *)event->action.getArgument();

	assertEqualsLM(msg, (long)expectedEvent->outputs[0], (long)actualEvent->outputs[0]);
// but this would not work	assertEqualsLM(msg, expectedPair, (long)eventPair);
}


void EngineTestHelper::applyTriggerWaveform() {
	Engine *engine = &this->engine;
	EXPAND_Engine

	ENGINE(initializeTriggerWaveform(NULL PASS_ENGINE_PARAMETER_SUFFIX));

	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
}

// todo: open question if this is worth a helper method or should be inlined?
void EngineTestHelper::assertRpm(int expectedRpm, const char *msg) {
	Engine *engine = &this->engine;
	EXPAND_Engine
	EXPECT_EQ(expectedRpm, GET_RPM()) << msg;
}

void setupSimpleTestEngineWithMaf(EngineTestHelper *eth, injection_mode_e injectionMode,
		trigger_type_e trigger) {
	Engine *engine = &eth->engine;
	EXPAND_Engine

	eth->clearQueue();

	ASSERT_EQ(LM_PLAIN_MAF, engineConfiguration->fuelAlgorithm);
	engineConfiguration->isIgnitionEnabled = false; // let's focus on injection
	engineConfiguration->specs.cylindersCount = 4;
	// a bit of flexibility - the mode may be changed by some tests
	engineConfiguration->injectionMode = injectionMode;
	// set cranking mode (it's used by getCurrentInjectionMode())
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;

	setArrayValues(config->cltFuelCorrBins, 1.0f);
	setArrayValues(engineConfiguration->injector.battLagCorr, 0.0f);
	// this is needed to update injectorLag
	engine->updateSlowSensors(PASS_ENGINE_PARAMETER_SIGNATURE);

	ASSERT_NEAR( 70,  engine->sensors.clt, EPS4D) << "CLT";


	ASSERT_EQ( 0,  isTriggerConfigChanged(PASS_ENGINE_PARAMETER_SIGNATURE)) << "trigger #1";
	eth->setTriggerType(trigger PASS_ENGINE_PARAMETER_SUFFIX);
}

void EngineTestHelper::setTriggerType(trigger_type_e trigger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	engineConfiguration->trigger.type = trigger;
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ( 1,  isTriggerConfigChanged(PASS_ENGINE_PARAMETER_SIGNATURE)) << "trigger #2";
	applyTriggerWaveform();
}

void setupSimpleTestEngineWithMafAndTT_ONE_trigger(EngineTestHelper *eth, injection_mode_e injectionMode) {
	setupSimpleTestEngineWithMaf(eth, injectionMode, TT_ONE);
}
