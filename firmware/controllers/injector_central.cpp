/**
 * @file    injector_central.cpp
 * @brief	Utility methods related to fuel injection.
 *
 *
 * @date Sep 8, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
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
#include "settings.h"

EXTERN_ENGINE
;

static Logging * logger;
static bool isRunningBench = false;

// todo: move into Engine object?
// todo: looks like these flags are not currently used? dead functionality? unfinished functionality?
static int is_injector_enabled[INJECTION_PIN_COUNT];

bool isRunningBenchTest(void) {
	return isRunningBench;
}

static void assertCylinderId(int cylinderId, const char *msg) {
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
static int isInjectorEnabled(int cylinderId) {
	assertCylinderId(cylinderId, "isInjectorEnabled");
	return is_injector_enabled[cylinderId - 1];
}

static void printInjectorsStatus(void) {
	for (int id = 1; id <= engineConfiguration->specs.cylindersCount; id++) {
		scheduleMsg(logger, "injector_%d: %d", id, isInjectorEnabled(id));
	}
}

static void setInjectorEnabled(int id, int value) {
	efiAssertVoid(id >= 0 && id < engineConfiguration->specs.cylindersCount, "injector id");
	is_injector_enabled[id] = value;
	printInjectorsStatus();
}

static void runBench(brain_pin_e brainPin, OutputPin *output, float delayMs, float onTimeMs, float offTimeMs,
		int count) {
    int delaySt = delayMs < 1 ? 1 : MS2ST(delayMs);
	int onTimeSt = onTimeMs < 1 ? 1 : MS2ST(onTimeMs);
	int offTimeSt = offTimeMs < 1 ? 1 : MS2ST(offTimeMs);
	if (delaySt < 0) {
		scheduleMsg(logger, "Invalid delay %.2f", delayMs);
		return;
	}
	if (onTimeSt <= 0) {
		scheduleMsg(logger, "Invalid onTime %.2f", onTimeMs);
		return;
	}
	if (offTimeSt <= 0) {
		scheduleMsg(logger, "Invalid offTime %.2f", offTimeMs);
		return;
	}
	scheduleMsg(logger, "Running bench: ON_TIME=%.2f ms OFF_TIME=%.2fms Counter=%d", onTimeMs, offTimeMs, count);
	scheduleMsg(logger, "output on %s", hwPortname(brainPin));

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
	scheduleMsg(logger, "Done!");
	isRunningBench = false;
}

static volatile bool isBenchTestPending = false;
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
	isBenchTestPending = true; // let's signal bench thread to wake up
}

static void doRunFuel(int humanIndex, const char *delayStr, const char * onTimeStr, const char *offTimeStr,
		const char *countStr) {
	if (humanIndex < 1 || humanIndex > engineConfiguration->specs.cylindersCount) {
		scheduleMsg(logger, "Invalid index: %d", humanIndex);
		return;
	}
	brain_pin_e b = boardConfiguration->injectionPins[humanIndex - 1];
	pinbench(delayStr, onTimeStr, offTimeStr, countStr, &enginePins.injectors[humanIndex - 1], b);
}

/**
 * delay 100, cylinder #2, 5ms ON, 1000ms OFF, repeat 2 times
 * fuelbench2 100 2 5 1000 2
 */
static void fuelbench2(const char *delayStr, const char *indexStr, const char * onTimeStr, const char *offTimeStr,
		const char *countStr) {
	int index = atoi(indexStr);
	doRunFuel(index, delayStr, onTimeStr, offTimeStr, countStr);
}

static void fanBenchExt(const char *durationMs) {
	pinbench("0", durationMs, "100", "1", &enginePins.fanRelay, boardConfiguration->fanPin);
}

void fanBench(void) {
	fanBenchExt("3000");
}

void milBench(void) {
	pinbench("0", "500", "500", "16", &enginePins.checkEnginePin, boardConfiguration->malfunctionIndicatorPin);
}

void fuelPumpBenchExt(const char *durationMs) {
	pinbench("0", durationMs, "100", "1", &enginePins.fuelPumpRelay, boardConfiguration->fuelPumpPin);
}

void fuelPumpBench(void) {
	fuelPumpBenchExt("3000");
}

// fuelbench 5 1000 2
static void fuelbench(const char * onTimeStr, const char *offTimeStr, const char *countStr) {
	fuelbench2("0", "1", onTimeStr, offTimeStr, countStr);
}

