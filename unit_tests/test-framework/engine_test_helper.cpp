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
#include "unit_test_logger.h"
#include "hardware.h"
// https://stackoverflow.com/questions/23427804/cant-find-mkdir-function-in-dirent-h-for-windows
#include <sys/types.h>
#include <sys/stat.h>

bool unitTestBusyWaitHack;
bool unitTestTaskPrecisionHack;
bool unitTestTaskNoFastCallWhileAdvancingTimeHack;

#if EFI_ENGINE_SNIFFER
#include "engine_sniffer.h"
extern WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */

#include "fw_configuration.h"

extern engine_configuration_s & activeConfiguration;
extern PinRepository pinRepository;
extern bool printTriggerDebug;
extern bool printTriggerTrace;
extern bool printFuelDebug;

EngineTestHelperBase::EngineTestHelperBase(Engine * eng, engine_configuration_s * econfig, persistent_config_s * pers) {
	// todo: make this not a global variable, we need currentTimeProvider interface on engine
	setTimeNowUs(0);
	ButtonDebounce::resetForUnitTests();
	unitTestBusyWaitHack = false;
	EnableToothLogger();
	if (engine || engineConfiguration || config) {
		firmwareError(ObdCode::OBD_PCM_Processor_Fault,
			      "Engine configuration not cleaned up by previous test");
	}
	engine = eng;
	engineConfiguration = econfig;
	config = pers;

	setup_custom_fw_overrides();
}

EngineTestHelperBase::~EngineTestHelperBase() {
	engine = nullptr;
	engineConfiguration = nullptr;
	config = nullptr;
}

EngineTestHelper::EngineTestHelper(engine_type_e engineType)
	: EngineTestHelper(engineType, &emptyCallbackWithConfiguration) {
}

EngineTestHelper::EngineTestHelper(engine_type_e engineType, configuration_callback_t configurationCallback)
	: EngineTestHelper(engineType, configurationCallback, {}) {
}

EngineTestHelper::EngineTestHelper(engine_type_e engineType, const std::unordered_map<SensorType, float>& sensorValues)
	: EngineTestHelper(engineType, &emptyCallbackWithConfiguration, sensorValues) {
}

warningBuffer_t *EngineTestHelper::recentWarnings() {
	return getRecentWarnings();
}

int EngineTestHelper::getWarningCounter() {
	return engine.engineState.warnings.warningCounter;
}

FILE *jsonTrace = nullptr;

EngineTestHelper::EngineTestHelper(engine_type_e engineType, configuration_callback_t configurationCallback, const std::unordered_map<SensorType, float>& sensorValues) :
	EngineTestHelperBase(&engine, &persistentConfig.engineConfiguration, &persistentConfig)
{
	persistentConfig = decltype(persistentConfig){};
	pinRepository = decltype(pinRepository){};

	auto testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
	extern bool hasInitGtest;
	if (hasInitGtest) {
		#if IS_WINDOWS_COMPILER
		  mkdir(TEST_RESULTS_DIR);
		#else
		  mkdir(TEST_RESULTS_DIR, 0777);
		#endif
		createUnitTestLog();

		std::stringstream filePath;
		filePath << TEST_RESULTS_DIR << "/unittest_" << testInfo->test_case_name() << "_" << testInfo->name() << "_trace.json";
		// fun fact: ASAN says not to extract 'fileName' into a variable, we must be doing something a bit not right?
		jsonTrace = fopen(filePath.str().c_str(), "wb");
		if (jsonTrace == nullptr) {
			//    		criticalError("Error creating file [%s]", filePath.str().c_str());
			// TOOD handle config tests
			printf("Error creating file [%s]\n", filePath.str().c_str());
		} else {
			fprintf(jsonTrace, "{\"traceEvents\": [\n");
			fprintf(jsonTrace, "{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":-16,\"tid\":0,\"args\":{\"name\":\"Main\"}}\n");
		}
    } else {
		// todo: document why this branch even exists
		jsonTrace = nullptr;
	}

	Sensor::setMockValue(SensorType::Clt, 70);
	Sensor::setMockValue(SensorType::Iat, 30);

	for (const auto& [s, v] : sensorValues) {
		Sensor::setMockValue(s, v);
	}

	activeConfiguration = engine_configuration_s{};

	enginePins.reset();
	enginePins.unregisterPins();

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

	initDataStructures();

	resetConfigurationExt(configurationCallback, engineType);

	validateConfigOnStartUpOrBurn();

	enginePins.startPins();

	commonInitEngineController();

	// this is needed to have valid CLT and IAT.
	//todo: reuse 	initPeriodicEvents() method
	engine.periodicSlowCallback();

	extern bool hasInitGtest;
	if (hasInitGtest) {
		// When built in unit tests mode UNSUPPORTED_ENUM_VALUE leads to acquiring of mockAirmassModel
		engineConfiguration->fuelAlgorithm = engine_load_mode_e::UNSUPPORTED_ENUM_VALUE;

		mockAirmass = std::make_unique<::testing::NiceMock<MockAirmass>>();
		engine.mockAirmassModel = mockAirmass.get();
	}

	memset(mockPinStates, 0, sizeof(mockPinStates));

	setVerboseTrigger(false);

	initHardware();
	rememberCurrentConfiguration();
}

