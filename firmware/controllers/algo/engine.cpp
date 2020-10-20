/**
 * @file	engine.cpp
 *
 *
 * This might be a http://en.wikipedia.org/wiki/God_object but that's best way I can
 * express myself in C/C++. I am open for suggestions :)
 *
 * @date May 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine.h"
#include "allsensors.h"
#include "efi_gpio.h"
#include "trigger_central.h"
#include "fuel_math.h"
#include "engine_math.h"
#include "advance_map.h"
#include "speed_density.h"
#include "advance_map.h"
#include "os_util.h"
#include "settings.h"
#include "aux_valves.h"
#include "map_averaging.h"
#include "fsio_impl.h"
#include "perf_trace.h"
#include "backup_ram.h"
#include "idle_thread.h"
#include "sensor.h"
#include "gppwm.h"
#include "tachometer.h"
#if EFI_MC33816
 #include "mc33816.h"
#endif // EFI_MC33816

#if EFI_TUNER_STUDIO
#include "tunerstudio_outputs.h"
#endif /* EFI_TUNER_STUDIO */

#if EFI_PROD_CODE
#include "trigger_emulator_algo.h"
#include "bench_test.h"
#else
#define isRunningBenchTest() true
#endif /* EFI_PROD_CODE */

#if (BOARD_TLE8888_COUNT > 0)
#include "gpio/tle8888.h"
#endif

LoggingWithStorage engineLogger("engine");

EXTERN_ENGINE;

#if EFI_ENGINE_SNIFFER
#include "engine_sniffer.h"
extern int waveChartUsedSize;
extern WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */

FsioState::FsioState() {
#if EFI_ENABLE_ENGINE_WARNING
	isEngineWarning = FALSE;
#endif
#if EFI_ENABLE_CRITICAL_ENGINE_STOP
	isCriticalEngineCondition = FALSE;
#endif
}

void Engine::resetEngineSnifferIfInTestMode() {
#if EFI_ENGINE_SNIFFER
	if (isFunctionalTestMode) {
		// TODO: what is the exact reasoning for the exact engine sniffer pause time I wonder
		waveChart.pauseEngineSnifferUntilNt = getTimeNowNt() + MS2NT(300);
		waveChart.reset();
	}
#endif /* EFI_ENGINE_SNIFFER */
}

trigger_type_e getVvtTriggerType(vvt_mode_e vvtMode) {
	switch (vvtMode) {
	case VVT_2JZ:
		return TT_VVT_JZ;
	case MIATA_NB2:
		return TT_VVT_MIATA_NB2;
	case VVT_BOSCH_QUICK_START:
		return TT_VVT_BOSCH_QUICK_START;
	case VVT_FIRST_HALF:
		return TT_ONE;
	case VVT_SECOND_HALF:
		return TT_ONE;
	case VVT_4_1:
		return TT_ONE;
	default:
		return TT_ONE;
	}
}

void Engine::initializeTriggerWaveform(Logging *logger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	static TriggerState initState;
	INJECT_ENGINE_REFERENCE(&initState);

	// Re-read config in case it's changed
	primaryTriggerConfiguration.update();
	vvtTriggerConfiguration.update();

#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
	// we have a confusing threading model so some synchronization would not hurt
	bool alreadyLocked = lockAnyContext();

	TRIGGER_WAVEFORM(initializeTriggerWaveform(logger,
			engineConfiguration->ambiguousOperationMode,
			engineConfiguration->useOnlyRisingEdgeForTrigger, &engineConfiguration->trigger));

	if (!TRIGGER_WAVEFORM(shapeDefinitionError)) {
		/**
	 	 * 'initState' instance of TriggerState is used only to initialize 'this' TriggerWaveform instance
	 	 * #192 BUG real hardware trigger events could be coming even while we are initializing trigger
	 	 */
		calculateTriggerSynchPoint(ENGINE(triggerCentral.triggerShape),
				initState PASS_ENGINE_PARAMETER_SUFFIX);

		engine->engineCycleEventCount = TRIGGER_WAVEFORM(getLength());
	}


	if (engineConfiguration->vvtMode != VVT_INACTIVE) {
		trigger_config_s config;
		ENGINE(triggerCentral).vvtTriggerType = config.type = getVvtTriggerType(engineConfiguration->vvtMode);

		ENGINE(triggerCentral).vvtShape.initializeTriggerWaveform(logger,
				engineConfiguration->ambiguousOperationMode,
				engine->engineConfigurationPtr->vvtCamSensorUseRise, &config);

		ENGINE(triggerCentral).vvtShape.initializeSyncPoint(initState,
				engine->vvtTriggerConfiguration,
				config);
	}

	if (!alreadyLocked) {
		unlockAnyContext();
	}

	if (!TRIGGER_WAVEFORM(shapeDefinitionError)) {
		prepareOutputSignals(PASS_ENGINE_PARAMETER_SIGNATURE);
	}
#endif /* EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT */
}

