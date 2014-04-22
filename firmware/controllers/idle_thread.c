/**
 * @file    idle_thread.c
 * @brief   Idle Air Control valve thread.
 *
 * 			This thread looks at current RPM and decides if it should increase or decrease IAC duty cycle
 *
 *
 * @date May 23, 2013
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

#include "main.h"
#include "idle_controller.h"
#include "rpm_calculator.h"
#include "pwm_generator.h"
#include "wave_math.h"
#include "idle_thread.h"
#include "pin_repository.h"

#define IDLE_AIR_CONTROL_VALVE_PWM_FREQUENCY 200

static WORKING_AREA(ivThreadStack, UTILITY_THREAD_STACK_SIZE);

static int isIdleActive = EFI_IDLE_CONTROL;

/**
 * here we keep the value we got from IDLE SWITCH input
 */
static volatile int idleSwitchState;

static PwmConfig idleValve;

static IdleValveState idle;
static Logging logger;

int getIdleSwitch() {
	return idleSwitchState;
}

void idleDebug(char *msg, int value) {
	printMsg(&logger, "%s%d", msg, value);
	scheduleLogging(&logger);
}

static void setIdle(int value) {
	// todoL change parameter type, maybe change parameter validation
	if (value < 1 || value > 999)
		return;
	scheduleMsg(&logger, "setting idle valve PWM %d", value);
	float v = 0.001 * value;
	idleValve.multiWave.switchTimes[0] = 1 - v;
}

static msg_t ivThread(int param) {
	chRegSetThreadName("IdleValve");

	int currentIdleValve = -1;
	while (TRUE) {
		chThdSleepMilliseconds(100);

		idleSwitchState = palReadPad(IDLE_SWITCH_PORT, IDLE_SWITCH_PIN);

		if (!isIdleActive)
			continue;

		int nowSec = chTimeNowSeconds();

		int newValue = getIdle(&idle, getRpm(), nowSec);

		// todo: invert wave & eliminate this inversion?
		newValue = 1000 - newValue; // convert algorithm value into actual PMW value

		if (currentIdleValve != newValue) {
			currentIdleValve = newValue;

			setIdle(newValue);
		}
	}
#if defined __GNUC__
	return -1;
#endif
}

static void setTargetIdle(int value) {
	setTargetRpm(&idle, value);
	scheduleMsg(&logger, "target idle RPM %d", value);
}

void startIdleThread() {
	initLogging(&logger, "Idle Valve Control");

	wePlainInit("Idle Valve", &idleValve, IDLE_VALVE_PORT, IDLE_VALVE_PIN, 0.5, IDLE_AIR_CONTROL_VALVE_PWM_FREQUENCY,
			IDLE_VALVE
			);

	idleInit(&idle);
	scheduleMsg(&logger, "initial idle %d", idle.value);
	if (!isIdleActive)
		printMsg(&logger,
				"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! idle control disabled");

	addConsoleActionI("target", setTargetIdle);

	chThdCreateStatic(ivThreadStack, sizeof(ivThreadStack), NORMALPRIO, (tfunc_t)ivThread, NULL);

	mySetPadMode("idle switch", IDLE_SWITCH_PORT, IDLE_SWITCH_PIN, PAL_MODE_INPUT);

	addConsoleActionI("idle", setIdle);
}
