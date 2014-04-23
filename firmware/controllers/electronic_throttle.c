/**
 * @file	electronic_throttle.c
 * @brief	Electronic Throttle Module driver L298N
 *
 * todo: make this more universal if/when we get other hardware options
 *
 * @date Dec 7, 2013
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
#include "electronic_throttle.h"
#include "boards.h"
#include "pin_repository.h"
#include "idle_controller.h"
#include "tps.h"
#include "io_pins.h"
#include "engine_configuration.h"
#include "pwm_generator.h"
#include "pwm_generator_logic.h"

#if EFI_ELECTRONIC_THROTTLE_BODY

static Logging logger;
/**
 * @brief Control Thread stack
 */
static WORKING_AREA(etbTreadStack, UTILITY_THREAD_STACK_SIZE);
/**
 * @brief Pulse-Width Modulation state
 */
static PwmConfig etbPwm;

static float prevTps;

extern engine_configuration_s *engineConfiguration;
extern board_configuration_s *boardConfiguration;

static msg_t etbThread(void *arg) {
	while (TRUE) {
		int tps = (int)getTPS();

		if (tps != prevTps) {
			prevTps = tps;
			scheduleMsg(&logger, "tps=%d", (int) tps);
		}

		// this thread is activated 10 times per second
		chThdSleepMilliseconds(100);
	}
#if defined __GNUC__
	return -1;
#endif
}

static void setThrottleConsole(int level) {
	scheduleMsg(&logger, "setting throttle=%d", level);

	etbPwm.multiWave.switchTimes[0] = 0.01 + (min(level, 98)) / 100.0;
	print("st = %f\r\n", etbPwm.multiWave.switchTimes[0]);
}

void initElectronicThrottle(void) {
	initLogging(&logger, "Electronic Throttle");

	engineConfiguration->tpsMin = 140;
	engineConfiguration->tpsMax = 898;

	// these two lines are controlling direction
//	outputPinRegister("etb1", ELECTRONIC_THROTTLE_CONTROL_1, ETB_CONTROL_LINE_1_PORT, ETB_CONTROL_LINE_1_PIN);
//	outputPinRegister("etb2", ELECTRONIC_THROTTLE_CONTROL_2, ETB_CONTROL_LINE_2_PORT, ETB_CONTROL_LINE_2_PIN);

	// this line used for PWM
	startSimplePwm(&etbPwm, "etb",
			boardConfiguration->electronicThrottlePin1,
			ELECTRONIC_THROTTLE_CONTROL_1,
			0.80,
			500);

	addConsoleActionI("e", setThrottleConsole);
	chThdCreateStatic(etbTreadStack, sizeof(etbTreadStack), NORMALPRIO, (tfunc_t) etbThread, NULL);
}
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