static void cylinderCleanupControl(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_ENGINE_CONTROL
	bool newValue;
	if (engineConfiguration->isCylinderCleanupEnabled) {
		newValue = !engine->rpmCalculator.isRunning() && Sensor::get(SensorType::DriverThrottleIntent).value_or(0) > CLEANUP_MODE_TPS;
	} else {
		newValue = false;
	}
	if (newValue != engine->isCylinderCleanupMode) {
		engine->isCylinderCleanupMode = newValue;
		scheduleMsg(&engineLogger, "isCylinderCleanupMode %s", boolToString(newValue));
	}
#endif
}

#if HW_CHECK_MODE
static void assertCloseTo(const char * msg, float actual, float expected) {
	if (actual < 0.75 * expected || actual > 1.25 * expected) {
		firmwareError(OBD_PCM_Processor_Fault, "%s analog input validation failed %f vs %f", msg, actual, expected);
	}
}
#endif // HW_CHECK_MODE

void Engine::periodicSlowCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	ScopePerf perf(PE::EnginePeriodicSlowCallback);

	// Re-read config in case it's changed
	primaryTriggerConfiguration.update();
	vvtTriggerConfiguration.update();

	watchdog();
	updateSlowSensors(PASS_ENGINE_PARAMETER_SIGNATURE);
	checkShutdown(PASS_ENGINE_PARAMETER_SIGNATURE);

#if EFI_FSIO
	runFsio(PASS_ENGINE_PARAMETER_SIGNATURE);
#else
	runHardcodedFsio(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif /* EFI_FSIO */

	updateGppwm();

	cylinderCleanupControl(PASS_ENGINE_PARAMETER_SIGNATURE);

	standardAirCharge = getStandardAirCharge(PASS_ENGINE_PARAMETER_SIGNATURE);

#if (BOARD_TLE8888_COUNT > 0)
	static efitick_t tle8888CrankingResetTime = 0;

	if (CONFIG(useTLE8888_cranking_hack) && ENGINE(rpmCalculator).isCranking()) {
		efitick_t nowNt = getTimeNowNt();
		if (nowNt - tle8888CrankingResetTime > MS2NT(300)) {
			requestTLE8888initialization();
			// let's reset TLE8888 every 300ms while cranking since that's the best we can do to deal with undervoltage reset
			// PS: oh yes, it's a horrible design! Please suggest something better!
			tle8888CrankingResetTime = nowNt;
		}
	}
#endif

	slowCallBackWasInvoked = true;

#if HW_CHECK_MODE
	efiAssertVoid(OBD_PCM_Processor_Fault, CONFIG(clt).adcChannel != EFI_ADC_NONE, "No CLT setting");
	efitimesec_t secondsNow = getTimeNowSeconds();
	if (secondsNow > 2 && secondsNow < 180) {
		assertCloseTo("RPM", Sensor::get(SensorType::Rpm).Value, HW_CHECK_RPM);
	} else if (!hasFirmwareError() && secondsNow > 180) {
		static bool isHappyTest = false;
		if (!isHappyTest) {
			setTriggerEmulatorRPM(5 * HW_CHECK_RPM);
			scheduleMsg(&engineLogger, "TEST PASSED");
			isHappyTest = true;
		}
	}
	assertCloseTo("clt", Sensor::get(SensorType::Clt).Value, 49.3);
	assertCloseTo("iat", Sensor::get(SensorType::Iat).Value, 73.2);
	assertCloseTo("aut1", Sensor::get(SensorType::AuxTemp1).Value, 13.8);
	assertCloseTo("aut2", Sensor::get(SensorType::AuxTemp2).Value, 6.2);
#endif // HW_CHECK_MODE
}


#if (BOARD_TLE8888_COUNT > 0)
extern float vBattForTle8888;
#endif /* BOARD_TLE8888_COUNT */

/**
 * We are executing these heavy (logarithm) methods from outside the trigger callbacks for performance reasons.
 * See also periodicFastCallback
 */
