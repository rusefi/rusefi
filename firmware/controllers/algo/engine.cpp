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

extern engine_pins_s enginePins;
extern fuel_Map3D_t veMap;
extern fuel_Map3D_t afrMap;


EXTERN_ENGINE
;

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
	engineState.iat = getIntakeAirTemperature(PASS_ENGINE_PARAMETER_F);
	engineState.clt = getCoolantTemperature(PASS_ENGINE_PARAMETER_F);

	if (engineConfiguration->fuelLevelSensor != EFI_ADC_NONE) {
		float fuelLevelVoltage = getVoltageDivided("fuel", engineConfiguration->fuelLevelSensor);
		engineState.fuelTankGauge = interpolate(boardConfiguration->fuelLevelEmptyTankVoltage, 0,
				boardConfiguration->fuelLevelFullTankVoltage, 100,
				fuelLevelVoltage);
	}
	float vBatt = hasVBatt(PASS_ENGINE_PARAMETER_F) ? getVBatt(PASS_ENGINE_PARAMETER_F) : 12;

	injectorLagMs = getInjectorLag(vBatt PASS_ENGINE_PARAMETER);
}

void Engine::onTriggerEvent(efitick_t nowNt) {
	isSpinning = true;
	lastTriggerEventTimeNt = nowNt;
}

static void invokeEnginePreCalculate(Engine *engine) {
	engine->preCalculate();
}

void Engine::addConfigurationListener(configuration_callback_t callback) {
	configurationListeners.registerCallback((VoidInt)invokeEnginePreCalculate, this);
}

Engine::Engine(persistent_config_s *config) {
	init(config);
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

	timeOfLastKnockEvent = 0;
	injectorLagMs = fuelMs = 0;
	clutchDownState = clutchUpState = false;
	memset(&m, 0, sizeof(m));

	addConfigurationListener(invokeEnginePreCalculate);
}

EngineState::EngineState() {
	dwellAngle = 0;
	engineNoiseHipLevel = 0;
}

void EngineState::periodicFastCallback(DECLARE_ENGINE_PARAMETER_F) {
	int rpm = ENGINE(rpmCalculator.rpmValue);

	sparkDwell = getSparkDwell(rpm PASS_ENGINE_PARAMETER);
	dwellAngle = sparkDwell / getOneDegreeTimeMs(rpm);

	iatFuelCorrection = getIatCorrection(iat PASS_ENGINE_PARAMETER);
	cltFuelCorrection = getCltCorrection(clt PASS_ENGINE_PARAMETER);

	engineNoiseHipLevel = interpolate2d(rpm, engineConfiguration->knockNoiseRpmBins,
					engineConfiguration->knockNoise, ENGINE_NOISE_CURVE_SIZE);

	baroCorrection = getBaroCorrection(PASS_ENGINE_PARAMETER_F);

	injectionOffset = getinjectionOffset(rpm PASS_ENGINE_PARAMETER);
	float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER_F);
	timingAdvance = getAdvance(rpm, engineLoad PASS_ENGINE_PARAMETER);

	if (engineConfiguration->algorithm == LM_SPEED_DENSITY) {
		float coolantC = ENGINE(engineState.clt);
		float intakeC = ENGINE(engineState.iat);
		float tps = getTPS(PASS_ENGINE_PARAMETER_F);
		tChargeK = convertCelsiusToKelvin(getTCharge(rpm, tps, coolantC, intakeC));
		float map = getMap();

		/**
		 * *0.01 because of https://sourceforge.net/p/rusefi/tickets/153/
		 */
		currentVE = baroCorrection * veMap.getValue(map, rpm) * 0.01;
		targerAFR = afrMap.getValue(map, rpm);
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

static bool stopPin(NamedOutputPin *output) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	if (output->isInitialized() && output->getLogicValue()) {
		output->setValue(false);
		scheduleMsg(&logger, "turning off %s", output->name);
		return true;
	}
#endif
	return false;
}

bool Engine::stopPins() {
	bool result = false;
	for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
		result |= stopPin(&enginePins.coils[i]);
		result |= stopPin(&enginePins.injectors[i]);
	}
	return result;
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
		if (!isRunningBenchTest() && stopPins()) {
			firmwareError("Some pins were turned off by 2nd pass watchdog");
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
#if EFI_PROD_CODE || EFI_SIMULATOR
	scheduleMsg(&logger, "engine has STOPPED");
	scheduleMsg(&logger, "templog engine has STOPPED [%x][%x] [%x][%x] %d",
			(int)(nowNt >> 32), (int)nowNt,
			(int)(lastTriggerEventTimeNt >> 32), (int)lastTriggerEventTimeNt,
			(int)timeSinceLastTriggerEvent
			);
	triggerInfo();
#endif

	stopPins();
#endif
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

	ENGINE(m.beforeInjectonSch) = GET_TIMESTAMP();

	injection_mode_e mode = isCrankingR(rpm) ? CONFIG(crankingInjectionMode) : CONFIG(injectionMode);

	ENGINE(engineConfiguration2)->processing->addFuelEvents(
			mode PASS_ENGINE_PARAMETER);
	ENGINE(m.injectonSchTime) = GET_TIMESTAMP() - ENGINE(m.beforeInjectonSch);

	/**
	 * Swap pointers. This way we are always reading from one instance while adjusting scheduling of another instance.
	 */
	FuelSchedule * t = ENGINE(engineConfiguration2)->injectionEvents;
	ENGINE(engineConfiguration2)->injectionEvents = ENGINE(engineConfiguration2)->processing;
	ENGINE(engineConfiguration2)->processing = t;
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

