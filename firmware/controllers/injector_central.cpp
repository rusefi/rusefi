/**
 * @file    injector_central.cpp
 * @brief	Utility methods related to fuel injection.
 *
 *
 * @date Sep 8, 2013
 * @author Andrey Belomutskiy, (c) 2012-2016
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

// todo: rename this file
#include "main.h"

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)

#include "injector_central.h"
#include "io_pins.h"
#include "signal_executor.h"
#include "main_trigger_callback.h"
#include "engine_configuration.h"
#include "pin_repository.h"
#include "efiGpio.h"

static LoggingWithStorage logger("InjectorCentral");
EXTERN_ENGINE
;

static bool_t isRunningBench = false;

static int is_injector_enabled[MAX_INJECTOR_COUNT];

extern engine_pins_s enginePins;

bool_t isRunningBenchTest(void) {
	return isRunningBench;
}

void assertCylinderId(int cylinderId, const char *msg) {
	int isValid = cylinderId >= 1 && cylinderId <= engineConfiguration->specs.cylindersCount;
	if (!isValid) {
		// we are here only in case of a fatal issue - at this point it is fine to make some blocking i-o
		//scheduleSimpleMsg(&logger, "cid=", cylinderId);
		print("ERROR [%s] cid=%d\r\n", msg, cylinderId);
		efiAssertVoid(false, "Cylinder ID");
	}
}

/**
 * @param cylinderId - from 1 to NUMBER_OF_CYLINDERS
 */
int isInjectorEnabled(int cylinderId) {
	assertCylinderId(cylinderId, "isInjectorEnabled");
	return is_injector_enabled[cylinderId - 1];
}

static void printStatus(void) {
	for (int id = 1; id <= engineConfiguration->specs.cylindersCount; id++) {
		scheduleMsg(&logger, "injector_%d_%d", isInjectorEnabled(id));
	}
}

static void setInjectorEnabled(int id, int value) {
	efiAssertVoid(id >= 0 && id < engineConfiguration->specs.cylindersCount, "injector id");
	is_injector_enabled[id] = value;
	printStatus();
}

static void runBench(brain_pin_e brainPin, OutputPin *output, float delayMs, float onTimeMs, float offTimeMs,
		int count) {
	int delaySt = (int) (delayMs * CH_FREQUENCY / 1000);
	int onTimeSt = (int) (onTimeMs * CH_FREQUENCY / 1000);
	int offTimeSt = (int) (offTimeMs * CH_FREQUENCY / 1000);
	if (delaySt < 0) {
		scheduleMsg(&logger, "Invalid delay %f", delayMs);
		return;
	}
	if (onTimeSt <= 0) {
		scheduleMsg(&logger, "Invalid onTime %f", onTimeMs);
		return;
	}
	if (offTimeSt <= 0) {
		scheduleMsg(&logger, "Invalid offTime %f", offTimeMs);
		return;
	}
	scheduleMsg(&logger, "Running bench: ON_TIME=%f ms OFF_TIME=%fms Counter=%d", onTimeMs, offTimeMs, count);
	scheduleMsg(&logger, "output on %s", hwPortname(brainPin));

	if (delaySt != 0) {
		chThdSleep(delaySt);
	}

	isRunningBench = true;
	for (int i = 0; i < count; i++) {
		output->setValue(true);
		chThdSleep(onTimeSt);
		output->setValue(false);
		chThdSleep(offTimeSt);
	}
	scheduleMsg(&logger, "Done!");
	isRunningBench = false;
}

static volatile bool_t needToRunBench = false;
static float onTime;
static float offTime;
static float delayMs;
static int count;
static brain_pin_e brainPin;
static OutputPin* pinX;

static void pinbench(const char *delayStr, const char *onTimeStr, const char *offTimeStr, const char *countStr,
		OutputPin* pinParam, brain_pin_e brainPinParam) {
	delayMs = atoff(delayStr);
	onTime = atoff(onTimeStr);
	offTime = atoff(offTimeStr);
	count = atoi(countStr);

	brainPin = brainPinParam;
	pinX = pinParam;
	needToRunBench = true;
}

/**
 * delay 100, cylinder #2, 5ms ON, 1000ms OFF, repeat 2 times
 * fuelbench2 100 2 5 1000 2
 */
static void fuelbench2(const char *delayStr, const char *indexStr, const char * onTimeStr, const char *offTimeStr,
		const char *countStr) {
	int index = atoi(indexStr);
	if (index < 1 || index > engineConfiguration->specs.cylindersCount) {
		scheduleMsg(&logger, "Invalid index: %d", index);
		return;
	}
	brain_pin_e b = boardConfiguration->injectionPins[index - 1];
	pinbench(delayStr, onTimeStr, offTimeStr, countStr, &enginePins.injectors[index - 1], b);
}