void Engine::updateSlowSensors(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_ENGINE_CONTROL
	int rpm = GET_RPM();
	isEngineChartEnabled = CONFIG(isEngineChartEnabled) && rpm < CONFIG(engineSnifferRpmThreshold);
	sensorChartMode = rpm < CONFIG(sensorSnifferRpmThreshold) ? CONFIG(sensorChartMode) : SC_OFF;

	engineState.updateSlowSensors(PASS_ENGINE_PARAMETER_SIGNATURE);

	// todo: move this logic somewhere to sensors folder?
	if (CONFIG(fuelLevelSensor) != EFI_ADC_NONE) {
		float fuelLevelVoltage = getVoltageDivided("fuel", engineConfiguration->fuelLevelSensor PASS_ENGINE_PARAMETER_SUFFIX);
		sensors.fuelTankLevel = interpolateMsg("fgauge", CONFIG(fuelLevelEmptyTankVoltage), 0,
				CONFIG(fuelLevelFullTankVoltage), 100,
				fuelLevelVoltage);
	}
	sensors.vBatt = hasVBatt(PASS_ENGINE_PARAMETER_SIGNATURE) ? getVBatt(PASS_ENGINE_PARAMETER_SIGNATURE) : 12;

#if (BOARD_TLE8888_COUNT > 0)
	// nasty value injection into C driver which would not be able to access Engine class
	vBattForTle8888 = sensors.vBatt;
#endif /* BOARD_TLE8888_COUNT */

#if EFI_MC33816
	initMc33816IfNeeded();
#endif // EFI_MC33816

	engineState.running.injectorLag = getInjectorLag(sensors.vBatt PASS_ENGINE_PARAMETER_SUFFIX);
#endif
}

void Engine::onTriggerSignalEvent(efitick_t nowNt) {
	isSpinning = true;
	lastTriggerToothEventTimeNt = nowNt;
}

Engine::Engine() {
	reset();
}

Engine::Engine(persistent_config_s *config) {
	setConfig(config);
	reset();
}

/**
 * @see scheduleStopEngine()
 * @return true if there is a reason to stop engine
 */
bool Engine::needToStopEngine(efitick_t nowNt) const {
	return stopEngineRequestTimeNt != 0 &&
			nowNt - stopEngineRequestTimeNt	< 3 * NT_PER_SECOND;
}

int Engine::getGlobalConfigurationVersion(void) const {
	return globalConfigurationVersion;
}

void Engine::reset() {
	/**
	 * it's important for fixAngle() that engineCycle field never has zero
	 */
	engineCycle = getEngineCycle(FOUR_STROKE_CRANK_SENSOR);
	memset(&ignitionPin, 0, sizeof(ignitionPin));
}


/**
 * Here we have a bunch of stuff which should invoked after configuration change
 * so that we can prepare some helper structures
 */
void Engine::preCalculate(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
// todo: start using this 'adcToVoltageInputDividerCoefficient' micro-optimization or... throw it away?
#if HAL_USE_ADC
	adcToVoltageInputDividerCoefficient = adcToVolts(1) * engineConfiguration->analogInputDividerCoefficient;
#else
	adcToVoltageInputDividerCoefficient = engineConfigurationPtr->analogInputDividerCoefficient;
#endif

#if EFI_TUNER_STUDIO
	// we take 2 bytes of crc32, no idea if it's right to call it crc16 or not
	// we have a hack here - we rely on the fact that engineMake is the first of three relevant fields
	tsOutputChannels.engineMakeCodeNameCrc16 = crc32(engineConfiguration->engineMake, 3 * VEHICLE_INFO_SIZE);

	// we need and can empty warning message for CRC purposes
	memset(engine->config->warning_message, 0, sizeof(error_message_t));
	tsOutputChannels.tuneCrc16 = crc32(engine->config, sizeof(persistent_config_s));
#endif /* EFI_TUNER_STUDIO */
}

