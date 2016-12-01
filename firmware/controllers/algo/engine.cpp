/**
 * @file	engine.cpp
 *
 *
 * This might be a http://en.wikipedia.org/wiki/God_object but that's best way I can
 * express myself in C/C++. I am open for suggestions :)
 *
 * @date May 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2016
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
	scheduleMsg(&logger, "fake voltage: channel %d value %f", hwChannel, voltage);

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
void Engine::updateSlowSensors(DECLARE_ENGINE_PARAMETER_F) {
	int rpm = rpmCalculator.rpmValue;
	isEngineChartEnabled = CONFIG(isEngineChartEnabled) && rpm < CONFIG(engineSnifferRpmThreshold);
	sensorChartMode = rpm < CONFIG(sensorSnifferRpmThreshold) ? boardConfiguration->sensorChartMode : SC_OFF;

	engineState.updateSlowSensors(PASS_ENGINE_PARAMETER_F);

	if (engineConfiguration->fuelLevelSensor != EFI_ADC_NONE) {
		float fuelLevelVoltage = getVoltageDivided("fuel", engineConfiguration->fuelLevelSensor);
		engineState.fuelTankGauge = interpolate(boardConfiguration->fuelLevelEmptyTankVoltage, 0,
				boardConfiguration->fuelLevelFullTankVoltage, 100,
				fuelLevelVoltage);
	}
	float vBatt = hasVBatt(PASS_ENGINE_PARAMETER_F) ? getVBatt(PASS_ENGINE_PARAMETER_F) : 12;

	engineState.injectorLag = getInjectorLag(vBatt PASS_ENGINE_PARAMETER);
}

void Engine::onTriggerEvent(efitick_t nowNt) {
	isSpinning = true;
	lastTriggerEventTimeNt = nowNt;
}

Engine::Engine(persistent_config_s *config) {
	init(config);
	engineState.warmupAfrPid.init(&config->engineConfiguration.warmupAfrPid,  0.5, 1.5);
	isEngineChartEnabled = false;
	sensorChartMode = SC_OFF;
	actualLastInjection = 0;
	fuelScheduleForThisEngineCycle = NULL;
	isAlternatorControlEnabled = false;
	wallFuelCorrection = 0;
	/**
	 * it's important for fixAngle() that engineCycle field never has zero
	 */
	engineCycle = getEngineCycle(FOUR_STROKE_CRANK_SENSOR);
	lastTriggerEventTimeNt = 0;
	isCylinderCleanupMode = false;
	engineCycleEventCount = 0;
	stopEngineRequestTimeNt = 0;
	isRunningPwmTest = false;
	isTestMode = false;
	isSpinning = false;
	adcToVoltageInputDividerCoefficient = NAN;
	engineConfiguration2 = NULL;
	engineState.iat = engineState.clt = NAN;
	memset(&ignitionPin, 0, sizeof(ignitionPin));

	knockNow = false;
	knockEver = false;
	knockCount = 0;
	knockDebug = false;
	knockVolts = 0;
	iHead = NULL;


	timeOfLastKnockEvent = 0;
	fuelMs = 0;
	clutchDownState = clutchUpState = false;
	memset(&m, 0, sizeof(m));

}

EngineState::EngineState() {
	dwellAngle = 0;
	engineNoiseHipLevel = 0;
	injectorLag = 0;
	warningCounter = 0;
	lastErrorCode = 0;
	targetAFR = 0;
	tpsAccelEnrich = 0;
	tChargeK = 0;
	runningFuel = baseFuel = currentVE = 0;
	timeOfPreviousWarning = -10;
	baseTableFuel = iat = iatFuelCorrection = 0;
	clt = cltFuelCorrection = 0;
	warmupTargetAfr = airMass = 0;
	baroCorrection = timingAdvance = fuelTankGauge = 0;
	sparkDwell = mapAveragingDuration = 0;
	injectionOffset = 0;
}

void EngineState::updateSlowSensors(DECLARE_ENGINE_PARAMETER_F) {
	iat = getIntakeAirTemperature(PASS_ENGINE_PARAMETER_F);
	clt = getCoolantTemperature(PASS_ENGINE_PARAMETER_F);

	warmupTargetAfr = interpolate2d(clt, engineConfiguration->warmupTargetAfrBins,
			engineConfiguration->warmupTargetAfr, WARMUP_TARGET_AFR_SIZE);
}

