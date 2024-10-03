//
// Created by kifir on 8/23/24.
//

#pragma once

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
    constexpr float TORQUE_REDUCTION_ARMING_RPM = 0.0f;
    constexpr float TORQUE_REDUCTION_ARMING_APP = 0.0f;

    /* Launch Control: */
    constexpr switch_input_pin_e LAUNCH_ACTIVATE_PIN = Gpio::Unassigned;
    constexpr bool LAUNCH_ACTIVATE_PIN_INVERTED = false;
}
