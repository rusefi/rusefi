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

#if EFI_PROD_CODE || EFI_SIMULATOR
static Logging logger;
#endif

/**
 * We are executing these heavy (logarithm) methods from outside the trigger callbacks for performance reasons.
 */
void Engine::updateSlowSensors() {
	engineState.iat = getIntakeAirTemperature();
	engineState.clt = getCoolantTemperature();
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
void Engine::watchdog() {
	if (!isSpinning) {
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
	isSpinning = true;
	scheduleMsg(&logger, "engine has STOPPED");


	for (int i = 0; i < engineConfiguration->cylindersCount; i++) {
//		io_pin_e pin = (io_pin_e) ((int) INJECTOR_1_OUTPUT + i);

		//pin = (io_pin_e) ((int) SPARKOUT_1_OUTPUT + i);
	}

}
