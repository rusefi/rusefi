//
// Created by kifir on 9/27/24.
//

#include "pch.h"

#if EFI_LAUNCH_CONTROL
#include "shift_torque_reduction_controller.h"
#include "boost_control.h"
#include "launch_control.h"
#include "advance_map.h"
#include "engine_state.h"
#include "advance_map.h"
#include "tinymt32.h"

void ShiftTorqueReductionController::update() {
    if (engineConfiguration->torqueReductionEnabled) {
		updateTriggerPinState();
		updateTimeConditionSatisfied();
        updateRpmConditionSatisfied();
        updateAppConditionSatisfied();

        isFlatShiftConditionSatisfied = torqueReductionTriggerPinState && isTimeConditionSatisfied
            && isRpmConditionSatisfied && isAppConditionSatisfied;
    }
}

void ShiftTorqueReductionController::updateTriggerPinState() {
    switch (engineConfiguration->torqueReductionActivationMode) {
        case TORQUE_REDUCTION_BUTTON: {
            updateTriggerPinState(
                engineConfiguration->torqueReductionTriggerPin,
                engineConfiguration->torqueReductionTriggerPinInverted
            );
            break;
        }
        case LAUNCH_BUTTON: {
            updateTriggerPinState(
                engineConfiguration->launchActivatePin,
                engineConfiguration->launchActivateInverted
            );
            break;
        }
        default: {
            break; // we shouldn't be here!
        }
    }
}

void ShiftTorqueReductionController::updateTriggerPinState(const switch_input_pin_e pin, const bool isPinInverted) {
#if !EFI_SIMULATOR
    isTorqueReductionTriggerPinValid = isBrainPinValid(pin);
    if (isTorqueReductionTriggerPinValid) {
		const bool previousTorqueReductionTriggerPinState = torqueReductionTriggerPinState;
        torqueReductionTriggerPinState = isPinInverted ^ efiReadPin(pin);
		if (!previousTorqueReductionTriggerPinState && torqueReductionTriggerPinState) {
			m_pinTriggeredTimer.reset();
		}
    } else {
        torqueReductionTriggerPinState = false;
    }
#endif // !EFI_SIMULATOR
}

void ShiftTorqueReductionController::updateTimeConditionSatisfied() {
	isTimeConditionSatisfied = torqueReductionTriggerPinState
	    ? (0.0f < engineConfiguration->torqueReductionTime)
	        && !m_pinTriggeredTimer.hasElapsedMs(engineConfiguration->torqueReductionTime)
	    : false;
}

void ShiftTorqueReductionController::updateRpmConditionSatisfied() {
    const float currentRpm = Sensor::getOrZero(SensorType::Rpm);
    isRpmConditionSatisfied = (engineConfiguration->torqueReductionArmingRpm <= currentRpm);
}

void ShiftTorqueReductionController::updateAppConditionSatisfied() {
	const SensorResult currentApp = Sensor::get(SensorType::DriverThrottleIntent);

    if (currentApp.Valid) {
        isAppConditionSatisfied = (engineConfiguration->torqueReductionArmingApp <= currentApp.Value);
    } else {
        isAppConditionSatisfied = false;
    }
}

#endif // EFI_LAUNCH_CONTROL