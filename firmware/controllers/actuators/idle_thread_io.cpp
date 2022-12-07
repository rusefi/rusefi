/*
 * @file idle_thread_io.cpp
 *
 *
 * enable verbose_idle
 * disable verbose_idle
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
 *
 * @date Oct 17, 2021
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if ! EFI_UNIT_TEST
#include "dc_motors.h"
#include "idle_hardware.h"

static void showIdleInfo() {
	const char * idleModeStr = getIdle_mode_e(engineConfiguration->idleMode);
	efiPrintf("useStepperIdle=%s useHbridges=%s useRawOutput=%s",
			boolToString(engineConfiguration->useStepperIdle),
			boolToString(engineConfiguration->useHbridgesToDriveIdleStepper),
			boolToString(engineConfiguration->useRawOutputToDriveIdleStepper));
	efiPrintf("idleMode=%s position=%.2f",
			idleModeStr, getIdlePosition());

	if (engineConfiguration->useStepperIdle) {
		if (engineConfiguration->useRawOutputToDriveIdleStepper) {
			efiPrintf(" A+=%s", hwPortname(engineConfiguration->stepper_raw_output[0]));
			efiPrintf(" A-=%s", hwPortname(engineConfiguration->stepper_raw_output[1]));
			efiPrintf(" B+=%s", hwPortname(engineConfiguration->stepper_raw_output[2]));
			efiPrintf(" B-=%s", hwPortname(engineConfiguration->stepper_raw_output[3]));
		} else if (engineConfiguration->useHbridgesToDriveIdleStepper) {
			efiPrintf("Coil A:");
			efiPrintf(" pin1=%s", hwPortname(engineConfiguration->stepperDcIo[0].directionPin1));
			efiPrintf(" pin2=%s", hwPortname(engineConfiguration->stepperDcIo[0].directionPin2));
			showDcMotorInfo(2);
			efiPrintf("Coil B:");
			efiPrintf(" pin1=%s", hwPortname(engineConfiguration->stepperDcIo[1].directionPin1));
			efiPrintf(" pin2=%s", hwPortname(engineConfiguration->stepperDcIo[1].directionPin2));
			showDcMotorInfo(3);
		} else {
			efiPrintf("directionPin=%s reactionTime=%.2f", hwPortname(engineConfiguration->idle.stepperDirectionPin),
					engineConfiguration->idleStepperReactionTime);
			efiPrintf("stepPin=%s steps=%d", hwPortname(engineConfiguration->idle.stepperStepPin),
					engineConfiguration->idleStepperTotalSteps);
			efiPrintf("enablePin=%s/%d", hwPortname(engineConfiguration->stepperEnablePin),
					engineConfiguration->stepperEnablePinMode);
		}
	} else {
		if (!engineConfiguration->isDoubleSolenoidIdle) {
			efiPrintf("idle valve freq=%d on %s", engineConfiguration->idle.solenoidFrequency,
					hwPortname(engineConfiguration->idle.solenoidPin));
		} else {
			efiPrintf("idle valve freq=%d on %s", engineConfiguration->idle.solenoidFrequency,
					hwPortname(engineConfiguration->idle.solenoidPin));
			efiPrintf(" and %s", hwPortname(engineConfiguration->secondSolenoidPin));
		}
	}

	if (engineConfiguration->idleMode == IM_AUTO) {
		engine->module<IdleController>().unmock().getIdlePid()->showPidStatus("idle");
	}
}

void setIdleMode(idle_mode_e value) {
	engineConfiguration->idleMode = value ? IM_AUTO : IM_MANUAL;
	showIdleInfo();
}

void setManualIdleValvePosition(int positionPercent) {
	if (positionPercent < 1 || positionPercent > 99)
		return;
	efiPrintf("setting idle valve position %d", positionPercent);
	showIdleInfo();
	// todo: this is not great that we have to write into configuration here
	engineConfiguration->manIdlePosition = positionPercent;
}

#endif /* EFI_UNIT_TEST */

#if EFI_PROD_CODE
static void startInputPinIfValid(const char *msg, brain_pin_e pin, pin_input_mode_e mode) {
	if (isBrainPinValid(pin)) {
		efiSetPadMode(msg, pin, getInputMode(mode));
	}
}
#endif // EFI_PROD_CODE

percent_t getIdlePosition() {
	return engine->module<IdleController>().unmock().currentIdlePosition;
}

