/**
 * @file	engine.cpp
 *
 *
 * This might be a http://en.wikipedia.org/wiki/God_object but that's best way I can
 * express myself in C/C++. I am open for suggestions :)
 *
 * @date May 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "global.h"
#include "engine.h"
#include "engine_state.h"
#include "efiGpio.h"
#include "trigger_central.h"
#include "fuel_math.h"
#include "engine_math.h"
#include "advance_map.h"
#include "speed_density.h"
#include "advance_map.h"
#include "efilib2.h"
#include "settings.h"
#include "aux_valves.h"
#include "map_averaging.h"
#include "fsio_impl.h"

#if EFI_PROD_CODE || defined(__DOXYGEN__)
#include "injector_central.h"
#else
#define isRunningBenchTest() true
#endif /* EFI_PROD_CODE */

static TriggerState initState CCM_OPTIONAL;

LoggingWithStorage engineLogger("engine");

extern int globalConfigurationVersion;


EXTERN_ENGINE
;

#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_TUNER_STUDIO */

FsioState::FsioState() {
#if EFI_ENABLE_ENGINE_WARNING
	isEngineWarning = FALSE;
#endif
#if EFI_ENABLE_CRITICAL_ENGINE_STOP
	isCriticalEngineCondition = FALSE;
#endif
}

void Engine::initializeTriggerShape(Logging *logger DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if (EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT) || defined(__DOXYGEN__)
#if !EFI_UNIT_TEST
	// we have a confusing threading model so some synchronization would not hurt
	bool alreadyLocked = lockAnyContext();
#endif /* EFI_UNIT_TEST */

	TRIGGER_SHAPE(initializeTriggerShape(logger,
			engineConfiguration->operationMode,
			engineConfiguration->useOnlyRisingEdgeForTrigger, &engineConfiguration->trigger));

	if (!TRIGGER_SHAPE(shapeDefinitionError)) {
		/**
	 	 * this instance is used only to initialize 'this' TriggerShape instance
	 	 * #192 BUG real hardware trigger events could be coming even while we are initializing trigger
	 	 */
		initState.resetTriggerState();
		calculateTriggerSynchPoint(&ENGINE(triggerCentral.triggerShape),
				&initState PASS_ENGINE_PARAMETER_SUFFIX);

		if (engine->triggerCentral.triggerShape.getSize() == 0) {
			firmwareError(CUSTOM_ERR_TRIGGER_ZERO, "triggerShape size is zero");
		}
		engine->engineCycleEventCount = TRIGGER_SHAPE(getLength());
	}

#if !EFI_UNIT_TEST
	if (!alreadyLocked) {
		unlockAnyContext();
	}
#endif /* EFI_UNIT_TEST */

	if (!TRIGGER_SHAPE(shapeDefinitionError)) {
		prepareOutputSignals(PASS_ENGINE_PARAMETER_SIGNATURE);
	}
#endif /* EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT */
}

static void cylinderCleanupControl(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
	bool newValue;
	if (engineConfiguration->isCylinderCleanupEnabled) {
		newValue = !engine->rpmCalculator.isRunning(PASS_ENGINE_PARAMETER_SIGNATURE) && getTPS(PASS_ENGINE_PARAMETER_SIGNATURE) > CLEANUP_MODE_TPS;
	} else {
		newValue = false;
	}
	if (newValue != engine->isCylinderCleanupMode) {
		engine->isCylinderCleanupMode = newValue;
		scheduleMsg(&engineLogger, "isCylinderCleanupMode %s", boolToString(newValue));
	}
#endif
}

void Engine::periodicSlowCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	watchdog();
	updateSlowSensors(PASS_ENGINE_PARAMETER_SIGNATURE);
	checkShutdown();

#if EFI_FSIO || defined(__DOXYGEN__)
	runFsio(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif /* EFI_PROD_CODE && EFI_FSIO */

	cylinderCleanupControl(PASS_ENGINE_PARAMETER_SIGNATURE);

	slowCallBackWasInvoked = TRUE;
}


/**
 * We are executing these heavy (logarithm) methods from outside the trigger callbacks for performance reasons.
 * See also periodicFastCallback
 */