void EngineState::periodicFastCallback(DECLARE_ENGINE_PARAMETER_F) {
	int rpm = ENGINE(rpmCalculator.rpmValue);

	sparkDwell = getSparkDwell(rpm PASS_ENGINE_PARAMETER);
	dwellAngle = sparkDwell / getOneDegreeTimeMs(rpm);

	// todo: move this into slow callback, no reason for IAT corr to be here
	iatFuelCorrection = getIatCorrection(iat PASS_ENGINE_PARAMETER);
	// todo: move this into slow callback, no reason for CLT corr to be here
	if (boardConfiguration->useWarmupPidAfr && clt < engineConfiguration->warmupAfrThreshold) {
		if (rpm < 200) {
			cltFuelCorrection = 1;
			warmupAfrPid.reset();
		} else {
			cltFuelCorrection = warmupAfrPid.getValue(warmupTargetAfr, getAfr(PASS_ENGINE_PARAMETER_F), 1);
		}
#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
		if (engineConfiguration->debugMode == WARMUP_ENRICH) {
			tsOutputChannels.debugFloatField1 = warmupTargetAfr;
			warmupAfrPid.postState(&tsOutputChannels);
		}
#endif

	} else {
		cltFuelCorrection = getCltCorrection(clt PASS_ENGINE_PARAMETER);
	}

	engineNoiseHipLevel = interpolate2d(rpm, engineConfiguration->knockNoiseRpmBins,
					engineConfiguration->knockNoise, ENGINE_NOISE_CURVE_SIZE);

	baroCorrection = getBaroCorrection(PASS_ENGINE_PARAMETER_F);

	injectionOffset = getinjectionOffset(rpm PASS_ENGINE_PARAMETER);
	float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER_F);
	timingAdvance = getAdvance(rpm, engineLoad PASS_ENGINE_PARAMETER);

	if (engineConfiguration->fuelAlgorithm == LM_SPEED_DENSITY) {
		float coolantC = ENGINE(engineState.clt);
		float intakeC = ENGINE(engineState.iat);
		float tps = getTPS(PASS_ENGINE_PARAMETER_F);
		tChargeK = convertCelsiusToKelvin(getTCharge(rpm, tps, coolantC, intakeC PASS_ENGINE_PARAMETER));
		float map = getMap();

		/**
		 * *0.01 because of https://sourceforge.net/p/rusefi/tickets/153/
		 */
		currentVE = baroCorrection * veMap.getValue(rpm, map) * 0.01;
		targetAFR = afrMap.getValue(rpm, map);
	} else {
		baseTableFuel = getBaseTableFuel(engineConfiguration, rpm, engineLoad);
	}

}


/**
 * Here we have a bunch of stuff which should invoked after configuration change
 * so that we can prepare some helper structures
 */
void Engine::preCalculate() {
	sparkTable.preCalc(engineConfiguration->sparkDwellBins,
			engineConfiguration->sparkDwell);

	/**
	 * Here we prepare a fast, index-based MAF lookup from a slower curve description
	 */
	for (int i = 0; i < MAF_DECODING_CACHE_SIZE; i++) {
		float volts = i / MAF_DECODING_CACHE_MULT;
		float maf = interpolate2d(volts, config->mafDecodingBins,
				config->mafDecoding, MAF_DECODING_COUNT);
		mafDecodingLookup[i] = maf;
	}
}

void Engine::init(persistent_config_s *config) {
	this->config = config;
	engineConfiguration = &config->engineConfiguration;
	memset(config, 0, sizeof(persistent_config_s));
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
	/**
	 * Lowest possible cranking is about 240 RPM, that's 4 revolutions per second.
	 * 0.25 second is 250000 uS
	 *
	 * todo: better watch dog implementation should be implemented - see
	 * http://sourceforge.net/p/rusefi/tickets/96/
	 *
	 * note that the result of this subtraction could be negative, that would happen if
	 * we have a trigger event between the time we've invoked 'getTimeNow' and here
	 */
	efitick_t timeSinceLastTriggerEvent = nowNt - lastTriggerEventTimeNt;
	if (timeSinceLastTriggerEvent < US2NT(250000LL)) {
		return;
	}
	isSpinning = false;
	ignitionList()->isReady = false;
#if EFI_PROD_CODE || EFI_SIMULATOR
	scheduleMsg(&logger, "engine has STOPPED");
	scheduleMsg(&logger, "templog engine has STOPPED [%x][%x] [%x][%x] %d",
			(int)(nowNt >> 32), (int)nowNt,
			(int)(lastTriggerEventTimeNt >> 32), (int)lastTriggerEventTimeNt,
			(int)timeSinceLastTriggerEvent
			);
	triggerInfo();
#endif

	enginePins.stopPins();
#endif
}

IgnitionEventList * Engine::ignitionList() {
	return &engineConfiguration2->ignitionEvents;
}

