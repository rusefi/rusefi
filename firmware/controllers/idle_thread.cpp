/**
 * @file    idle_thread.cpp
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
#include "idle_thread.h"
#include "pin_repository.h"
#include "engine_configuration.h"
#include "engine.h"

#if EFI_IDLE_CONTROL || defined(__DOXYGEN__)

static THD_WORKING_AREA(ivThreadStack, UTILITY_THREAD_STACK_SIZE);

/**
 * here we keep the value we got from IDLE SWITCH input
 */
static volatile int idleSwitchState;

static Logging logger;
EXTERN_ENGINE
;

static SimplePwm idleValvePwm;

/**
 * Idle level calculation algorithm lives in idle_controller.c
 */
static IdleValveState idle;

int getIdleSwitch() {
	return idleSwitchState;
}

void idleDebug(const char *msg, percent_t value) {
	printMsg(&logger, "%s%f", msg, value);
	scheduleLogging(&logger);
}

static void showIdleInfo(void) {
	scheduleMsg(&logger, "idleMode=%s duty=%f", getIdle_mode_e(engineConfiguration->idleMode), boardConfiguration->idleSolenoidPwm);
	scheduleMsg(&logger, "idle valve freq=%d on %s", boardConfiguration->idleSolenoidFrequency,
			hwPortname(boardConfiguration->idleValvePin));
}

static void setIdleControlEnabled(int value) {
	engineConfiguration->idleMode = value ? IM_AUTO : IM_MANUAL;
	showIdleInfo();
}

static void setIdleValvePwm(percent_t value) {
	if (value < 0.01 || value > 99.9)
		return;
	scheduleMsg(&logger, "setting idle valve PWM %f", value);
	float f = 0.01 * value;
	boardConfiguration->idleSolenoidPwm = f;
	showIdleInfo();
	/**
	 * currently idle level is an percent value (0-100 range), and PWM takes a float in the 0..1 range
	 * todo: unify?
	 */
	idleValvePwm.setSimplePwmDutyCycle(f);
}

static msg_t ivThread(int param) {
	(void) param;
	chRegSetThreadName("IdleValve");

	percent_t currentIdleValve = -1;
	while (true) {
		chThdSleepMilliseconds(boardConfiguration->idleThreadPeriod);

		// this value is not used yet
		idleSwitchState = palReadPad(getHwPort(boardConfiguration->idleSwitchPin),
				getHwPin(boardConfiguration->idleSwitchPin));

		if (engineConfiguration->idleMode != IM_AUTO)
			continue;

		efitimems_t now = currentTimeMillis();

		percent_t newValue = idle.getIdle(getRpm(), now PASS_ENGINE_PARAMETER);

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

static void applyIdleSolenoidPinState(PwmConfig *state, int stateIndex) {
	efiAssertVoid(stateIndex < PWM_PHASE_MAX_COUNT, "invalid stateIndex");
	efiAssertVoid(state->multiWave.waveCount == 1, "invalid idle waveCount");
	io_pin_e ioPin = state->outputPins[0];
	int value = state->multiWave.waves[0].pinStates[stateIndex];
	if (!value || engine->rpmCalculator.rpmValue != 0)
		setOutputPinValue(ioPin, value);
}

void startIdleThread(Engine *engine) {
	initLogging(&logger, "Idle Valve Control");

	/**
	 * Start PWM for IDLE_VALVE logical / idleValvePin physical
	 */
	startSimplePwmExt(&idleValvePwm, "Idle Valve", boardConfiguration->idleValvePin, IDLE_VALVE,
			boardConfiguration->idleSolenoidFrequency, boardConfiguration->idleSolenoidPwm, applyIdleSolenoidPinState);

	idle.init();
	scheduleMsg(&logger, "initial idle %d", idle.value);

	chThdCreateStatic(ivThreadStack, sizeof(ivThreadStack), NORMALPRIO, (tfunc_t) ivThread, NULL);

	// this is idle switch INPUT - sometimes there is a switch on the throttle pedal
	// this switch is not used yet
	mySetPadMode2("idle switch", boardConfiguration->idleSwitchPin, PAL_MODE_INPUT);

	addConsoleAction("idleinfo", showIdleInfo);
	addConsoleActionI("set_idle_rpm", setIdleRpmAction);
	addConsoleActionF("set_idle_pwm", setIdleValvePwm);
	addConsoleActionI("set_idle_enabled", (VoidInt) setIdleControlEnabled);
}

#endif
