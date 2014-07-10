/**
 * @file    injector_central.cpp
 * @brief	Utility methods related to fuel injection.
 *
 *
 * @date Sep 8, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
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

#include "injector_central.h"
#include "main.h"
#include "io_pins.h"
#include "signal_executor.h"
#include "main_trigger_callback.h"
#include "engine_configuration.h"
#include "pin_repository.h"
#include "ec2.h"

static Logging logger;

extern engine_configuration_s *engineConfiguration;
extern engine_configuration2_s *engineConfiguration2;
extern board_configuration_s *boardConfiguration;

static int is_injector_enabled[MAX_INJECTOR_COUNT];

void assertCylinderId(int cylinderId, const char *msg) {
	int isValid = cylinderId >= 1 && cylinderId <= engineConfiguration->cylindersCount;
	if (!isValid) {
		// we are here only in case of a fatal issue - at this point it is fine to make some blocking i-o
		//scheduleSimpleMsg(&logger, "cid=", cylinderId);
		print("ERROR [%s] cid=%d\r\n", msg, cylinderId);
		efiAssertVoid(FALSE, "Cylinder ID");
	}
}

///**
// * This method schedules asynchronous fuel squirt
// */

/**
 * @param cylinderId - from 1 to NUMBER_OF_CYLINDERS
 */
int isInjectorEnabled(int cylinderId) {
	assertCylinderId(cylinderId, "isInjectorEnabled");
	return is_injector_enabled[cylinderId - 1];
}

static void printStatus(void) {
	for (int id = 1; id <= engineConfiguration->cylindersCount; id++) {
		resetLogging(&logger);

		appendPrintf(&logger, "injector%d%s", id, DELIMETER);
		appendPrintf(&logger, "%d", isInjectorEnabled(id));
		appendPrintf(&logger, DELIMETER);
		scheduleLogging(&logger);
	}
}

static void setInjectorEnabled(int id, int value) {
	efiAssertVoid(id >= 0 && id < engineConfiguration->cylindersCount, "injector id");
	is_injector_enabled[id] = value;
	printStatus();
}

void runBench(brain_pin_e brainPin, io_pin_e pin, float onTime, float offTime, int count) {
	scheduleMsg(&logger, "Running bench: ON_TIME=%f ms OFF_TIME=%fms Counter=%d", onTime, offTime, count);
	scheduleMsg(&logger, "output on %s", hwPortname(brainPin));

	for (int i = 0; i < count; i++) {
		setOutputPinValue(pin, TRUE);
		chThdSleep((int) (onTime * CH_FREQUENCY / 1000));
		setOutputPinValue(pin, FALSE);
		chThdSleep((int) (offTime * CH_FREQUENCY / 1000));
	}
	scheduleMsg(&logger, "Done!");
}

static volatile int needToRunBench = FALSE;
float onTime;
float offTime;
int count;
brain_pin_e brainPin;
io_pin_e pin;

static void fuelbench(char * onStr, char *offStr, char *countStr) {
	onTime = atoff(onStr);
	offTime = atoff(offStr);
	count = atoi(countStr);

	brainPin = boardConfiguration->injectionPins[0];
	pin = INJECTOR_1_OUTPUT;
	needToRunBench = TRUE;
}

static void sparkbench(char * onStr, char *offStr, char *countStr) {
	onTime = atoff(onStr);
	offTime = atoff(offStr);
	count = atoi(countStr);

	brainPin = boardConfiguration->ignitionPins[0];
	pin = SPARKOUT_1_OUTPUT;
	needToRunBench = TRUE;
}

static THD_WORKING_AREA(benchThreadStack, UTILITY_THREAD_STACK_SIZE);

static msg_t benchThread(int param) {
	chRegSetThreadName("BenchThread");

	while (TRUE) {
		while (!needToRunBench) {
			chThdSleepMilliseconds(200);
		}
		needToRunBench = FALSE;
		runBench(brainPin, pin, onTime, offTime, count);
	}
#if defined __GNUC__
	return 0;
#endif
}

void initInjectorCentral(void) {
	initLogging(&logger, "InjectorCentral");
	chThdCreateStatic(benchThreadStack, sizeof(benchThreadStack), NORMALPRIO, (tfunc_t) benchThread, NULL);

	for (int i = 0; i < engineConfiguration->cylindersCount; i++)
		is_injector_enabled[i] = true;

	// todo: should we move this code closer to the injection logic?
	for (int i = 0; i < engineConfiguration->cylindersCount; i++) {
		io_pin_e pin = (io_pin_e)((int)INJECTOR_1_OUTPUT + i);

		outputPinRegisterExt2(getPinName(pin), pin,
				boardConfiguration->injectionPins[i], &boardConfiguration->injectionPinMode);
	}

	printStatus();
	addConsoleActionII("injector", setInjectorEnabled);

	addConsoleActionSSS("fuelbench", &fuelbench);
	addConsoleActionSSS("sparkbench", &sparkbench);
}
