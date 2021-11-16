/**
 * @file	engine_test_helper.cpp
 *
 * @date Jun 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "pch.h"

#include "trigger_decoder.h"
#include "speed_density.h"
#include "fuel_math.h"
#include "accel_enrichment.h"
#include "advance_map.h"
#include "tooth_logger.h"
#include "logicdata.h"
#include "hardware.h"

#if EFI_ENGINE_SNIFFER
#include "engine_sniffer.h"
extern WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */


extern int timeNowUs;
extern WarningCodeState unitTestWarningCodeState;
extern engine_configuration_s & activeConfiguration;
extern bool printTriggerDebug;
extern bool printTriggerTrace;
extern bool printFuelDebug;
extern int minCrankingRpm;
extern Engine *engineForLuaUnitTests;

EngineTestHelperBase::EngineTestHelperBase() { 
	// todo: make this not a global variable, we need currentTimeProvider interface on engine
	timeNowUs = 0; 
	minCrankingRpm = 0;
	EnableToothLogger();
}

EngineTestHelper::EngineTestHelper(engine_type_e engineType, configuration_callback_t configurationCallback)
	: EngineTestHelper(engineType, configurationCallback, {}) {
}

EngineTestHelper::EngineTestHelper(engine_type_e engineType, const std::unordered_map<SensorType, float>& sensorValues)
	: EngineTestHelper(engineType, &emptyCallbackWithConfiguration, sensorValues) {
}

warningBuffer_t *EngineTestHelper::recentWarnings() {
	return &unitTestWarningCodeState.recentWarnings;
}

int EngineTestHelper::getWarningCounter() {
	return unitTestWarningCodeState.warningCounter;
}

EngineTestHelper::EngineTestHelper(engine_type_e engineType, configuration_callback_t configurationCallback, const std::unordered_map<SensorType, float>& sensorValues) {
	Engine *engine = &this->engine;
	engine->setConfig(engine, &persistentConfig.engineConfiguration, &persistentConfig);
	EXPAND_Engine;

	engineForLuaUnitTests = engine;

	Sensor::setMockValue(SensorType::Clt, 70);
	Sensor::setMockValue(SensorType::Iat, 30);

	for (const auto& [s, v] : sensorValues) {
		Sensor::setMockValue(s, v);
	}

	unitTestWarningCodeState.clear();

	memset(&activeConfiguration, 0, sizeof(activeConfiguration));

	enginePins.inject(PASS_ENGINE_PARAMETER_SIGNATURE);
	enginePins.reset();
	enginePins.unregisterPins();

	waveChart.inject(PASS_ENGINE_PARAMETER_SIGNATURE);
	waveChart.init();

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

	resetConfigurationExt(configurationCallback, engineType PASS_ENGINE_PARAMETER_SUFFIX);

	validateConfig(PASS_CONFIG_PARAMETER_SIGNATURE);

	enginePins.startPins();

	commonInitEngineController(PASS_ENGINE_PARAMETER_SIGNATURE);

	engineConfiguration->mafAdcChannel = EFI_ADC_10;
	engine->engineState.mockAdcState.setMockVoltage(EFI_ADC_10, 0 PASS_ENGINE_PARAMETER_SUFFIX);

	// this is needed to have valid CLT and IAT.
//todo: reuse 	initPeriodicEvents(PASS_ENGINE_PARAMETER_SIGNATURE) method
	engine->periodicSlowCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Setup running in mock airmass mode
	engineConfiguration->fuelAlgorithm = LM_MOCK;
	engine->mockAirmassModel = &mockAirmass;

	memset(mockPinStates, 0, sizeof(mockPinStates));

	initHardware(PASS_ENGINE_PARAMETER_SIGNATURE);
	rememberCurrentConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
}

EngineTestHelper::~EngineTestHelper() {
	Engine *engine = &this->engine;
	EXPAND_Engine;
	// Write history to file
	std::stringstream filePath;
	filePath << "unittest_" << ::testing::UnitTest::GetInstance()->current_test_info()->name() << ".logicdata";
	writeEvents(filePath.str().c_str());

	// Cleanup
	enginePins.reset();
	enginePins.unregisterPins();
	Sensor::resetRegistry();
	memset(mockPinStates, 0, sizeof(mockPinStates));
}

static CompositeEvent compositeEvents[COMPOSITE_PACKET_COUNT];

void EngineTestHelper::writeEvents(const char *fileName) {
	int count = copyCompositeEvents(compositeEvents);
	if (count < 2) {
		printf("Not enough data for %s\n", fileName);
		return;
	}
	printf("Writing %d records to %s\n", count, fileName);
	writeFile(fileName, compositeEvents, count);
}

/**
 * mock a change of time and fire single RISE front event
 * DEPRECATED many usages should be migrated to
 */
void EngineTestHelper::fireRise(float delayMs) {
	moveTimeForwardUs(MS2US(delayMs));
	firePrimaryTriggerRise();
}

