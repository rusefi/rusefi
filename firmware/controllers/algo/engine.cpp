/**
 * @file	engine.cpp
 *
 *
 * This might be a http://en.wikipedia.org/wiki/God_object but that's best way I can
 * express myself in C/C++. I am open for suggestions :)
 *
 * @date May 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
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

#if EFI_PROD_CODE || defined(__DOXYGEN__)
#include "injector_central.h"
#else
#define isRunningBenchTest() true
#endif /* EFI_PROD_CODE */

static LoggingWithStorage logger("engine");

extern fuel_Map3D_t veMap;
extern afr_Map3D_t afrMap;

EXTERN_ENGINE
;

#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
extern TunerStudioOutputChannels tsOutputChannels;
#endif

MockAdcState::MockAdcState() {
	memset(hasMockAdc, 0, sizeof(hasMockAdc));
}

#if EFI_ENABLE_MOCK_ADC || EFI_SIMULATOR
void MockAdcState::setMockVoltage(int hwChannel, float voltage) {
	scheduleMsg(&logger, "fake voltage: channel %d value %.2f", hwChannel, voltage);

	fakeAdcValues[hwChannel] = voltsToAdc(voltage);
	hasMockAdc[hwChannel] = true;
}
#endif /* EFI_ENABLE_MOCK_ADC */

int MockAdcState::getMockAdcValue(int hwChannel) {
	return fakeAdcValues[hwChannel];
}

/**
 * We are executing these heavy (logarithm) methods from outside the trigger callbacks for performance reasons.
 * See also periodicFastCallback
 */
void Engine::updateSlowSensors(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	int rpm = rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE);
	isEngineChartEnabled = CONFIG(isEngineChartEnabled) && rpm < CONFIG(engineSnifferRpmThreshold);
	sensorChartMode = rpm < CONFIG(sensorSnifferRpmThreshold) ? boardConfiguration->sensorChartMode : SC_OFF;

	engineState.updateSlowSensors(PASS_ENGINE_PARAMETER_SIGNATURE);

	// todo: move this logic somewhere to sensors folder?
	if (engineConfiguration->fuelLevelSensor != EFI_ADC_NONE) {
		float fuelLevelVoltage = getVoltageDivided("fuel", engineConfiguration->fuelLevelSensor);
		sensors.fuelTankGauge = interpolate(boardConfiguration->fuelLevelEmptyTankVoltage, 0,
				boardConfiguration->fuelLevelFullTankVoltage, 100,
				fuelLevelVoltage);
	}
	sensors.vBatt = hasVBatt(PASS_ENGINE_PARAMETER_SIGNATURE) ? getVBatt(PASS_ENGINE_PARAMETER_SIGNATURE) : 12;

	engineState.injectorLag = getInjectorLag(sensors.vBatt PASS_ENGINE_PARAMETER_SUFFIX);
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

Accelerometer::Accelerometer() {
	x = y = z = 0;
}

SensorsState::SensorsState() {
	reset();
}

void SensorsState::reset() {
	fuelTankGauge = vBatt = 0;
	iat = clt = NAN;
}

void Engine::reset() {
	withError = isEngineChartEnabled = false;
	etbAutoTune = false;
	sensorChartMode = SC_OFF;
	actualLastInjection = 0;
	fsioTimingAdjustment = 0;
	isAlternatorControlEnabled = false;
	callFromPitStopEndTime = 0;
	rpmHardLimitTimestamp = 0;
	wallFuelCorrection = 0;
	/**
	 * it's important for fixAngle() that engineCycle field never has zero
	 */
	engineCycle = getEngineCycle(FOUR_STROKE_CRANK_SENSOR);
	lastTriggerToothEventTimeNt = 0;
	isCylinderCleanupMode = false;
	engineCycleEventCount = 0;
	stopEngineRequestTimeNt = 0;
	isRunningPwmTest = false;
	isTestMode = false;
	isSpinning = false;
	isCltBroken = false;
	adcToVoltageInputDividerCoefficient = NAN;
	sensors.reset();
	memset(&ignitionPin, 0, sizeof(ignitionPin));

	knockNow = false;
	knockEver = false;
	knockCount = 0;
	knockDebug = false;
	knockVolts = 0;
	iHead = NULL;


	timeOfLastKnockEvent = 0;
	injectionDuration = 0;
	clutchDownState = clutchUpState = brakePedalState = false;
	memset(&m, 0, sizeof(m));

}

FuelConsumptionState::FuelConsumptionState() {
	perSecondConsumption = perSecondAccumulator = 0;
	perMinuteConsumption = perMinuteAccumulator = 0;
	accumulatedSecondPrevNt = accumulatedMinutePrevNt = getTimeNowNt();
}

void FuelConsumptionState::addData(float durationMs) {
	if (durationMs > 0.0f) {
		perSecondAccumulator += durationMs;
		perMinuteAccumulator += durationMs;
	}
}

