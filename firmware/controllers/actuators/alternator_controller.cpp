/**
 * @file    alternator_controller.cpp
 * @brief   alternator controller - some newer vehicles control alternator with ECU
 *
 * @date Apr 6, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if EFI_ALTERNATOR_CONTROL
#include "alternator_controller.h"
#include "efi_pid.h"
#include "local_version_holder.h"
#include "periodic_task.h"

#if defined(HAS_OS_ACCESS)
#error "Unexpected OS ACCESS HERE"
#endif /* HAS_OS_ACCESS */

static SimplePwm alternatorControl("alt");

static percent_t currentAltDuty;

static bool shouldResetPid = false;

void AlternatorController::init() {
	alternatorPid.initPidClass(&engineConfiguration->alternatorControl);
}

expected<float> AlternatorController::observePlant() {
	auto vBatt = Sensor::get(SensorType::BatteryVoltage);
	// if somehow battery voltage isn't valid
	if (!vBatt)
		return unexpected;
	return vBatt.Value;
}

expected<float> AlternatorController::getSetpoint() {
	// check if the engine is not running
	bool alternatorShouldBeEnabledAtCurrentRpm = Sensor::getOrZero(SensorType::Rpm) > engineConfiguration->cranking.rpm;

	if (!engineConfiguration->isAlternatorControlEnabled || !alternatorShouldBeEnabledAtCurrentRpm) {
		return unexpected;
	}

	return engineConfiguration->targetVBatt;
}

expected<percent_t> AlternatorController::getOpenLoop(float target) {
	UNUSED(target);
	return 0;
}

expected<percent_t> AlternatorController::getClosedLoop(float targetVoltage, float vBattVoltage) {
	percent_t altDuty = alternatorPid.getOutput(targetVoltage, vBattVoltage, FAST_CALLBACK_PERIOD_MS / 1000.0f);

	// this block could be executed even in on/off alternator control mode
	// but at least we would reflect latest state
#if EFI_TUNER_STUDIO
	alternatorPid.postState(engine->outputChannels.alternatorStatus);
#endif /* EFI_TUNER_STUDIO */

   	// see "idle air Bump for AC" comment
	int acDutyBump = engine->module<AcController>().unmock().acButtonState ? engineConfiguration->acRelayAlternatorDutyAdder : 0;
	altDuty += acDutyBump;
	return altDuty;
}

void AlternatorController::setOutput(expected<percent_t> outputValue) {
	if (outputValue) {
		currentAltDuty = outputValue.Value;
		alternatorControl.setSimplePwmDutyCycle(PERCENT_TO_DUTY(outputValue.Value));
	} else {
		// Shut off output if not needed
		alternatorControl.setSimplePwmDutyCycle(0);
		// we need to avoid accumulating iTerm while the alternator is not working
		pidReset();
	}
}

void AlternatorController::pidReset() {
	alternatorPid.reset();
}

void AlternatorController::onFastCallback() {
	if (!isBrainPinValid(engineConfiguration->alternatorControlPin)) {
		return;
	}
#if ! EFI_UNIT_TEST
	if (shouldResetPid) {
		pidReset();
		shouldResetPid = false;
	}
#endif

	ClosedLoopController::update();
}


void showAltInfo(void) {
	efiPrintf("alt=%s @%s t=%dms", boolToString(engineConfiguration->isAlternatorControlEnabled),
			hwPortname(engineConfiguration->alternatorControlPin),
			engineConfiguration->alternatorControl.periodMs);
	efiPrintf("p=%.2f/i=%.2f/d=%.2f offset=%.2f", engineConfiguration->alternatorControl.pFactor,
			0, 0, engineConfiguration->alternatorControl.offset); // todo: i & d
	efiPrintf("vbatt=%.2f/duty=%.2f/target=%.2f", Sensor::getOrZero(SensorType::BatteryVoltage), currentAltDuty,
			engineConfiguration->targetVBatt);
}

void setAltPFactor(float p) {
	engineConfiguration->alternatorControl.pFactor = p;
	efiPrintf("setAltPid: %.2f", p);
	engine->module<AlternatorController>()->pidReset();
	showAltInfo();
}

void AlternatorController::onConfigurationChange(engine_configuration_s const * previousConfiguration) {
	shouldResetPid = !previousConfiguration || !alternatorPid.isSame(&previousConfiguration->alternatorControl);
}

void initAlternatorCtrl() {
	addConsoleAction("altinfo", showAltInfo);

	engine->module<AlternatorController>()->init();

	if (!isBrainPinValid(engineConfiguration->alternatorControlPin))
		return;

	startSimplePwm(&alternatorControl,
				"Alternator control",
				&engine->executor,
				&enginePins.alternatorPin,
				engineConfiguration->alternatorPwmFrequency, 0);
}

#endif /* EFI_ALTERNATOR_CONTROL */