static void doRunSpark(int humanIndex, const char *delayStr, const char * onTimeStr, const char *offTimeStr,
		const char *countStr) {
	if (humanIndex < 1 || humanIndex > engineConfiguration->specs.cylindersCount) {
		scheduleMsg(logger, "Invalid index: %d", humanIndex);
		return;
	}
	brain_pin_e b = boardConfiguration->ignitionPins[humanIndex - 1];
	pinbench(delayStr, onTimeStr, offTimeStr, countStr, &enginePins.coils[humanIndex - 1], b);
}

/**
 * sparkbench2 0 1 5 1000 2
 */
static void sparkbench2(const char *delayStr, const char *indexStr, const char * onTimeStr, const char *offTimeStr,
		const char *countStr) {
	int index = atoi(indexStr);
	doRunSpark(index, delayStr, onTimeStr, offTimeStr, countStr);
}

/**
 * sparkbench 5 400 2
 * 5 ms ON, 400 ms OFF, two times
 */
static void sparkbench(const char * onTimeStr, const char *offTimeStr, const char *countStr) {
	sparkbench2("0", "1", onTimeStr, offTimeStr, countStr);
}


void dizzyBench(void) {
	pinbench("300", "5", "400", "3", &enginePins.dizzyOutput, engineConfiguration->dizzySparkOutputPin);
}


static THD_WORKING_AREA(benchThreadStack, UTILITY_THREAD_STACK_SIZE);

static msg_t benchThread(int param) {
	(void) param;
	chRegSetThreadName("BenchThread");

	while (true) {
		// naive inter-thread communication - waiting for a flag
		while (!isBenchTestPending) {
			chThdSleepMilliseconds(200);
		}
		isBenchTestPending = false;
		runBench(brainPin, pinX, delayMs, onTime, offTime, count);
	}
#if defined __GNUC__
	return 0;
#endif
}

void OutputPin::unregisterOutput(brain_pin_e oldPin, brain_pin_e newPin) {
	if (oldPin != GPIO_UNASSIGNED && oldPin != newPin) {
		scheduleMsg(logger, "unregistering %s", hwPortname(oldPin));
		unmarkPin(oldPin);
#if EFI_GPIO_HARDWARE || defined(__DOXYGEN__)
		port = NULL;
#endif /* EFI_PROD_CODE */
	}
}

void runIoTest(int subsystem, int index) {
	scheduleMsg(logger, "IO test subsystem=%d index=%d", subsystem, index);

	if (subsystem == 0x12) {
		doRunSpark(index, "300", "4", "400", "3");
	} else if (subsystem == 0x13) {
		doRunFuel(index, "300", "4", "400", "3");
	} else if (subsystem == 0x14) {
		fuelPumpBench();
	} else if (subsystem == 0x15) {
		fanBench();
	} else if (subsystem == 0x16) {
		milBench();
	} else if (subsystem == 0x17) {
	} else if (subsystem == 0x20 && index == 0x3456) {
		// call to pit
		setCallFromPitStop(30000);
	} else if (subsystem == 0x99) {
		stopEngine();
	}
}

void initInjectorCentral(Logging *sharedLogger) {
	logger = sharedLogger;
	chThdCreateStatic(benchThreadStack, sizeof(benchThreadStack), NORMALPRIO, (tfunc_t) benchThread, NULL);

	for (int i = 0; i < INJECTION_PIN_COUNT; i++) {
		is_injector_enabled[i] = true;
	}

	enginePins.startInjectionPins();
	enginePins.startIgnitionPins();
	enginePins.startAuxValves();

	printInjectorsStatus();
	addConsoleActionII("injector", setInjectorEnabled);

	addConsoleAction("fuelpumpbench", fuelPumpBench);
	addConsoleActionS("fuelpumpbench2", fuelPumpBenchExt);
	addConsoleAction("fanbench", fanBench);
	addConsoleActionS("fanbench2", fanBenchExt);
	addConsoleAction("dizzybench", dizzyBench); // this is useful for tach output testing

	addConsoleAction("milbench", milBench);
	addConsoleActionSSS("fuelbench", fuelbench);
	addConsoleActionSSS("sparkbench", sparkbench);

	addConsoleActionSSSSS("fuelbench2", fuelbench2);
	addConsoleActionSSSSS("sparkbench2", sparkbench2);
}

#endif