void FuelConsumptionState::update(efitick_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	efitick_t deltaNt = nowNt - accumulatedSecondPrevNt;
	if (deltaNt >= US2NT(US_PER_SECOND_LL)) {
		perSecondConsumption = getFuelRate(perSecondAccumulator, deltaNt PASS_ENGINE_PARAMETER_SUFFIX);
		perSecondAccumulator = 0;
		accumulatedSecondPrevNt = nowNt;
	}

	deltaNt = nowNt - accumulatedMinutePrevNt;
	if (deltaNt >= US2NT(US_PER_SECOND_LL * 60)) {
		perMinuteConsumption = getFuelRate(perMinuteAccumulator, deltaNt PASS_ENGINE_PARAMETER_SUFFIX);
		perMinuteAccumulator = 0;
		accumulatedMinutePrevNt = nowNt;
	}
}

TransmissionState::TransmissionState() {

}

EngineState::EngineState() {
	dwellAngle = 0;
	engineNoiseHipLevel = 0;
	injectorLag = 0;
	warningCounter = 0;
	lastErrorCode = 0;
	crankingTime = 0;
	timeSinceCranking = 0;
	vssEventCounter = 0;
	targetAFR = 0;
	tpsAccelEnrich = 0;
	tChargeK = 0;
	cltTimingCorrection = 0;
	runningFuel = baseFuel = currentVE = 0;
	timeOfPreviousWarning = -10;
	baseTableFuel = iatFuelCorrection = 0;
	fuelPidCorrection = 0;
	cltFuelCorrection = postCrankingFuelCorrection = 0;
	warmupTargetAfr = airMass = 0;
	baroCorrection = timingAdvance = 0;
	sparkDwell = mapAveragingDuration = 0;
	totalLoggedBytes = injectionOffset = 0;
	auxValveStart = auxValveEnd = 0;
	fuelCutoffCorrection = 0;
	coastingFuelCutStartTime = 0;
}

void EngineState::updateSlowSensors(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engine->sensors.iat = getIntakeAirTemperature(PASS_ENGINE_PARAMETER_SIGNATURE);
	engine->sensors.clt = getCoolantTemperature(PASS_ENGINE_PARAMETER_SIGNATURE);
	engine->sensors.oilPressure = getOilPressure(PASS_ENGINE_PARAMETER_SIGNATURE);

	warmupTargetAfr = interpolate2d("warm", engine->sensors.clt, engineConfiguration->warmupTargetAfrBins,
			engineConfiguration->warmupTargetAfr, WARMUP_TARGET_AFR_SIZE);
}

