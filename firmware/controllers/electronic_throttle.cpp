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
 *
 * set debug_mode 17
 * for PID outputs
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

#include "global.h"
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
#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_TUNER_STUDIO */
static bool shouldResetPid = false;

static pid_s tuneWorkingPidSettings;
static Pid tuneWorkingPid(&tuneWorkingPidSettings);
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
static float valueOverride = NAN;
/*
static SimplePwm etbPwmDown CCM_OPTIONAL;
static OutputPin outputDirectionOpen CCM_OPTIONAL;
*/
static OutputPin outputDirectionClose CCM_OPTIONAL;

EXTERN_ENGINE;

static Pid pid(&engineConfiguration->etb);

static percent_t currentEtbDuty;

static bool wasEtbBraking = false;

// todo: need to fix PWM so that it supports zero duty cycle
#define PERCENT_TO_DUTY(X) (maxF(minI(X, 99.9), 0.1) / 100.0)

static msg_t etbThread(void *arg) {
        UNUSED(arg);
	while (true) {
		// set debug_mode 17
		if (engineConfiguration->debugMode == DBG_ELECTRONIC_THROTTLE_PID) {
#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
			pid.postState(&tsOutputChannels);
#endif /* EFI_TUNER_STUDIO */
		} else if (engineConfiguration->debugMode == DBG_ELECTRONIC_THROTTLE_EXTRA) {
#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
			// set debug_mode 29
			tsOutputChannels.debugFloatField1 = valueOverride;
#endif /* EFI_TUNER_STUDIO */
		}

		if (shouldResetPid) {
			pid.reset();
			shouldResetPid = false;
		}

		if (!cisnan(valueOverride)) {
			etbPwmUp.setSimplePwmDutyCycle(valueOverride);
			pid.sleep();
			continue;
		}

		percent_t actualThrottlePosition = getTPS();

		if (engine->etbAutoTune) {
			autoTune.input = actualThrottlePosition;
			bool result = autoTune.Runtime(&logger);

			tuneWorkingPid.updateFactors(autoTune.output, 0, 0);

			float value = tuneWorkingPid.getValue(50, actualThrottlePosition);
			scheduleMsg(&logger, "AT input=%f output=%f PID=%f", autoTune.input,
					autoTune.output,
					value);
			scheduleMsg(&logger, "AT PID=%f", value);
			etbPwmUp.setSimplePwmDutyCycle(PERCENT_TO_DUTY(value));

			if (result) {
				scheduleMsg(&logger, "GREAT NEWS! %f/%f/%f", autoTune.GetKp(), autoTune.GetKi(), autoTune.GetKd());
			}

			tuneWorkingPid.sleep();
			continue;
		}


		percent_t targetPosition = getPedalPosition(PASS_ENGINE_PARAMETER_SIGNATURE);


		currentEtbDuty = pid.getValue(targetPosition, actualThrottlePosition);

		etbPwmUp.setSimplePwmDutyCycle(PERCENT_TO_DUTY(currentEtbDuty));

		if (boardConfiguration->etbDirectionPin2 != GPIO_UNASSIGNED) {
			bool needEtbBraking = absF(targetPosition - actualThrottlePosition) < 3;
			if (needEtbBraking != wasEtbBraking) {
				scheduleMsg(&logger, "need ETB braking: %d", needEtbBraking);
				wasEtbBraking = needEtbBraking;
			}
			outputDirectionClose.setValue(needEtbBraking);
		}

		if (engineConfiguration->isVerboseETB) {
			pid.showPidStatus(&logger, "ETB");
		}


		// this thread is activated 10 times per second
		pid.sleep();
	}
#if defined __GNUC__
	return -1;
#endif
}

/**
 * set_etb X
 * manual duty cycle control without PID. Percent value from 0 to 100
 */
static void setThrottleDutyCycle(float level) {
	scheduleMsg(&logger, "setting ETB duty=%f%%", level);
	if (cisnan(level)) {
		valueOverride = NAN;
		return;
	}

	float dc = PERCENT_TO_DUTY(level);
	valueOverride = dc;
	etbPwmUp.setSimplePwmDutyCycle(dc);
	scheduleMsg(&logger, "duty ETB duty=%f", dc);
}

