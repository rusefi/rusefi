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
#include "gppwm_channel_reader.h"

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

float ShiftTorqueReductionController::getSparkSkipRatio() {
    float result = 0.0f;
    auto torqueReductionXAxis = readGppwmChannel(config->torqueReductionCutXaxis);
    trqRedCutXaxisValue = torqueReductionXAxis.Value;
    int8_t currentGear = Sensor::getOrZero(SensorType::DetectedGear);

    if (engineConfiguration->torqueReductionEnabled && isFlatShiftConditionSatisfied) {
        result =  interpolate3d(
                config->torqueReductionIgnitionCutTable,
                config->torqueReductionCutGearBins, currentGear,
                config->torqueReductionCutXBins, torqueReductionXAxis.Value
            ) / 100.0f;
    }
    return result;
}

void ShiftTorqueReductionController::updateTriggerPinState() {
    switch (engineConfiguration->torqueReductionActivationMode) {
        case TORQUE_REDUCTION_BUTTON: {
            updateTriggerPinState(
                engineConfiguration->torqueReductionTriggerPin,
                engineConfiguration->torqueReductionTriggerPinMode,
                /*invertPhysicalPin*/false,
                engine->engineState.lua.torqueReductionState
            );
            break;
        }
        case LAUNCH_BUTTON: {
            updateTriggerPinState(
                engineConfiguration->launchActivatePin,
                engineConfiguration->launchActivatePinMode,
                /*invertPhysicalPin*/false,
                false
            );
            break;
        }
        case TORQUE_REDUCTION_CLUTCH_DOWN_SWITCH: {
            updateTriggerPinState(
                engineConfiguration->clutchDownPin,
                engineConfiguration->clutchDownPinMode,
                /*invertPhysicalPin*/false,
                engine->engineState.lua.clutchDownState
            );
            break;
        }
        case TORQUE_REDUCTION_CLUTCH_UP_SWITCH: {
            updateTriggerPinState(
                engineConfiguration->clutchUpPin,
                engineConfiguration->clutchUpPinMode,
                /*invertPhysicalPin*/true,
                engine->engineState.lua.clutchUpState
            );
            break;
        }
        default: {
            break; // we shouldn't be here!
        }
    }
}

static bool isShiftTorqueBelowTemperatureThreshold() {
  if (engineConfiguration->torqueReductionActivationTemperature == 0) {
    return false;
  }
  return Sensor::getOrZero(SensorType::Clt) < engineConfiguration->torqueReductionActivationTemperature;
}

// todo: rename method since we now check temperature not just pin state
void ShiftTorqueReductionController::updateTriggerPinState(
    const switch_input_pin_e pin,
    const pin_input_mode_e mode,
    const bool invertPhysicalPin,
    const bool invalidPinState
) {
  if (!torqueReductionTriggerPinState) {
    isBelowTemperatureThreshold = isShiftTorqueBelowTemperatureThreshold();
    if (isBelowTemperatureThreshold) {
        // disable by not even reading control pin below threshold temperature, unless already active
      return;
    }
  }

#if !EFI_SIMULATOR
    isTorqueReductionTriggerPinValid = isBrainPinValid(pin);
    const bool previousTorqueReductionTriggerPinState = torqueReductionTriggerPinState;
    if (isTorqueReductionTriggerPinValid) {
        torqueReductionTriggerPinState = efiReadPin(pin, mode) ^ invertPhysicalPin;
    } else {
        torqueReductionTriggerPinState = invalidPinState;
    }
    if (!previousTorqueReductionTriggerPinState && torqueReductionTriggerPinState) {
    m_pinTriggeredTimer.reset();
    }
#endif // !EFI_SIMULATOR
}

void ShiftTorqueReductionController::updateTimeConditionSatisfied() {
    auto torqueReductionTimeXaxis = readGppwmChannel(config->torqueReductionTimeXaxis);
	trqRedTimeXaxisValue = torqueReductionTimeXaxis.Value;
    int8_t currentGear = Sensor::getOrZero(SensorType::DetectedGear);

    auto torqueReductionTime = interpolate3d(
        config->torqueReductionTimeTable,
        config->torqueReductionTimeGearBins, currentGear,
        config->torqueReductionTimeXBins, torqueReductionTimeXaxis.Value
    );

    isTimeConditionSatisfied = torqueReductionTriggerPinState
        ? !engineConfiguration->limitTorqueReductionTime ||
            ((0.0f < torqueReductionTime)
                && !m_pinTriggeredTimer.hasElapsedMs(torqueReductionTime)
            )
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

float ShiftTorqueReductionController::getTorqueReductionIgnitionRetard() {
    auto torqueReductionXAxis = readGppwmChannel(config->torqueReductionIgnitionRetardXaxis);
    trqRedIgnRetXaxisValue = torqueReductionXAxis.Value;
    int8_t currentGear = Sensor::getOrZero(SensorType::DetectedGear);

    return interpolate3d(
        config->torqueReductionIgnitionRetardTable,
        config->torqueReductionIgnitionRetardGearBins, currentGear,
        config->torqueReductionIgnitionRetardXBins, torqueReductionXAxis.Value
    );
}

#endif // EFI_LAUNCH_CONTROL
