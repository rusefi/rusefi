/**
 * @file	electronic_throttle.cpp
 * @brief	Electronic Throttle Module driver L298N
 *
 * todo: make this more universal if/when we get other hardware options
 *
 * Jan 2017 status:
 * Electronic throttle body with it's spring is definitely not linear - both P and I factors of PID are required to get any results
 *  PID implementation tested on a bench only
 *  it is believed that more than just PID would be needed, as is this is probably
 *  not usable on a real vehicle. Needs to be tested :)
 *
 *
 *
 *  ETB is controlled according to pedal position input (pedal position sensor is a potentiometer)
 *    petal 0% means pedal not pressed / idle
 *    pedal 100% means pedal all the way down
 *  (not TPS - not the one you can calibrate in TunerStudio)
 *
 *  At the moment we only control opening motor - while relying on ETB spring to move throttle butterfly
 *  back. Throttle position sensor inside ETB is used for closed-loop PID control of ETB.
 *
 * See also pid.cpp
 *
 * Relevant console commands:
 *
 * enable verbose_etb
 * disable verbose_etb
 * ethinfo
 * set mock_pedal_position X
 *
 * http://rusefi.com/forum/viewtopic.php?f=5&t=592
 *
 * @date Dec 7, 2013
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

#include "main.h"
#include "electronic_throttle.h"
#include "tps.h"
#include "io_pins.h"
#include "engine_configuration.h"
#include "pwm_generator_logic.h"
#include "pid.h"
#include "engine_controller.h"

#if EFI_ELECTRONIC_THROTTLE_BODY || defined(__DOXYGEN__)
#include "pin_repository.h"
#include "pwm_generator.h"
#include "pid_auto_tune.h"
extern TunerStudioOutputChannels tsOutputChannels;
static bool shouldResetPid = false;

static PID_AutoTune autoTune;

static LoggingWithStorage logger("ETB");
/**
 * @brief Control Thread stack
 */
static THD_WORKING_AREA(etbTreadStack, UTILITY_THREAD_STACK_SIZE);
/**
 * @brief Pulse-Width Modulation state
 */
static SimplePwm etbPwmUp CCM_OPTIONAL;
static SimplePwm etbPwmDown CCM_OPTIONAL;

static OutputPin outputDirectionOpen CCM_OPTIONAL;
static OutputPin outputDirectionClose CCM_OPTIONAL;

EXTERN_ENGINE;

static Pid pid(&engineConfiguration->etb);

//static float prevTps;

static percent_t currentEtbDuty;

static bool wasEtbBraking = false;

static msg_t etbThread(void *arg) {
        UNUSED(arg);
	while (true) {
		if (shouldResetPid) {
			pid.reset();
//			alternatorPidResetCounter++;
			shouldResetPid = false;
		}

		if (engine->etbAutoTune) {
			autoTune.Runtime(&logger);

			etbPwmUp.setSimplePwmDutyCycle(autoTune.output);

			pid.sleep();
			continue;
		}


		percent_t targetPosition = getPedalPosition(PASS_ENGINE_PARAMETER_SIGNATURE);

		percent_t actualThrottlePosition = getTPS();

		currentEtbDuty = pid.getValue(targetPosition, actualThrottlePosition);

		etbPwmUp.setSimplePwmDutyCycle(currentEtbDuty / 100);

		bool needEtbBraking = absF(targetPosition - actualThrottlePosition) < 3;
		if (needEtbBraking != wasEtbBraking) {
			scheduleMsg(&logger, "need ETB braking: %d", needEtbBraking);
			wasEtbBraking = needEtbBraking;
		}
		outputDirectionClose.setValue(needEtbBraking);

		if (engineConfiguration->debugMode == DBG_ELECTRONIC_THROTTLE) {
			pid.postState(&tsOutputChannels);
		}
		if (engineConfiguration->isVerboseETB) {
			pid.showPidStatus(&logger, "ETB");
		}


//		if (tps != prevTps) {
//			prevTps = tps;
//			scheduleMsg(&logger, "tps=%d", (int) tps);
//		}

		// this thread is activated 10 times per second
		pid.sleep();
	}
#if defined __GNUC__
	return -1;
#endif
}

static void setThrottleConsole(int level) {
	scheduleMsg(&logger, "setting throttle=%d", level);

	float dc = 0.01 + (minI(level, 98)) / 100.0;
	etbPwmUp.setSimplePwmDutyCycle(dc);
	print("st = %.2f\r\n", dc);
}