void Engine::updateSlowSensors(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
	int rpm = GET_RPM();
	isEngineChartEnabled = CONFIG(isEngineChartEnabled) && rpm < CONFIG(engineSnifferRpmThreshold);
	sensorChartMode = rpm < CONFIG(sensorSnifferRpmThreshold) ? CONFIGB(sensorChartMode) : SC_OFF;

	engineState.updateSlowSensors(PASS_ENGINE_PARAMETER_SIGNATURE);

	// todo: move this logic somewhere to sensors folder?
	if (CONFIG(fuelLevelSensor) != EFI_ADC_NONE) {
		float fuelLevelVoltage = getVoltageDivided("fuel", engineConfiguration->fuelLevelSensor);
		sensors.fuelTankLevel = interpolateMsg("fgauge", CONFIGB(fuelLevelEmptyTankVoltage), 0,
				CONFIGB(fuelLevelFullTankVoltage), 100,
				fuelLevelVoltage);
	}
	sensors.vBatt = hasVBatt(PASS_ENGINE_PARAMETER_SIGNATURE) ? getVBatt(PASS_ENGINE_PARAMETER_SIGNATURE) : 12;

	engineState.injectorLag = getInjectorLag(sensors.vBatt PASS_ENGINE_PARAMETER_SUFFIX);
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
bool Engine::needToStopEngine(efitick_t nowNt) {
	return stopEngineRequestTimeNt != 0 &&
			nowNt - stopEngineRequestTimeNt	< 3 * US2NT(US_PER_SECOND_LL);
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

	memset(&m, 0, sizeof(m));
}


/**
 * Here we have a bunch of stuff which should invoked after configuration change
 * so that we can prepare some helper structures
 */
void Engine::preCalculate(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	sparkTable.preCalc(engineConfiguration->sparkDwellRpmBins,
			engineConfiguration->sparkDwellValues);

#if HAL_USE_ADC
	adcToVoltageInputDividerCoefficient = adcToVolts(1) * engineConfiguration->analogInputDividerCoefficient;
#else
	adcToVoltageInputDividerCoefficient = engineConfigurationPtr->analogInputDividerCoefficient;
#endif

	/**
	 * Here we prepare a fast, index-based MAF lookup from a slower curve description
	 */
	for (int i = 0; i < MAF_DECODING_CACHE_SIZE; i++) {
		float volts = i / MAF_DECODING_CACHE_MULT;
		float maf = interpolate2d("maf", volts, config->mafDecodingBins,
				config->mafDecoding, MAF_DECODING_COUNT);
		mafDecodingLookup[i] = maf;
	}
}

void Engine::setConfig(persistent_config_s *config) {
	this->config = config;
	engineConfigurationPtr = &config->engineConfiguration;
	memset(config, 0, sizeof(persistent_config_s));
	engineState.warmupAfrPid.initPidClass(&config->engineConfiguration.warmupAfrPid);
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
#if EFI_PROD_CODE || EFI_SIMULATOR || defined(__DOXYGEN__)
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

void Engine::checkShutdown() {
#if EFI_MAIN_RELAY_CONTROL || defined(__DOXYGEN__)
	int rpm = rpmCalculator.getRpm();

	/**
	 * Something is weird here: "below 5.0 volts on battery" what is it about? Is this about
	 * Frankenso powering everything while driver has already turned ignition off? or what is this condition about?
	 */
	const float vBattThreshold = 5.0f;
	if (isValidRpm(rpm) && sensors.vBatt < vBattThreshold && stopEngineRequestTimeNt == 0) {
		scheduleStopEngine();
		// todo: add stepper motor parking
	}
#endif /* EFI_MAIN_RELAY_CONTROL */
}

bool Engine::isInShutdownMode() {
#if EFI_MAIN_RELAY_CONTROL || defined(__DOXYGEN__)
	if (stopEngineRequestTimeNt == 0)	// the shutdown procedure is not started
		return false;
	
	const efitime_t engineStopWaitTimeoutNt = 5LL * 1000000LL;
	// The engine is still spinning! Give it some time to stop (but wait no more than 5 secs)
	if (isSpinning && (getTimeNowNt() - stopEngineRequestTimeNt) < US2NT(engineStopWaitTimeoutNt))
		return true;
	// todo: add checks for stepper motor parking
#endif /* EFI_MAIN_RELAY_CONTROL */
	return false;
}

injection_mode_e Engine::getCurrentInjectionMode(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return rpmCalculator.isCranking(PASS_ENGINE_PARAMETER_SIGNATURE) ? CONFIG(crankingInjectionMode) : CONFIG(injectionMode);
}

/**
 * The idea of this method is to execute all heavy calculations in a lower-priority thread,
 * so that trigger event handler/IO scheduler tasks are faster.
 */
void Engine::periodicFastCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE) {

#if EFI_MAP_AVERAGING
	refreshMapAveragingPreCalc(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif

	engineState.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	engine->m.beforeFuelCalc = GET_TIMESTAMP();
	int rpm = GET_RPM();
	ENGINE(injectionDuration) = getInjectionDuration(rpm PASS_ENGINE_PARAMETER_SUFFIX);
	engine->m.fuelCalcTime = GET_TIMESTAMP() - engine->m.beforeFuelCalc;

}

void doScheduleStopEngine(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engine->stopEngineRequestTimeNt = getTimeNowNt();
	// let's close injectors or else if these happen to be open right now
	enginePins.stopPins();
}
