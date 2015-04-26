/**
 * @file	engine.cpp
 *
 *
 * This might be a http://en.wikipedia.org/wiki/God_object but that's best way I can
 * express myself in C/C++. I am open for suggestions :)
 *
 * @date May 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"
#include "engine.h"
#include "engine_state.h"
#include "efiGpio.h"
#include "trigger_central.h"
#include "fuel_math.h"

#if EFI_PROD_CODE
#include "injector_central.h"
#else
#define isRunningBenchTest() true
#endif

static LoggingWithStorage logger("engine");

extern engine_pins_s enginePins;

EXTERN_ENGINE
;

/**
 * We are executing these heavy (logarithm) methods from outside the trigger callbacks for performance reasons.
 */
void Engine::updateSlowSensors() {
	Engine *engine = this;
	engineState.iat = getIntakeAirTemperature(PASS_ENGINE_PARAMETER_F);
	engineState.clt = getCoolantTemperature(PASS_ENGINE_PARAMETER_F);

	injectorLagMs = getInjectorLag(getVBatt(engineConfiguration) PASS_ENGINE_PARAMETER);
}

void Engine::onTriggerEvent(uint64_t nowNt) {
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
	lastTriggerEventTimeNt = 0;
	isCylinderCleanupMode = false;
	engineCycleEventCount = 0;
	stopEngineRequestTimeNt = 0;
	isRunningPwmTest = false;
	isTestMode = false;
	isSpinning = false;
	adcToVoltageInputDividerCoefficient = NAN;
	this->config = config;
	engineConfiguration = &config->engineConfiguration;
	engineConfiguration2 = NULL;
	engineState.iat = engineState.clt = NAN;
	memset(&ignitionPin, 0, sizeof(ignitionPin));
	memset(config, 0, sizeof(persistent_config_s));

	knockNow = false;
	knockEver = false;

	iat.config = NULL;
	iat.channel = EFI_ADC_NONE;

	clt.config = NULL;
	clt.channel = EFI_ADC_NONE;

	injectorLagMs = advance = dwellAngle = fuelMs = 0;
	clutchDownState = clutchUpState = false;
	memset(&m, 0, sizeof(m));

	addConfigurationListener(invokeEnginePreCalculate);
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

void Engine::init() {
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

bool_t Engine::stopPins() {
	bool result = false;
	for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
		result |= stopPin(&enginePins.coils[i]);
		result |= stopPin(&enginePins.injectors[i]);
	}
	return result;
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
	 */
	if (nowNt - lastTriggerEventTimeNt < US2NT(250000LL)) {
		return;
	}
	isSpinning = false;
#if EFI_PROD_CODE || EFI_SIMULATOR
	scheduleMsg(&logger, "engine has STOPPED");
	triggerInfo(engine);
#endif

	stopPins();
#endif
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
	if (engine->rpmCalculator.rpm(PASS_ENGINE_PARAMETER_F) == 0) {
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