static void showEthInfo(void) {
	static char pinNameBuffer[16];

	scheduleMsg(&logger, "etbAutoTune=%d",
			engine->etbAutoTune);

	scheduleMsg(&logger, "throttlePedal=%.2f %.2f/%.2f @%s",
			getPedalPosition(),
			engineConfiguration->throttlePedalUpVoltage,
			engineConfiguration->throttlePedalWOTVoltage,
			getPinNameByAdcChannel("tPedal", engineConfiguration->pedalPositionChannel, pinNameBuffer));

	scheduleMsg(&logger, "TPS=%.2f", getTPS());

	scheduleMsg(&logger, "etbControlPin1=%s duty=%.2f freq=%d",
			hwPortname(boardConfiguration->etbControlPin1),
			currentEtbDuty,
			engineConfiguration->etbFreq);
	scheduleMsg(&logger, "close dir=%s", hwPortname(boardConfiguration->etbDirectionPin2));
	pid.showPidStatus(&logger, "ETB");
}

static void apply(void) {
	pid.updateFactors(engineConfiguration->etb.pFactor, engineConfiguration->etb.iFactor, 0);
}

void setEtbPFactor(float value) {
	engineConfiguration->etb.pFactor = value;
	apply();
	showEthInfo();
}

void setEtbIFactor(float value) {
	engineConfiguration->etb.iFactor = value;
	apply();
	showEthInfo();
}

void setDefaultEtbParameters(void) {
	engineConfiguration->throttlePedalUpVoltage = 0; // that's voltage, not ADC like with TPS
	engineConfiguration->throttlePedalWOTVoltage = 6; // that's voltage, not ADC like with TPS

	engineConfiguration->etb.pFactor = 1;
	engineConfiguration->etb.iFactor = 0.5;
	engineConfiguration->etb.period = 100;
	engineConfiguration->etbFreq = 300;
}

bool isETBRestartNeeded(void) {
	/**
	 * We do not want any interruption in HW pin while adjusting other properties
	 */
	return engineConfiguration->bc.etbControlPin1 != activeConfiguration.bc.etbControlPin1 ||
		   engineConfiguration->bc.etbControlPin2 != activeConfiguration.bc.etbControlPin2 ||
		   engineConfiguration->bc.etbDirectionPin1 != activeConfiguration.bc.etbDirectionPin1 ||
		   engineConfiguration->bc.etbDirectionPin2 != activeConfiguration.bc.etbDirectionPin2;
}

void stopETBPins(void) {
	unmarkPin(activeConfiguration.bc.etbControlPin1);
	unmarkPin(activeConfiguration.bc.etbControlPin2);
	unmarkPin(activeConfiguration.bc.etbDirectionPin1);
	unmarkPin(activeConfiguration.bc.etbDirectionPin2);
}

void onConfigurationChangeElectronicThrottleCallback(engine_configuration_s *previousConfiguration) {
	shouldResetPid = !pid.isSame(&previousConfiguration->etb);
}

void startETBPins(void) {

	int freq = maxI(100, engineConfiguration->etbFreq);

	// this line used for PWM
	startSimplePwmExt(&etbPwmUp, "etb1",
			boardConfiguration->etbControlPin1,
			&enginePins.etbOutput1,
			freq,
			0.80,
			applyPinState);
	startSimplePwmExt(&etbPwmDown, "etb2",
			boardConfiguration->etbControlPin2,
			&enginePins.etbOutput2,
			freq,
			0.80,
			applyPinState);

	outputDirectionOpen.initPin("etb dir open", boardConfiguration->etbDirectionPin1);
	outputDirectionClose.initPin("etb dir close", boardConfiguration->etbDirectionPin2);
}

static void setTempOutput(float value) {
	autoTune.output = value;

}

static void setTempStep(float value) {
	autoTune.oStep = value;

}

void initElectronicThrottle(void) {
	// these two lines are controlling direction
//	outputPinRegister("etb1", ELECTRONIC_THROTTLE_CONTROL_1, ETB_CONTROL_LINE_1_PORT, ETB_CONTROL_LINE_1_PIN);
//	outputPinRegister("etb2", ELECTRONIC_THROTTLE_CONTROL_2, ETB_CONTROL_LINE_2_PORT, ETB_CONTROL_LINE_2_PIN);

	addConsoleAction("ethinfo", showEthInfo);
	if (!hasPedalPositionSensor()) {
		return;
	}

	startETBPins();

	addConsoleActionI("set_etb", setThrottleConsole);

	addConsoleActionF("set_etb_output", setTempOutput);
	addConsoleActionF("set_etb_step", setTempStep);

	apply();

	chThdCreateStatic(etbTreadStack, sizeof(etbTreadStack), NORMALPRIO, (tfunc_t) etbThread, NULL);
}

#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