#if EFI_SHAFT_POSITION_INPUT
void Engine::OnTriggerStateDecodingError() {
	Engine *engine = this;
	EXPAND_Engine;

	warning(CUSTOM_SYNC_COUNT_MISMATCH, "trigger not happy current %d/%d/%d expected %d/%d/%d",
			triggerCentral.triggerState.currentCycle.eventCount[0],
			triggerCentral.triggerState.currentCycle.eventCount[1],
			triggerCentral.triggerState.currentCycle.eventCount[2],
			TRIGGER_WAVEFORM(expectedEventCount[0]),
			TRIGGER_WAVEFORM(expectedEventCount[1]),
			TRIGGER_WAVEFORM(expectedEventCount[2]));
	triggerCentral.triggerState.setTriggerErrorState();


	triggerCentral.triggerState.totalTriggerErrorCounter++;
	if (CONFIG(verboseTriggerSynchDetails) || (triggerCentral.triggerState.someSortOfTriggerError && !CONFIG(silentTriggerError))) {
#if EFI_PROD_CODE
		scheduleMsg(&engineLogger, "error: synchronizationPoint @ index %d expected %d/%d/%d got %d/%d/%d",
				triggerCentral.triggerState.currentCycle.current_index,
				TRIGGER_WAVEFORM(expectedEventCount[0]),
				TRIGGER_WAVEFORM(expectedEventCount[1]),
				TRIGGER_WAVEFORM(expectedEventCount[2]),
				triggerCentral.triggerState.currentCycle.eventCount[0],
				triggerCentral.triggerState.currentCycle.eventCount[1],
				triggerCentral.triggerState.currentCycle.eventCount[2]);
#endif /* EFI_PROD_CODE */
	}

}

void Engine::OnTriggerStateProperState(efitick_t nowNt) {
	Engine *engine = this;
	EXPAND_Engine;

#if EFI_SHAFT_POSITION_INPUT
	triggerCentral.triggerState.runtimeStatistics(&triggerCentral.triggerFormDetails, nowNt PASS_ENGINE_PARAMETER_SUFFIX);
#endif /* EFI_SHAFT_POSITION_INPUT */

	rpmCalculator.setSpinningUp(nowNt);
}

void Engine::OnTriggerSynchronizationLost() {
	Engine *engine = this;
	EXPAND_Engine;

	// Needed for early instant-RPM detection
	engine->rpmCalculator.setStopSpinning();
}

void Engine::OnTriggerInvalidIndex(int currentIndex) {
	Engine *engine = this;
	EXPAND_Engine;
	// let's not show a warning if we are just starting to spin
	if (GET_RPM() != 0) {
		warning(CUSTOM_SYNC_ERROR, "sync error: index #%d above total size %d", currentIndex, triggerCentral.triggerShape.getSize());
		triggerCentral.triggerState.setTriggerErrorState();
	}
}

void Engine::OnTriggerSyncronization(bool wasSynchronized) {
	// We only care about trigger shape once we have synchronized trigger. Anything could happen
	// during first revolution and it's fine
	if (wasSynchronized) {
		Engine *engine = this;
		EXPAND_Engine;

		/**
	 	 * We can check if things are fine by comparing the number of events in a cycle with the expected number of event.
	 	 */
		bool isDecodingError = triggerCentral.triggerState.validateEventCounters(triggerCentral.triggerShape);

		enginePins.triggerDecoderErrorPin.setValue(isDecodingError);

		// 'triggerStateListener is not null' means we are running a real engine and now just preparing trigger shape
		// that's a bit of a hack, a sweet OOP solution would be a real callback or at least 'needDecodingErrorLogic' method?
		if (isDecodingError) {
			OnTriggerStateDecodingError();
		}

		engine->triggerErrorDetection.add(isDecodingError);

		if (isTriggerDecoderError(PASS_ENGINE_PARAMETER_SIGNATURE)) {
			warning(CUSTOM_OBD_TRG_DECODING, "trigger decoding issue. expected %d/%d/%d got %d/%d/%d",
					TRIGGER_WAVEFORM(expectedEventCount[0]), TRIGGER_WAVEFORM(expectedEventCount[1]),
					TRIGGER_WAVEFORM(expectedEventCount[2]),
					triggerCentral.triggerState.currentCycle.eventCount[0],
					triggerCentral.triggerState.currentCycle.eventCount[1],
					triggerCentral.triggerState.currentCycle.eventCount[2]);
		}
	}

}
#endif

void Engine::injectEngineReferences() {
	Engine *engine = this;
	EXPAND_Engine;

	INJECT_ENGINE_REFERENCE(&primaryTriggerConfiguration);
	INJECT_ENGINE_REFERENCE(&vvtTriggerConfiguration);

	primaryTriggerConfiguration.update();
	vvtTriggerConfiguration.update();
	triggerCentral.init(PASS_ENGINE_PARAMETER_SIGNATURE);
}

void Engine::setConfig(persistent_config_s *config) {
	this->config = config;
	engineConfigurationPtr = &config->engineConfiguration;
	memset(config, 0, sizeof(persistent_config_s));

	injectEngineReferences();
}

