/**
 * @file    injector_central.c
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
#include "engines.h"
#include "io_pins.h"
#include "signal_executor.h"
#include "main_trigger_callback.h"
#include "engine_configuration.h"
#include "pin_repository.h"

static Logging logger;

static int isInjectionEnabledFlag = TRUE;

extern engine_configuration_s *engineConfiguration;
extern engine_configuration2_s *engineConfiguration2;

static int is_injector_enabled[MAX_INJECTOR_COUNT];

int isInjectionEnabled(void) {
	return isInjectionEnabledFlag;
}

void assertCylinderId(int cylinderId, char *msg) {
	int isValid = cylinderId >= 1 && cylinderId <= engineConfiguration->cylindersCount;
	if (!isValid) {
		// we are here only in case of a fatal issue - at this point it is fine to make some blocking i-o
		//scheduleSimpleMsg(&logger, "cid=", cylinderId);
		print("ERROR [%s] cid=%d\r\n", msg, cylinderId);
		chDbgAssert(TRUE, "Cylinder ID", null);
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
	chDbgCheck(id >= 0 && id < engineConfiguration->cylindersCount, "injector id");
	is_injector_enabled[id] = value;
	printStatus();
}

static void fuelBench(char * onStr, char *offStr, char *countStr) {
	float onTime = atoff(onStr);
	float offTime = atoff(offStr);
	int count = atoi(countStr);

	print("Running fuel bench: ON_TIME=%f, OFF_TIME=%f. Counter=%d\r\n", onTime, offTime, count);
	scheduleMsg(&logger, "INJECTOR_1_OUTPUT on %s\r\n", hwPortname(engineConfiguration->injectionPins[0]));

	for (int i = 0; i < count; i++) {
		setOutputPinValue(INJECTOR_1_OUTPUT, TRUE);
		chThdSleep((int)(onTime * CH_FREQUENCY / 1000));
		setOutputPinValue(INJECTOR_1_OUTPUT, FALSE);
		chThdSleep((int)(offTime * CH_FREQUENCY / 1000));
	}
	print("Done running fuel bench\r\n");
}

void initInjectorCentral(void) {
	initLogging(&logger, "InjectorCentral");

	for (int i = 0; i < engineConfiguration->cylindersCount; i++)
		is_injector_enabled[i] = true;
	printStatus();

	// todo: should we move this code closer to the injection logic?
	outputPinRegisterExt2("injector1", INJECTOR_1_OUTPUT, engineConfiguration->injectionPins[0], &engineConfiguration->injectionPinMode);
	outputPinRegisterExt2("injector2", INJECTOR_2_OUTPUT, engineConfiguration->injectionPins[1], &engineConfiguration->injectionPinMode);
	outputPinRegisterExt2("injector3", INJECTOR_3_OUTPUT, engineConfiguration->injectionPins[2], &engineConfiguration->injectionPinMode);
	outputPinRegisterExt2("injector4", INJECTOR_4_OUTPUT, engineConfiguration->injectionPins[3], &engineConfiguration->injectionPinMode);
	outputPinRegisterExt2("injector5", INJECTOR_5_OUTPUT, engineConfiguration->injectionPins[4], &engineConfiguration->injectionPinMode);

	addConsoleActionII("injector", setInjectorEnabled);

	addConsoleActionSSS("fuelbench", &fuelBench);
}