void EngineState::periodicFastCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	efitick_t nowNt = getTimeNowNt();
	if (ENGINE(rpmCalculator).isCranking(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		crankingTime = nowNt;
		timeSinceCranking = 0.0f;
	} else {
		timeSinceCranking = nowNt - crankingTime;
	}
	updateAuxValves(PASS_ENGINE_PARAMETER_SIGNATURE);

	int rpm = ENGINE(rpmCalculator).getRpm(PASS_ENGINE_PARAMETER_SIGNATURE);
	sparkDwell = getSparkDwell(rpm PASS_ENGINE_PARAMETER_SUFFIX);
	dwellAngle = sparkDwell / getOneDegreeTimeMs(rpm);
	if (hasAfrSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		engine->sensors.currentAfr = getAfr(PASS_ENGINE_PARAMETER_SIGNATURE);
	}

	// todo: move this into slow callback, no reason for IAT corr to be here
	iatFuelCorrection = getIatFuelCorrection(engine->sensors.iat PASS_ENGINE_PARAMETER_SUFFIX);
	// todo: move this into slow callback, no reason for CLT corr to be here
	if (boardConfiguration->useWarmupPidAfr && engine->sensors.clt < engineConfiguration->warmupAfrThreshold) {
		if (rpm < 200) {
			cltFuelCorrection = 1;
			warmupAfrPid.reset();
		} else {
			cltFuelCorrection = warmupAfrPid.getValue(warmupTargetAfr, engine->sensors.currentAfr, 1);
		}
#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
		if (engineConfiguration->debugMode == DBG_WARMUP_ENRICH) {
			tsOutputChannels.debugFloatField1 = warmupTargetAfr;
			warmupAfrPid.postState(&tsOutputChannels);
		}
#endif

	} else {
		cltFuelCorrection = getCltFuelCorrection(PASS_ENGINE_PARAMETER_SIGNATURE);
	}

	// update fuel consumption states
	fuelConsumption.update(nowNt PASS_ENGINE_PARAMETER_SUFFIX);

	// Fuel cut-off isn't just 0 or 1, it can be tapered
	fuelCutoffCorrection = getFuelCutOffCorrection(nowNt, rpm PASS_ENGINE_PARAMETER_SUFFIX);
	
	// post-cranking fuel enrichment.
	// for compatibility reasons, apply only if the factor is greater than zero (0.01 margin used)
	if (engineConfiguration->postCrankingFactor > 0.01f) {
		// convert to microsecs and then to seconds
		float timeSinceCrankingInSecs = NT2US(timeSinceCranking) / 1000000.0f;
		// use interpolation for correction taper
		postCrankingFuelCorrection = interpolateClamped(0.0f, engineConfiguration->postCrankingFactor, 
			engineConfiguration->postCrankingDurationSec, 1.0f, timeSinceCrankingInSecs);
	} else {
		postCrankingFuelCorrection = 1.0f;
	}

	cltTimingCorrection = getCltTimingCorrection(PASS_ENGINE_PARAMETER_SIGNATURE);

	engineNoiseHipLevel = interpolate2d("knock", rpm, engineConfiguration->knockNoiseRpmBins,
					engineConfiguration->knockNoise, ENGINE_NOISE_CURVE_SIZE);

	baroCorrection = getBaroCorrection(PASS_ENGINE_PARAMETER_SIGNATURE);

	injectionOffset = getinjectionOffset(rpm PASS_ENGINE_PARAMETER_SUFFIX);
	float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER_SIGNATURE);
	timingAdvance = getAdvance(rpm, engineLoad PASS_ENGINE_PARAMETER_SUFFIX);

	if (engineConfiguration->fuelAlgorithm == LM_SPEED_DENSITY) {
		float coolantC = ENGINE(sensors.clt);
		float intakeC = ENGINE(sensors.iat);
		float tps = getTPS(PASS_ENGINE_PARAMETER_SIGNATURE);
		tChargeK = convertCelsiusToKelvin(getTCharge(rpm, tps, coolantC, intakeC PASS_ENGINE_PARAMETER_SUFFIX));
		float map = getMap();

		/**
		 * *0.01 because of https://sourceforge.net/p/rusefi/tickets/153/
		 */
		float rawVe = veMap.getValue(rpm, map);
		// get VE from the separate table for Idle
		if (CONFIG(useSeparateVeForIdle)) {
			float idleVe = interpolate2d("idleVe", rpm, config->idleVeBins, config->idleVe, IDLE_VE_CURVE_SIZE);
			// interpolate between idle table and normal (running) table using TPS threshold
			rawVe = interpolateClamped(0.0f, idleVe, boardConfiguration->idlePidDeactivationTpsThreshold, rawVe, tps);
		}
		currentVE = baroCorrection * rawVe * 0.01;
		targetAFR = afrMap.getValue(rpm, map);
	} else {
		baseTableFuel = getBaseTableFuel(rpm, engineLoad);
	}
}


/**
 * Here we have a bunch of stuff which should invoked after configuration change
 * so that we can prepare some helper structures
 */
void Engine::preCalculate() {
	sparkTable.preCalc(engineConfiguration->sparkDwellRpmBins,
			engineConfiguration->sparkDwellValues);

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
	engineConfiguration = &config->engineConfiguration;
	memset(config, 0, sizeof(persistent_config_s));
	engineState.warmupAfrPid.init(&config->engineConfiguration.warmupAfrPid);
}

void Engine::printKnockState(void) {
	scheduleMsg(&logger, "knock now=%s/ever=%s", boolToString(knockNow), boolToString(knockEver));
}

void Engine::knockLogic(float knockVolts) {
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
	scheduleMsg(&logger, "engine has STOPPED");
	scheduleMsg(&logger, "templog engine has STOPPED [%x][%x] [%x][%x] %d",
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

	const float vBattThreshold = 5.0f;
	if (isValidRpm(rpm) && sensors.vBatt < vBattThreshold && stopEngineRequestTimeNt == 0) {
		stopEngine();
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
	int rpm = rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE);
	ENGINE(injectionDuration) = getInjectionDuration(rpm PASS_ENGINE_PARAMETER_SUFFIX);
	engine->m.fuelCalcTime = GET_TIMESTAMP() - engine->m.beforeFuelCalc;

}

StartupFuelPumping::StartupFuelPumping() {
	isTpsAbove50 = false;
	pumpsCounter = 0;
}

void StartupFuelPumping::setPumpsCounter(int newValue) {
	if (pumpsCounter != newValue) {
		pumpsCounter = newValue;

		if (pumpsCounter == PUMPS_TO_PRIME) {
			scheduleMsg(&logger, "let's squirt prime pulse %.2f", pumpsCounter);
			pumpsCounter = 0;
		} else {
			scheduleMsg(&logger, "setPumpsCounter %d", pumpsCounter);
		}
	}
}

void StartupFuelPumping::update(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE) == 0) {
		bool isTpsAbove50 = getTPS(PASS_ENGINE_PARAMETER_SIGNATURE) >= 50;

		if (this->isTpsAbove50 != isTpsAbove50) {
			setPumpsCounter(pumpsCounter + 1);
		}

	} else {
		/**
		 * Engine is not stopped - not priming pumping mode
		 */
		setPumpsCounter(0);
		isTpsAbove50 = false;
	}
}