injection_mode_e Engine::getCurrentInjectionMode(DECLARE_ENGINE_PARAMETER_F) {
	int rpm = rpmCalculator.rpmValue;
	return isCrankingR(rpm) ? CONFIG(crankingInjectionMode) : CONFIG(injectionMode);
}

void Engine::prepareFuelSchedule(DECLARE_ENGINE_PARAMETER_F) {
	efiAssertVoid(ENGINE(engineConfiguration2)->injectionEvents != ENGINE(engineConfiguration2)->processing, "fuel pointers");

	ENGINE(m.beforeInjectonSch) = GET_TIMESTAMP();

	if (ENGINE(engineConfiguration2)->processing->usedAtEngineCycle != 0 &&
			ENGINE(engineConfiguration2)->processing->usedAtEngineCycle == ENGINE(rpmCalculator).getRevolutionCounter()) {
		// we are here if engine is still using this older fuel schedule, not yet time to override it
//		scheduleMsg(&logger, "still need %d", ENGINE(rpmCalculator).getRevolutionCounter());
		return;
	}

	ENGINE(engineConfiguration2)->processing->addFuelEvents(PASS_ENGINE_PARAMETER_F);
	ENGINE(m.injectonSchTime) = GET_TIMESTAMP() - ENGINE(m.beforeInjectonSch);

	/**
	 * Swap pointers. This way we are always reading from one instance while adjusting scheduling of another instance.
	 */
	FuelSchedule * t = ENGINE(engineConfiguration2)->injectionEvents;
	ENGINE(engineConfiguration2)->injectionEvents = ENGINE(engineConfiguration2)->processing;
	ENGINE(engineConfiguration2)->processing = t;
}

/**
 * The idea of this method is to execute all heavy calculations in a lower-priority thread,
 * so that trigger event handler/IO scheduler tasks are faster.
 */
void Engine::periodicFastCallback(DECLARE_ENGINE_PARAMETER_F) {
	int rpm = rpmCalculator.rpmValue;

	if (isValidRpm(rpm)) {
		MAP_sensor_config_s * c = &engineConfiguration->map;
		angle_t start = interpolate2d(rpm, c->samplingAngleBins, c->samplingAngle, MAP_ANGLE_SIZE);

		angle_t offsetAngle = TRIGGER_SHAPE(eventAngles[CONFIG(mapAveragingSchedulingAtIndex)]);

		for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
			angle_t cylinderOffset = getEngineCycle(engineConfiguration->operationMode) * i / engineConfiguration->specs.cylindersCount;
			float cylinderStart = start + cylinderOffset - offsetAngle + tdcPosition();
			fixAngle(cylinderStart);
			engine->engineState.mapAveragingStart[i] = cylinderStart;
		}
		engine->engineState.mapAveragingDuration = interpolate2d(rpm, c->samplingWindowBins, c->samplingWindow, MAP_WINDOW_SIZE);
	} else {
		for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
			engine->engineState.mapAveragingStart[i] = NAN;
		}
		engine->engineState.mapAveragingDuration = NAN;
	}

	engineState.periodicFastCallback(PASS_ENGINE_PARAMETER_F);

	engine->m.beforeFuelCalc = GET_TIMESTAMP();
	ENGINE(fuelMs) = getInjectionDuration(rpm PASS_ENGINE_PARAMETER) * engineConfiguration->globalFuelCorrection;
	engine->m.fuelCalcTime = GET_TIMESTAMP() - engine->m.beforeFuelCalc;

	prepareFuelSchedule(PASS_ENGINE_PARAMETER_F);
}

StartupFuelPumping::StartupFuelPumping() {
	isTpsAbove50 = false;
	pumpsCounter = 0;
}

void StartupFuelPumping::setPumpsCounter(
		engine_configuration_s *engineConfiguration, int newValue) {
	if (pumpsCounter != newValue) {
		pumpsCounter = newValue;

		if (pumpsCounter == PUMPS_TO_PRIME) {
			scheduleMsg(&logger, "let's squirt prime pulse %f", pumpsCounter);
			pumpsCounter = 0;
		} else {
			scheduleMsg(&logger, "setPumpsCounter %d", pumpsCounter);
		}
	}
}

void StartupFuelPumping::update(DECLARE_ENGINE_PARAMETER_F) {
	if (engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_F) == 0) {
		bool isTpsAbove50 = getTPS(PASS_ENGINE_PARAMETER_F) >= 50;

		if (this->isTpsAbove50 != isTpsAbove50) {
			setPumpsCounter(engineConfiguration, pumpsCounter + 1);
		}

	} else {
		/**
		 * Engine is not stopped - not priming pumping mode
		 */
		setPumpsCounter(engineConfiguration, 0);
		isTpsAbove50 = false;
	}
}