void fanBench(void) {
	pinbench("0", "3000", "100", "1", &enginePins.fanRelay, boardConfiguration->fanPin);
}

extern OutputPin checkEnginePin;

void milBench(void) {
	pinbench("0", "3000", "100", "1", &checkEnginePin, boardConfiguration->malfunctionIndicatorPin);
}

void fuelPumpBench(void) {
	pinbench("0", "3000", "100", "1", &enginePins.fuelPumpRelay, boardConfiguration->fuelPumpPin);
}

// fuelbench 5 1000 2
static void fuelbench(const char * onTimeStr, const char *offTimeStr, const char *countStr) {
	fuelbench2("0", "1", onTimeStr, offTimeStr, countStr);
}

/**
 * sparkbench2 0 1 5 1000 2
 */
static void sparkbench2(const char *delayStr, const char *indexStr, const char * onTimeStr, const char *offTimeStr,
		const char *countStr) {
	int index = atoi(indexStr);
	if (index < 1 || index > engineConfiguration->specs.cylindersCount) {
		scheduleMsg(&logger, "Invalid index: %d", index);
		return;
	}
	brain_pin_e b = boardConfiguration->ignitionPins[index - 1];
	pinbench(delayStr, onTimeStr, offTimeStr, countStr, &enginePins.coils[index - 1], b);
}

/**
 * sparkbench 5 400 2
 * 5 ms ON, 400 ms OFF, two times
 */
static void sparkbench(const char * onTimeStr, const char *offTimeStr, const char *countStr) {
	sparkbench2("0", "1", onTimeStr, offTimeStr, countStr);
}

static THD_WORKING_AREA(benchThreadStack, UTILITY_THREAD_STACK_SIZE);

static msg_t benchThread(int param) {
	(void) param;
	chRegSetThreadName("BenchThread");

	while (true) {
		while (!needToRunBench) {
			chThdSleepMilliseconds(200);
		}
		needToRunBench = false;
		runBench(brainPin, pinX, delayMs, onTime, offTime, count);
	}
#if defined __GNUC__
	return 0;
#endif
}

extern engine_configuration_s activeConfiguration;

static void unregister(brain_pin_e currentPin, OutputPin *output) {
	if (currentPin == GPIO_UNASSIGNED)
		return;
	scheduleMsg(&logger, "unregistering %s", hwPortname(currentPin));
	unmarkPin(currentPin);
	output->unregister();
}

void stopIgnitionPins(void) {
	for (int i = 0; i < IGNITION_PIN_COUNT; i++) {
		NamedOutputPin *output = &enginePins.injectors[i];
		brain_pin_e currentPin = activeConfiguration.bc.ignitionPins[i];
		if (engineConfiguration->bc.ignitionPins[i] != currentPin) {
			unregister(currentPin, output);
		}
	}
}

void stopInjectionPins(void) {
	for (int i = 0; i < INJECTION_PIN_COUNT; i++) {
		NamedOutputPin *output = &enginePins.injectors[i];
		brain_pin_e currentPin = activeConfiguration.bc.injectionPins[i];
		if (engineConfiguration->bc.injectionPins[i] != currentPin) {
			unregister(currentPin, output);
		}
	}
}

void startIgnitionPins(void) {
	for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
		NamedOutputPin *output = &enginePins.coils[i];
		if (boardConfiguration->ignitionPins[i] != activeConfiguration.bc.ignitionPins[i]) {
			outputPinRegisterExt2(output->name, output, boardConfiguration->ignitionPins[i],
				&boardConfiguration->ignitionPinMode);
		}
	}
}

void startInjectionPins(void) {
	// todo: should we move this code closer to the injection logic?
	for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
		NamedOutputPin *output = &enginePins.injectors[i];
		if (engineConfiguration->bc.injectionPins[i] != activeConfiguration.bc.injectionPins[i]) {

			outputPinRegisterExt2(output->name, output, boardConfiguration->injectionPins[i],
					&boardConfiguration->injectionPinMode);
		}
	}
}

void initInjectorCentral(void) {
	chThdCreateStatic(benchThreadStack, sizeof(benchThreadStack), NORMALPRIO, (tfunc_t) benchThread, NULL);

	for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
		is_injector_enabled[i] = true;
	}

	startInjectionPins();
	startIgnitionPins();

	printStatus();
	addConsoleActionII("injector", setInjectorEnabled);

	addConsoleAction("fuelpumpbench", fuelPumpBench);
	addConsoleAction("fanbench", fanBench);

	addConsoleAction("milbench", milBench);
	addConsoleActionSSS("fuelbench", fuelbench);
	addConsoleActionSSS("sparkbench", sparkbench);

	addConsoleActionSSSSS("fuelbench2", fuelbench2);
	addConsoleActionSSSSS("sparkbench2", sparkbench2);
}

#endif