void Engine::printKnockState(void) {
	scheduleMsg(&engineLogger, "knock now=%s/ever=%s", boolToString(knockNow), boolToString(knockEver));
}

void Engine::knockLogic(float knockVolts DECLARE_ENGINE_PARAMETER_SUFFIX) {
	this->knockVolts = knockVolts;
    knockNow = knockVolts > engineConfiguration->knockVThreshold;
    /**
     * KnockCount is directly proportional to the degrees of ignition
     * advance removed
     * ex: degrees to subtract = knockCount;
     */

    /**
     * TODO use knockLevel as a factor for amount of ignition advance
     * to remove
     * Perhaps allow the user to set a multiplier
     * ex: degrees to subtract = knockCount + (knockLevel * X)
     * X = user configurable multiplier
     */
    if (knockNow) {
        knockEver = true;
        timeOfLastKnockEvent = getTimeNowUs();
        if (knockCount < engineConfiguration->maxKnockSubDeg)
            knockCount++;
    } else if (knockCount >= 1) {
        knockCount--;
	} else {
        knockCount = 0;
    }
}

void Engine::watchdog() {
#if EFI_ENGINE_CONTROL
	if (isRunningPwmTest)
		return;
	if (!isSpinning) {
		if (!isRunningBenchTest() && enginePins.stopPins()) {
			// todo: make this a firmwareError assuming functional tests would run
			warning(CUSTOM_ERR_2ND_WATCHDOG, "Some pins were turned off by 2nd pass watchdog");
		}
		return;
	}
	efitick_t nowNt = getTimeNowNt();
// note that we are ignoring the number of tooth here - we
// check for duration between tooth as if we only have one tooth per revolution which is not the case
#define REVOLUTION_TIME_HIGH_THRESHOLD (60 * 1000000LL / RPM_LOW_THRESHOLD)
	/**
	 * todo: better watch dog implementation should be implemented - see
	 * http://sourceforge.net/p/rusefi/tickets/96/
	 *
	 * note that the result of this subtraction could be negative, that would happen if
	 * we have a trigger event between the time we've invoked 'getTimeNow' and here
	 */
	efitick_t timeSinceLastTriggerEvent = nowNt - lastTriggerToothEventTimeNt;
	if (timeSinceLastTriggerEvent < US2NT(REVOLUTION_TIME_HIGH_THRESHOLD)) {
		return;
	}
	isSpinning = false;
	ignitionEvents.isReady = false;
#if EFI_PROD_CODE || EFI_SIMULATOR
	scheduleMsg(&engineLogger, "engine has STOPPED");
	scheduleMsg(&engineLogger, "templog engine has STOPPED [%x][%x] [%x][%x] %d",
			(int)(nowNt >> 32), (int)nowNt,
			(int)(lastTriggerToothEventTimeNt >> 32), (int)lastTriggerToothEventTimeNt,
			(int)timeSinceLastTriggerEvent
			);
	triggerInfo();
#endif

	enginePins.stopPins();
#endif
}

void Engine::checkShutdown(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_MAIN_RELAY_CONTROL
	// if we are already in the "ignition_on" mode, then do nothing
	if (ignitionOnTimeNt > 0) {
		return;
	}

	// here we are in the shutdown (the ignition is off) or initial mode (after the firmware fresh start)
	const efitick_t engineStopWaitTimeoutUs = 500000LL;	// 0.5 sec
	// in shutdown mode, we need a small cooldown time between the ignition off and on
	if (stopEngineRequestTimeNt == 0 || (getTimeNowNt() - stopEngineRequestTimeNt) > US2NT(engineStopWaitTimeoutUs)) {
		// if the ignition key is turned on again,
		// we cancel the shutdown mode, but only if all shutdown procedures are complete
		const float vBattThresholdOn = 8.0f;
		if ((sensors.vBatt > vBattThresholdOn) && !isInShutdownMode(PASS_ENGINE_PARAMETER_SIGNATURE)) {
			ignitionOnTimeNt = getTimeNowNt();
			stopEngineRequestTimeNt = 0;
			scheduleMsg(&engineLogger, "Ingition voltage detected! Cancel the engine shutdown!");
		}
	}
#endif /* EFI_MAIN_RELAY_CONTROL */
}