static void writeEventsToFile(const char *fileName,
		const std::vector<CompositeEvent> &events) {
	FILE *ptr = fopen(fileName, "wb");
	size_t count = events.size();

	// todo: move magic keywords to something.txt and reuse magic constants from C and java, once we have java converter
	fprintf(ptr, "count,%d\n", count);

#define numChannels 6 // todo: clean-up

	for (size_t i = 0; i < count; i++) {
		const CompositeEvent *event = &events[i];

		uint32_t ts = event->timestamp;
		fprintf(ptr, "timestamp,%d\n", ts);

		for (int ch = 0; ch < numChannels; ch++) {
			int chState = getChannelState(ch, event);
			fprintf(ptr, "state,%d,%d\n", ch, chState);

		}

	}


	fclose(ptr);
}

EngineTestHelper::~EngineTestHelper() {
	// Write history to file
	extern bool hasInitGtest;
	auto testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
	if (hasInitGtest) {
    	std::stringstream filePath;
    	filePath << TEST_RESULTS_DIR << "/unittest_" << testInfo->test_case_name() << "_" << testInfo->name() << ".logicdata";
	    writeEventsLogicData(filePath.str().c_str());
	}
	if (hasInitGtest) {
    	std::stringstream filePath;
    	filePath << TEST_RESULTS_DIR << "/unittest_" << testInfo->test_case_name() << "_" << testInfo->name() << ".events.txt";
	    writeEvents2(filePath.str().c_str());
	}

  if (jsonTrace != nullptr) {
   	fprintf(jsonTrace, "]}\n");
    fclose(jsonTrace);
    jsonTrace = nullptr;
  }
  closeUnitTestLog();

	// Cleanup
  	// reset pin config state, will trigger isPinConfigurationChanged
	enginePins.resetForUnitTest();
	enginePins.reset();
	enginePins.unregisterPins();
	Sensor::resetRegistry();
	memset(mockPinStates, 0, sizeof(mockPinStates));
	unitTestTaskNoFastCallWhileAdvancingTimeHack = false;
}

void EngineTestHelper::writeEventsLogicData(const char *fileName) {
	const auto& events = getCompositeEvents();
	if (events.size() < 2) {
		printf("Not enough data for %s\n", fileName);
		return;
	}
	printf("Writing %d records to %s\n", events.size(), fileName);
	writeLogicDataFile(fileName, events);
}

void EngineTestHelper::writeEvents2(const char *fileName) {
	const auto& events = getCompositeEvents();
	if (events.size() < 2) {
		printf("Not enough data for %s\n", fileName);
		return;
	}
	printf("Writing %d records to %s\n", events.size(), fileName);
	writeEventsToFile(fileName, events);
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
	LogTriggerTooth(SHAFT_PRIMARY_RISING, nowNt);
	handleShaftSignal(0, true, nowNt);
}

void EngineTestHelper::firePrimaryTriggerFall() {
	efitick_t nowNt = getTimeNowNt();
	LogTriggerTooth(SHAFT_PRIMARY_FALLING, nowNt);
	handleShaftSignal(0, false, nowNt);
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
	engine.scheduler.executeAll(99999999); // this is needed to clear 'isScheduled' flag
	ASSERT_EQ( 0,  engine.scheduler.size()) << "Failed to clearQueue";
}

int EngineTestHelper::executeActions() {
	return engine.scheduler.executeAll(getTimeNowUs());
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
	advanceTimeUs(deltaTimeUs);
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
	setTimeAndInvokeEventsUs(getTimeNowUs() + deltaTimeUs);
}

void EngineTestHelper::setTimeNtAndInvokeCallBacks(efitick_t nt)
{
	// we need to call fast callback every FAST_CALLBACK_PERIOD_MS
	efitick_t step = MS2US(FAST_CALLBACK_PERIOD_MS) * US_TO_NT_MULTIPLIER;

	while (getTimeNowNt() < nt) {
		// get next FAST_CALLBACK_PERIOD_MS tick time
		efitick_t nextStep = (getTimeNowNt() + step) / step * step;

		if (nextStep > nt) {
			setTimeNowNt(nt);
			return;
		}

		setTimeNowNt(nextStep);
		engine.periodicFastCallback();
	}
}