static void showEthInfo(void) {
	static char pinNameBuffer[16];

	scheduleMsg(&logger, "etbAutoTune=%d",
			engine->etbAutoTune);

	scheduleMsg(&logger, "throttlePedal=%.2f %.2f/%.2f @%s",
			getPedalPosition(),
			engineConfiguration->throttlePedalUpVoltage,
			engineConfiguration->throttlePedalWOTVoltage,
			getPinNameByAdcChannel("tPedal", engineConfiguration->pedalPositionAdcChannel, pinNameBuffer));

	scheduleMsg(&logger, "TPS=%.2f", getTPS());

	scheduleMsg(&logger, "etbControlPin1=%s duty=%.2f freq=%d",
			hwPortname(boardConfiguration->etbControlPin1),
			currentEtbDuty,
			engineConfiguration->etbFreq);
	scheduleMsg(&logger, "close dir=%s", hwPortname(boardConfiguration->etbDirectionPin2));
	pid.showPidStatus(&logger, "ETB");
}

void setEtbPFactor(float value) {
	engineConfiguration->etb.pFactor = value;
	pid.reset();
	showEthInfo();
}

void setEtbIFactor(float value) {
	engineConfiguration->etb.iFactor = value;
	pid.reset();
	showEthInfo();
}

void setEtbDFactor(float value) {
	engineConfiguration->etb.dFactor = value;
	pid.reset();
	showEthInfo();
}

void setEtbOffset(int value) {
	engineConfiguration->etb.offset = value;
	pid.reset();
	showEthInfo();
}

void setDefaultEtbParameters(void) {
	engineConfiguration->throttlePedalUpVoltage = 0; // that's voltage, not ADC like with TPS
	engineConfiguration->throttlePedalWOTVoltage = 6; // that's voltage, not ADC like with TPS

	engineConfiguration->etb.pFactor = 1;
	engineConfiguration->etb.iFactor = 0.5;
	engineConfiguration->etb.period = 100;
	engineConfiguration->etbFreq = 300;

//	boardConfiguration->etbControlPin1 = GPIOE_4; // test board, matched default fuel pump relay
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
/*
	startSimplePwmExt(&etbPwmDown, "etb2",
			boardConfiguration->etbControlPin2,
			&enginePins.etbOutput2,
			freq,
			0.80,
			applyPinState);
	outputDirectionOpen.initPin("etb dir open", boardConfiguration->etbDirectionPin1);
*/
	outputDirectionClose.initPin("etb dir close", boardConfiguration->etbDirectionPin2);
}

static void setTempOutput(float value) {
	autoTune.output = value;
}

/**
 * set_etbat_step X
 */
static void setAutoStep(float value) {
	autoTune.reset();
	autoTune.SetOutputStep(value);
}

static void setAutoPeriod(int period) {
	tuneWorkingPidSettings.period = period;
	autoTune.reset();
}

static void setAutoOffset(int offset) {
	tuneWorkingPidSettings.offset = offset;
	autoTune.reset();
}

void initElectronicThrottle(void) {
	addConsoleAction("ethinfo", showEthInfo);
	if (!hasPedalPositionSensor()) {
		return;
	}
	autoTune.SetOutputStep(0.1);

	startETBPins();

	// manual duty cycle control without PID. Percent value from 0 to 100
	addConsoleActionNANF("set_etb", setThrottleDutyCycle);


	tuneWorkingPidSettings.pFactor = 1;
	tuneWorkingPidSettings.iFactor = 0;
	tuneWorkingPidSettings.dFactor = 0;
//	tuneWorkingPidSettings.offset = 10; // todo: not hard-coded value
	//todo tuneWorkingPidSettings.period = 10;
	tuneWorkingPidSettings.minValue = 0;
	tuneWorkingPidSettings.maxValue = 100;
	tuneWorkingPidSettings.period = 100;

	// this is useful one you do "enable etb_auto"
	addConsoleActionF("set_etbat_output", setTempOutput);
	addConsoleActionF("set_etbat_step", setAutoStep);
	addConsoleActionI("set_etbat_period", setAutoPeriod);
	addConsoleActionI("set_etbat_offset", setAutoOffset);

	pid.reset();

	chThdCreateStatic(etbTreadStack, sizeof(etbTreadStack), NORMALPRIO, (tfunc_t) etbThread, NULL);
}

#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

