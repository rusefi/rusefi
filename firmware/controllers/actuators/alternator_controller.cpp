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

AlternatorController::AlternatorController() {
	alternatorPid.initPidClass(&engineConfiguration->alternatorControl);
}

void AlternatorController::onFastCallback() {
	if (!isBrainPinValid(engineConfiguration->alternatorControlPin)) {
		return;
	}

	// this block could be executed even in on/off alternator control mode
	// but at least we would reflect latest state
#if EFI_TUNER_STUDIO
	alternatorPid.postState(engine->outputChannels.alternatorStatus);
#endif /* EFI_TUNER_STUDIO */

	update();
}

expected<float> AlternatorController::getSetpoint() {
	const float rpm = Sensor::getOrZero(SensorType::Rpm);
	bool alternatorShouldBeEnabledAtCurrentRpm = rpm > engineConfiguration->cranking.rpm;

	if (!engineConfiguration->isAlternatorControlEnabled || !alternatorShouldBeEnabledAtCurrentRpm) {
		return unexpected;
	}

	const float load = getEngineState()->fuelingLoad;
	return interpolate3d(
		config->alternatorVoltageTargetTable,
		config->alternatorVoltageTargetLoadBins, load,
		config->alternatorVoltageTargetRpmBins, rpm
	);
}

expected<float> AlternatorController::observePlant() {
	return Sensor::get(SensorType::BatteryVoltage);
}

expected<percent_t> AlternatorController::getOpenLoop(float /*target*/) {
	// see "idle air Bump for AC" comment
	return engine->module<AcController>().unmock().acButtonState ? engineConfiguration->acRelayAlternatorDutyAdder : 0;
}

expected<percent_t> AlternatorController::getClosedLoop(float setpoint, float observation) {
		alternatorPid.iTermMin = engineConfiguration->alternator_iTermMin;
		alternatorPid.iTermMax = engineConfiguration->alternator_iTermMax;
	return alternatorPid.getOutput(setpoint, observation, FAST_CALLBACK_PERIOD_MS / 1000.0f);
}

void AlternatorController::setOutput(expected<percent_t> outputValue) {
	if (outputValue) {
		alternatorControl.setSimplePwmDutyCycle(PERCENT_TO_DUTY(outputValue.Value));
	} else {
		// Shut off output if not needed
		alternatorPid.reset();
		alternatorControl.setSimplePwmDutyCycle(0);
	}
}

void AlternatorController::onConfigurationChange(engine_configuration_s const * previousConfiguration) {
	if(!alternatorPid.isSame(&previousConfiguration->alternatorControl)) {
		alternatorPid.reset();
	}
}

void initAlternatorCtrl() {
	if (!isBrainPinValid(engineConfiguration->alternatorControlPin))
		return;

	startSimplePwm(&alternatorControl,
				"Alternator control",
				&engine->scheduler,
				&enginePins.alternatorPin,
				engineConfiguration->alternatorPwmFrequency, 0);
}

#endif /* EFI_ALTERNATOR_CONTROL */
