/**
 * @file	engine.cpp
 *
 *
 * This might be a http://en.wikipedia.org/wiki/God_object but that's best way I can
 * express myself in C/C++. I am open for suggestions :)
 *
 * @date May 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "engine.h"
#include "engine_state.h"
#include "efiGpio.h"

#if EFI_PROD_CODE || EFI_SIMULATOR
static Logging logger;
#endif

/**
 * We are executing these heavy (logarithm) methods from outside the trigger callbacks for performance reasons.
 */
void Engine::updateSlowSensors() {
	engineState.iat = getIntakeAirTemperature(engineConfiguration2);
	engineState.clt = getCoolantTemperature(engineConfiguration2);
}

void Engine::onTriggerEvent(uint64_t nowUs) {
	isSpinning = true;
	lastTriggerEventTimeUs = nowUs;
}

void Engine::init() {
#if EFI_PROD_CODE || EFI_SIMULATOR
	initLogging(&logger, "engine");
#endif
}

static bool stopPin(io_pin_e pin) {
	if (getOutputPinValue(pin)) {
		setOutputPinValue(pin, 0);
#if EFI_PROD_CODE || EFI_SIMULATOR
		scheduleMsg(&logger, "turning off %s", getPinName(pin));
#endif
		return true;
	}
	return false;
}

bool Engine::stopPins() {
	bool result = false;
	for (int i = 0; i < engineConfiguration->cylindersCount; i++) {
		io_pin_e pin = (io_pin_e) ((int) INJECTOR_1_OUTPUT + i);
		result |= stopPin(pin);
		pin = (io_pin_e) ((int) SPARKOUT_1_OUTPUT + i);
		result |= stopPin(pin);
	}
	return result;
}

void Engine::watchdog() {
	if (!isSpinning) {
		if (stopPins()) {
			firmwareError("Some pins were turned off by 2nd pass watchdog");
		}
		return;
	}
	uint64_t nowUs = getTimeNowUs();
	/**
	 * Lowest possible cranking is about 240 RPM, that's 4 revolutions per second.
	 * 0.25 second is 250000 uS
	 */
	if (nowUs - lastTriggerEventTimeUs < 250000) {
		return;
	}
	isSpinning = false;
#if EFI_PROD_CODE || EFI_SIMULATOR
	scheduleMsg(&logger, "engine has STOPPED");
#endif

	stopPins();
}

StartupFuelPumping::StartupFuelPumping() {

}

void StartupFuelPumping::setPumpsCounter(int newValue) {
	if (pumpsCounter != newValue) {
		pumpsCounter = newValue;
	}
}

void StartupFuelPumping::update(Engine *engine) {
	if (engine->rpmCalculator->rpm() == 0) {
		bool isAbove50 = getTPS(engine->engineConfiguration) >= 50;

	} else {
		/**
		 * Engine is not stopped - not priming pumping mode
		 */
		setPumpsCounter(0);
	}
}