void startPedalPins() {
#if EFI_PROD_CODE
	// this is neutral/no gear switch input. on Miata it's wired both to clutch pedal and neutral in gearbox
	// this switch is not used yet
	startInputPinIfValid("clutch down switch", engineConfiguration->clutchDownPin, engineConfiguration->clutchDownPinMode);

	startInputPinIfValid("clutch up switch", engineConfiguration->clutchUpPin, engineConfiguration->clutchUpPinMode);

	startInputPinIfValid("throttle pedal up switch", engineConfiguration->throttlePedalUpPin, engineConfiguration->throttlePedalUpPinMode);

	startInputPinIfValid("brake pedal switch", engineConfiguration->brakePedalPin, engineConfiguration->brakePedalPinMode);
	startInputPinIfValid("Launch Button", engineConfiguration->launchActivatePin, engineConfiguration->launchActivatePinMode);
#endif /* EFI_PROD_CODE */
}

void stopPedalPins() {
	brain_pin_markUnused(activeConfiguration.clutchUpPin);
	brain_pin_markUnused(activeConfiguration.clutchDownPin);
	brain_pin_markUnused(activeConfiguration.throttlePedalUpPin);
	brain_pin_markUnused(activeConfiguration.brakePedalPin);
	brain_pin_markUnused(activeConfiguration.launchActivatePin);
}

#if ! EFI_UNIT_TEST

static void applyPidSettings() {
	engine->module<IdleController>().unmock().getIdlePid()->updateFactors(engineConfiguration->idleRpmPid.pFactor, engineConfiguration->idleRpmPid.iFactor, engineConfiguration->idleRpmPid.dFactor);
}

void setTargetIdleRpm(int value) {
	setTargetRpmCurve(value);
	efiPrintf("target idle RPM %d", value);
	showIdleInfo();
}

void setIdlePFactor(float value) {
	engineConfiguration->idleRpmPid.pFactor = value;
	applyPidSettings();
	showIdleInfo();
}

void setIdleIFactor(float value) {
	engineConfiguration->idleRpmPid.iFactor = value;
	applyPidSettings();
	showIdleInfo();
}

void setIdleDFactor(float value) {
	engineConfiguration->idleRpmPid.dFactor = value;
	applyPidSettings();
	showIdleInfo();
}

/**
 * Idle test would activate the solenoid for three seconds
 */
void startIdleBench(void) {
	engine->timeToStopIdleTest = getTimeNowUs() + MS2US(3000); // 3 seconds
	efiPrintf("idle valve bench test");
	showIdleInfo();
}

#endif /* EFI_UNIT_TEST */

#if EFI_IDLE_CONTROL


void setDefaultIdleParameters() {
	engineConfiguration->idleRpmPid.pFactor = 0.01f;
	engineConfiguration->idleRpmPid.iFactor = 0.05f;
	engineConfiguration->idleRpmPid.dFactor = 0.0f;

	engineConfiguration->idlerpmpid_iTermMin = -20;
	engineConfiguration->idlerpmpid_iTermMax =  20;

	// Good starting point is 10 degrees per 100 rpm, aka 0.1 deg/rpm
	engineConfiguration->idleTimingPid.pFactor = 0.1f;
	engineConfiguration->idleTimingPid.iFactor = 0;
	engineConfiguration->idleTimingPid.dFactor = 0;

	// Allow +- 10 degrees adjustment
	engineConfiguration->idleTimingPid.minValue = -10;
	engineConfiguration->idleTimingPid.maxValue = 10;

	// Idle region is target + 100 RPM
	engineConfiguration->idlePidRpmUpperLimit = 100;

	engineConfiguration->idlePidRpmDeadZone = 50;
}

/**
 * I use this questionable feature to tune acceleration enrichment
 */
static void blipIdle(int idlePosition, int durationMs) {
#if ! EFI_UNIT_TEST
	if (engine->timeToStopBlip != 0) {
		return; // already in idle blip
	}
	engine->blipIdlePosition = idlePosition;
	engine->timeToStopBlip = getTimeNowUs() + 1000 * durationMs;
#endif // EFI_UNIT_TEST
}

void startIdleThread() {
	// Force the idle controller to use 0 offset, as this is handled by the open loop table instead.
	engineConfiguration->idleRpmPid.offset = 0;

	IdleController *controller = &engine->module<IdleController>().unmock();

	controller->init();

#if ! EFI_UNIT_TEST
	// todo: we still have to explicitly init all hardware on start in addition to handling configuration change via
	// 'applyNewHardwareSettings' todo: maybe unify these two use-cases?
	initIdleHardware();
#endif /* EFI_UNIT_TEST */

	controller->idleState = INIT;
	controller->baseIdlePosition = -100.0f;
	controller->currentIdlePosition = -100.0f;

#if ! EFI_UNIT_TEST

	addConsoleAction("idleinfo", showIdleInfo);

	addConsoleActionII("blipidle", blipIdle);

	// split this whole file into manual controller and auto controller? move these commands into the file
	// which would be dedicated to just auto-controller?

	addConsoleAction("idlebench", startIdleBench);
	applyPidSettings();
#endif /* EFI_UNIT_TEST */
}

#endif /* EFI_IDLE_CONTROL */
