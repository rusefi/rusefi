//
// Created by kifir on 8/23/24.
//

#pragma once

#include "batt_lag_corr_curve.h"

namespace engine_configuration_defaults {
    /* A/C Settings: */
    constexpr float AC_DELAY = 0.5f;
    constexpr uint16_t MIN_AC_PRESSURE = 100;
    constexpr uint16_t MAX_AC_PRESSURE = 300;
    constexpr float AC_PRESSURE_ENABLE_HYST = 10.0f;

    /* Shift Torque Reduction (Flat Shift): */
    constexpr bool ENABLE_SHIFT_TORQUE_REDUCTION = false;
    constexpr torqueReductionActivationMode_e TORQUE_REDUCTION_ACTIVATION_MODE = TORQUE_REDUCTION_BUTTON;
    constexpr switch_input_pin_e TORQUE_REDUCTION_TRIGGER_PIN = Gpio::Unassigned;
    constexpr bool TORQUE_REDUCTION_TRIGGER_PIN_INVERTED = false;
    constexpr float TORQUE_REDUCTION_TIME = 0.0f;
    constexpr bool LIMIT_TORQUE_REDUCTION_TIME = false;
    constexpr float TORQUE_REDUCTION_ARMING_RPM = 0.0f;
    constexpr float TORQUE_REDUCTION_ARMING_APP = 0.0f;
    constexpr int8_t TORQUE_REDUCTION_IGNITION_CUT = 0;
    constexpr float TORQUE_REDUCTION_IGNITION_RETARD = 0.0f;

    /* Launch Control: */
    constexpr switch_input_pin_e LAUNCH_ACTIVATE_PIN = Gpio::Unassigned;
    constexpr bool LAUNCH_ACTIVATE_PIN_INVERTED = false;
    constexpr launchActivationMode_e LAUNCH_ACTIVATION_MODE = SWITCH_INPUT_LAUNCH;
    constexpr int LAUNCH_SPEED_THRESHOLD = 0;

    /* Injector */
    constexpr bool INJECTOR_FLOW_AS_MASS_FLOW = false;
    constexpr float INJECTOR_FLOW = 200.0f;
    constexpr BattLagCorrCurve INJECTOR_BATT_LAG_CURR { 3.371f, 1.974f, 1.383f, 1.194f, 1.04f, 0.914f, 0.797f, 0.726 };

    /* Secondary injector: */
    constexpr float INJECTOR_SECONDARY_FLOW = INJECTOR_FLOW;
    constexpr BattLagCorrCurve INJECTOR_SECONDARY_BATT_LAG_CURR = INJECTOR_BATT_LAG_CURR;

    /* Staged injection: */
    constexpr bool ENABLE_STAGED_INJECTION = false;
}