void EngineTestHelper::setTimeAndInvokeEventsUs(int targetTimeUs) {
	int counter = 0;
	while (true) {
		criticalAssertVoid(counter++ < 100'000, "EngineTestHelper: failing to setTimeAndInvokeEventsUs");
		scheduling_s* nextScheduledEvent = engine.scheduler.getHead();
		if (nextScheduledEvent == nullptr) {
			// nothing pending - we are done here
			break;
		}
		efitick_t nextEventNt = nextScheduledEvent->getMomentNt();
		if (nextEventNt > US2NT(targetTimeUs)) {
			// next event is too far in the future
			break;
		}
		// see #8725 for details
		if (unitTestTaskNoFastCallWhileAdvancingTimeHack) {
			setTimeNowNt(nextEventNt);
		} else {
			setTimeNtAndInvokeCallBacks(nextEventNt);
		}
		engine.scheduler.executeAllNt(getTimeNowNt());
	}
	if (unitTestTaskNoFastCallWhileAdvancingTimeHack) {
		setTimeNowUs(targetTimeUs);
	} else {
		setTimeNtAndInvokeCallBacks(US_TO_NT_MULTIPLIER * targetTimeUs);
	}
}

void EngineTestHelper::fireTriggerEvents(int count) {
	fireTriggerEvents2(count, 5); // 5ms
}

void EngineTestHelper::assertInjectorUpEvent(const char *msg, int eventIndex, efitimeus_t momentUs, long injectorIndex) {
	InjectionEvent *event = &engine.injectionEvents.elements[injectorIndex];
	auto const expected_action{ action_s::make<turnInjectionPinHigh>(uintptr_t{}) };
	assertEvent(msg, eventIndex, expected_action, momentUs, event);
}

void EngineTestHelper::assertInjectorDownEvent(const char *msg, int eventIndex, efitimeus_t momentUs, long injectorIndex) {
	InjectionEvent *event = &engine.injectionEvents.elements[injectorIndex];
	auto const expected_action{ action_s::make<turnInjectionPinLow>((InjectionEvent*){}) };
	assertEvent(msg, eventIndex, expected_action, momentUs, event);
}

scheduling_s * EngineTestHelper::assertEvent5(const char *msg, int index, action_s const& action_expected, efitimeus_t expectedTimestamp) {
	TestExecutor *executor = &engine.scheduler;
	EXPECT_TRUE(executor->size() > index) << msg << " valid index";
	scheduling_s *event = executor->getForUnitTest(index);
	assert(event != nullptr);

	auto const& action_scheduled{ event->action };

	EXPECT_EQ(action_scheduled.getCallback(), action_expected.getCallback()) << msg << " callback up/down";
	efitimeus_t start = getTimeNowUs();
	EXPECT_NEAR(expectedTimestamp, event->getMomentUs() - start,/*3us precision to address rounding etc*/3) << msg;
	return event;
}

angle_t EngineTestHelper::timeToAngle(float timeMs) {
  return MS2US(timeMs) / engine.rpmCalculator.oneDegreeUs;
}

const AngleBasedEvent * EngineTestHelper::assertTriggerEvent(const char *msg,
		int index, AngleBasedEvent *expected,
		action_s const& action_expected,
		angle_t enginePhase) {
	auto event = engine.module<TriggerScheduler>()->getElementAtIndexForUnitTest(index);

	if (action_expected) {
		auto const& action_scheduled{ event->action };
		EXPECT_EQ(action_scheduled.getCallback(), action_expected.getCallback()) << " callback up/down";
	}

	EXPECT_NEAR(enginePhase, event->getAngle(), EPS4D) << " angle";
	return event;
}

scheduling_s * EngineTestHelper::assertScheduling(const char *msg, int index, scheduling_s *expected, action_s const& action, efitimeus_t expectedTimestamp) {
	scheduling_s * actual = assertEvent5(msg, index, action, expectedTimestamp);
	return actual;
}

void EngineTestHelper::assertEvent(const char *msg, int index, action_s const& action, efitimeus_t momentUs, InjectionEvent *expectedEvent) {
	scheduling_s *event = assertEvent5(msg, index, action, momentUs);

	auto const actualEvent{ event->action.getArgument<InjectionEvent*>() };

	ASSERT_EQ(expectedEvent->outputs[0], actualEvent->outputs[0]) << msg;
// but this would not work	assertEqualsLM(msg, expectedPair, (long)eventPair);
}

bool EngineTestHelper::assertEventExistsAtEnginePhase(const char *msg, action_s const& action_expected, angle_t expectedEventEnginePhase){
	TestExecutor *executor = &engine.scheduler;

	//std::cout << "executor->size():              " << executor->size() << std::endl;
	//std::cout << "expected_action.getCallback():  0x" << std::hex << reinterpret_cast<size_t>(action_expected.getCallback()) << "; name: " << action_expected.getCallbackName() << std::endl;

	for (int i = 0; i < executor->size(); i++) {
		auto event = executor->getForUnitTest(i);
		assert(event != nullptr);

		auto const action_scheduled{ event->action };

		// Uncomment next to see what was stored in executor queue
		// std::cout << "action_scheduled.getCallback(): 0x" << std::hex << reinterpret_cast<size_t>(action_scheduled.getCallback()) << "; name: " << action_scheduled.getCallbackName() << std::endl;

		if(action_scheduled.getCallback() == action_expected.getCallback()) {
			efitimeus_t start = getTimeNowUs();
			efitimeus_t expectedTimestamp = angleToTimeUs(expectedEventEnginePhase);
			// after #7245 we can increase the resolution of this test for expect 0.5 or less
			EXPECT_NEAR( expectedTimestamp, event->getMomentUs() - start, angleToTimeUs( 1 ) )
                            << "Expected angle: " << expectedEventEnginePhase << " but got " << (event->getMomentUs() - start) / engine.rpmCalculator.oneDegreeUs << " -- "
                            << msg;
			return true;
		}
	}
	return false;
}

void EngineTestHelper::spin60_2UntilDeg(struct testSpinEngineUntilData& spinInfo, int targetRpm, float targetDegree) {
  	volatile float tick_per_deg = 6000 * 60 / 360 / (float)targetRpm;
	constexpr float tooth_per_deg = 360 / 60;

	size_t targetTooth = (targetDegree - spinInfo.currentDegree) / tooth_per_deg;

	for (size_t i = 0; i < targetTooth; i++) {
		if (spinInfo.currentTooth < 30 || spinInfo.currentTooth > 31) {
			smartFireTriggerEvents2(1 /* count */, tick_per_deg /*ms*/);
		}

		if (spinInfo.currentTooth == 30) {
			// now fire missed tooth rise/fall
    		fireRise(tick_per_deg * 5 /*ms*/);
    		fireFall(tick_per_deg);
    		executeActions();
		}

		if (spinInfo.currentTooth > 58) {
            spinInfo.currentTooth = 0;
		}

		spinInfo.currentTooth++;
	}
}

void EngineTestHelper::applyTriggerWaveform() {
	engine.updateTriggerConfiguration();

	incrementGlobalConfigurationVersion("helper");
}

// todo: open question if this is worth a helper method or should be inlined?
void EngineTestHelper::assertRpm(int expectedRpm, const char *msg) {
	EXPECT_NEAR(expectedRpm, Sensor::getOrZero(SensorType::Rpm), 0.01) << msg;
}

void setupSimpleTestEngineWithMaf(EngineTestHelper *eth, injection_mode_e injectionMode,
		trigger_type_e trigger) {
	engineConfiguration->isIgnitionEnabled = false; // let's focus on injection
	engineConfiguration->cylindersCount = 4;
	// a bit of flexibility - the mode may be changed by some tests
	engineConfiguration->injectionMode = injectionMode;
	// set cranking mode (it's used by getCurrentInjectionMode())
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;

	setArrayValues(config->cltFuelCorrBins, 1.0f);
	setFlatInjectorLag(0.0);
	// this is needed to update injectorLag
	engine->updateSlowSensors();

	ASSERT_EQ( 0,  engine->triggerCentral.isTriggerConfigChanged()) << "trigger #1";
	eth->setTriggerType(trigger);
}

void EngineTestHelper::setTriggerType(trigger_type_e trigger) {
	engineConfiguration->trigger.type = trigger;
	incrementGlobalConfigurationVersion();
	ASSERT_EQ( 1, engine.triggerCentral.isTriggerConfigChanged()) << "trigger #2";
	applyTriggerWaveform();
}

void setupSimpleTestEngineWithMafAndTT_ONE_trigger(EngineTestHelper *eth, injection_mode_e injectionMode) {
	setCamOperationMode();
	setupSimpleTestEngineWithMaf(eth, injectionMode, trigger_type_e::TT_HALF_MOON);
}

void setVerboseTrigger(bool isEnabled) {
	printTriggerDebug = isEnabled;
	printTriggerTrace = isEnabled;
}

warningBuffer_t * getRecentWarnings() {
  return &engine->engineState.warnings.recentWarnings;
}