bool Engine::isInShutdownMode(DECLARE_ENGINE_PARAMETER_SIGNATURE) const {
#if EFI_MAIN_RELAY_CONTROL
	// if we are in "ignition_on" mode and not in shutdown mode
	if (stopEngineRequestTimeNt == 0 && ignitionOnTimeNt > 0) {
		const float vBattThresholdOff = 5.0f;
		// start the shutdown process if the ignition voltage dropped low
		if (sensors.vBatt <= vBattThresholdOff) {
			scheduleStopEngine();
		}
	}

	// we are not in the shutdown mode?
	if (stopEngineRequestTimeNt == 0) {
		return false;
	}

	const efitick_t turnOffWaitTimeoutUs = 1LL * 1000000LL;
	// We don't want any transients to step in, so we wait at least 1 second whatever happens.
	// Also it's good to give the stepper motor some time to start moving to the initial position (or parking)
	if ((getTimeNowNt() - stopEngineRequestTimeNt) < US2NT(turnOffWaitTimeoutUs))
		return true;

	const efitick_t engineSpinningWaitTimeoutUs = 5LL * 1000000LL;
	// The engine is still spinning! Give it some time to stop (but wait no more than 5 secs)
	if (isSpinning && (getTimeNowNt() - stopEngineRequestTimeNt) < US2NT(engineSpinningWaitTimeoutUs))
		return true;

	// The idle motor valve is still moving! Give it some time to park (but wait no more than 10 secs)
	// Usually it can move to the initial 'cranking' position or zero 'parking' position.
	const efitick_t idleMotorWaitTimeoutUs = 10LL * 1000000LL;
	if (isIdleMotorBusy(PASS_ENGINE_PARAMETER_SIGNATURE) && (getTimeNowNt() - stopEngineRequestTimeNt) < US2NT(idleMotorWaitTimeoutUs))
		return true;
#endif /* EFI_MAIN_RELAY_CONTROL */
	return false;
}

bool Engine::isMainRelayEnabled(DECLARE_ENGINE_PARAMETER_SIGNATURE) const {
#if EFI_MAIN_RELAY_CONTROL
	return enginePins.mainRelay.getLogicValue();
#else
	// if no main relay control, we assume it's always turned on
	return true;
#endif /* EFI_MAIN_RELAY_CONTROL */
}


float Engine::getTimeIgnitionSeconds(void) const {
	// return negative if the ignition is turned off
	if (ignitionOnTimeNt == 0)
		return -1;
	float numSeconds = (float)NT2US(getTimeNowNt() - ignitionOnTimeNt) / 1000000.0f;
	return numSeconds;
}

injection_mode_e Engine::getCurrentInjectionMode(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return rpmCalculator.isCranking() ? CONFIG(crankingInjectionMode) : CONFIG(injectionMode);
}

// see also in TunerStudio project '[doesTriggerImplyOperationMode] tag
static bool doesTriggerImplyOperationMode(trigger_type_e type) {
	return type != TT_TOOTHED_WHEEL
			&& type != TT_ONE
			&& type != TT_ONE_PLUS_ONE
			&& type != TT_3_1_CAM
			&& type != TT_TOOTHED_WHEEL_60_2
			&& type != TT_TOOTHED_WHEEL_36_1;
}

operation_mode_e Engine::getOperationMode(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	/**
	 * here we ignore user-provided setting for well known triggers.
	 * For instance for Miata NA, there is no reason to allow user to set FOUR_STROKE_CRANK_SENSOR
	 */
	return doesTriggerImplyOperationMode(engineConfiguration->trigger.type) ? triggerCentral.triggerShape.getOperationMode() : engineConfiguration->ambiguousOperationMode;
}

int Engine::getRpmHardLimit(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (engineConfiguration->useFSIO6ForRevLimiter) {
		return fsioState.fsioRpmHardLimit;
	}
	return CONFIG(rpmHardLimit);
}

/**
 * The idea of this method is to execute all heavy calculations in a lower-priority thread,
 * so that trigger event handler/IO scheduler tasks are faster.
 */
void Engine::periodicFastCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	ScopePerf pc(PE::EnginePeriodicFastCallback);

#if EFI_MAP_AVERAGING
	refreshMapAveragingPreCalc(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif

	engineState.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	tachSignalCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
}

void doScheduleStopEngine(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	scheduleMsg(&engineLogger, "Starting doScheduleStopEngine");
	engine->stopEngineRequestTimeNt = getTimeNowNt();
	engine->ignitionOnTimeNt = 0;
	// let's close injectors or else if these happen to be open right now
	enginePins.stopPins();
	// todo: initiate stepper motor parking
	// make sure we have stored all the info
#if EFI_PROD_CODE
	//todo: FIX kinetis build with this line
	//backupRamFlush();
#endif // EFI_PROD_CODE
}