void EngineTestHelper::smartFireRise(float delayMs) {
	moveTimeForwardAndInvokeEventsUs(MS2US(delayMs));
	firePrimaryTriggerRise();
}

void EngineTestHelper::fireFall(float delayMs) {
	moveTimeForwardUs(MS2US(delayMs));
	firePrimaryTriggerFall();
}

void EngineTestHelper::smartFireFall(float delayMs) {
	moveTimeForwardAndInvokeEventsUs(MS2US(delayMs));
	firePrimaryTriggerFall();
}

/**
 * fire single RISE front event
 */
void EngineTestHelper::firePrimaryTriggerRise() {
	efitick_t nowNt = getTimeNowNt();
	Engine *engine = &this->engine;
	EXPAND_Engine;
	LogTriggerTooth(SHAFT_PRIMARY_RISING, nowNt PASS_ENGINE_PARAMETER_SUFFIX);
	handleShaftSignal(0, true, nowNt PASS_ENGINE_PARAMETER_SUFFIX);
}

void EngineTestHelper::firePrimaryTriggerFall() {
	efitick_t nowNt = getTimeNowNt();
	Engine *engine = &this->engine;
	EXPAND_Engine;
	LogTriggerTooth(SHAFT_PRIMARY_FALLING, nowNt PASS_ENGINE_PARAMETER_SUFFIX);
	handleShaftSignal(0, false, nowNt PASS_ENGINE_PARAMETER_SUFFIX);
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

void EngineTestHelper::smartFireTriggerEvents2(int count, float durationMs) {
	for (int i = 0; i < count; i++) {
		smartFireRise(durationMs);
		smartFireFall(durationMs);
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

void EngineTestHelper::moveTimeForwardSec(float deltaTimeSec) {
	moveTimeForwardUs(MS2US(1000 * deltaTimeSec));
}

void EngineTestHelper::moveTimeForwardUs(int deltaTimeUs) {
	if (printTriggerDebug || printFuelDebug) {
		printf("moveTimeForwardUs %.1fms\r\n", deltaTimeUs / 1000.0);
	}
	timeNowUs += deltaTimeUs;
}

void EngineTestHelper::moveTimeForwardAndInvokeEventsSec(int deltaTimeSeconds) {
	moveTimeForwardAndInvokeEventsUs(MS2US(1000 * deltaTimeSeconds));
}

/**
 * this method executes all pending events while moving time forward
 */
void EngineTestHelper::moveTimeForwardAndInvokeEventsUs(int deltaTimeUs) {
	if (printTriggerDebug || printFuelDebug) {
		printf("moveTimeForwardAndInvokeEventsUs %.1fms\r\n", deltaTimeUs / 1000.0);
	}
	setTimeAndInvokeEventsUs(timeNowUs + deltaTimeUs);
}

void EngineTestHelper::setTimeAndInvokeEventsUs(int targetTime) {
	while (true) {
		scheduling_s* nextScheduledEvent = engine.executor.getHead();
		if (nextScheduledEvent == nullptr) {
			// nothing pending - we are done here
			break;
		}
		int nextEventTime = nextScheduledEvent->momentX;
		if (nextEventTime > targetTime) {
			// next event is too far in the future
			break;
		}
		timeNowUs = nextEventTime;
		engine.executor.executeAll(timeNowUs);
	}

	timeNowUs = targetTime;
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
	firmwareError(OBD_PCM_Processor_Fault, "getElementAtIndexForUnitText: null");
#endif /* EFI_UNIT_TEST */
	return nullptr;
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

	ENGINE(initializeTriggerWaveform(PASS_ENGINE_PARAMETER_SIGNATURE));

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

	ASSERT_EQ( 0,  engine->triggerCentral.isTriggerConfigChanged(PASS_ENGINE_PARAMETER_SIGNATURE)) << "trigger #1";
	eth->setTriggerType(trigger PASS_ENGINE_PARAMETER_SUFFIX);
}

void EngineTestHelper::setTriggerType(trigger_type_e trigger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	engineConfiguration->trigger.type = trigger;
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ( 1,  engine->triggerCentral.isTriggerConfigChanged(PASS_ENGINE_PARAMETER_SIGNATURE)) << "trigger #2";
	applyTriggerWaveform();
}

void EngineTestHelper::executeUntil(int timeUs) {
	scheduling_s *head;
	while ((head = engine.executor.getHead()) != nullptr) {
		if (head->momentX > timeUs) {
			break;
		}
		setTimeAndInvokeEventsUs(head->momentX);
	}
}

void setupSimpleTestEngineWithMafAndTT_ONE_trigger(EngineTestHelper *eth, injection_mode_e injectionMode) {
	setupSimpleTestEngineWithMaf(eth, injectionMode, TT_ONE);
}

void setVerboseTrigger(bool isEnabled) {
	printTriggerDebug = isEnabled;
	printTriggerTrace = isEnabled;
}
