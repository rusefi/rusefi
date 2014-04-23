/**
 * @file    idle_thread.c
 * @brief   Idle Air Control valve thread.
 *
 * This thread looks at current RPM and decides if it should increase or decrease IAC duty cycle.
 * This file is has the hardware & scheduling logic, desired idle level lives separately
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
#include "engine_configuration.h"

#define IDLE_AIR_CONTROL_VALVE_PWM_FREQUENCY 200

static WORKING_AREA(ivThreadStack, UTILITY_THREAD_STACK_SIZE);

static volatile int isIdleControlActive = EFI_IDLE_CONTROL;
extern board_configuration_s *boardConfiguration;

/**
 * here we keep the value we got from IDLE SWITCH input
 */
static volatile int idleSwitchState;

static Logging logger;

static PwmConfig idleValve;

/**
 * Idle level calculation algorithm lives in idle_controller.c
 */
static IdleValveState idle;

int getIdleSwitch() {
	return idleSwitchState;
}

void idleDebug(char *msg, int value) {
	printMsg(&logger, "%s%d", msg, value);
	scheduleLogging(&logger);
}

static void setIdleControlEnabled(int value) {
	isIdleControlActive = value;
	scheduleMsg(&logger, "isIdleControlActive=%d", isIdleControlActive);
}

static void setIdleValvePwm(int value) {
	// todo: change parameter type, maybe change parameter validation?
	if (value < 1 || value > 999)
		return;
	scheduleMsg(&logger, "setting idle valve PWM %d", value);
	/**
	 * currently IDEL level is an integer per mil (0-1000 range), and PWM takes a fioat in the 0..1 range
	 * todo: unify?
	 */
	setSimplePwmDutyCycle(&idleValve, 0.001 * value);
}

static msg_t ivThread(int param) {
	chRegSetThreadName("IdleValve");

	int currentIdleValve = -1;
	while (TRUE) {
		chThdSleepMilliseconds(100);

		// this value is not used yet
		idleSwitchState = palReadPad(getHwPort(boardConfiguration->idleSwitchPin), getHwPin(boardConfiguration->idleSwitchPin));

		if (!isIdleControlActive)
			continue;

		int nowSec = chTimeNowSeconds();

		int newValue = getIdle(&idle, getRpm(), nowSec);

		if (currentIdleValve != newValue) {
			currentIdleValve = newValue;

			setIdleValvePwm(newValue);
		}
	}
#if defined __GNUC__
	return -1;
#endif
}

static void setIdleRpmAction(int value) {
	setIdleRpm(&idle, value);
	scheduleMsg(&logger, "target idle RPM %d", value);
}

void startIdleThread() {
	initLogging(&logger, "Idle Valve Control");

	startSimplePwm(&idleValve, "Idle Valve",
			boardConfiguration->idleValvePin,
			0.5,
			IDLE_AIR_CONTROL_VALVE_PWM_FREQUENCY,
			IDLE_VALVE
			);

	idleInit(&idle);
	scheduleMsg(&logger, "initial idle %d", idle.value);

	chThdCreateStatic(ivThreadStack, sizeof(ivThreadStack), NORMALPRIO, (tfunc_t)ivThread, NULL);

	// this is idle switch INPUT - sometimes there is a switch on the throttle pedal
	// this switch is not used yet
	mySetPadMode("idle switch", getHwPort(boardConfiguration->idleSwitchPin), getHwPin(boardConfiguration->idleSwitchPin), PAL_MODE_INPUT);

	addConsoleActionI("set_idle_rpm", setIdleRpmAction);
	addConsoleActionI("set_idle_pwm", setIdleValvePwm);
	addConsoleActionI("set_idle_enabled", setIdleControlEnabled);
}
