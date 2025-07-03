//
// Created by kifir on 8/23/24.
//

#pragma once
#include <array>
#include "arrays_util.h"
#include "batt_lag_corr_curve.h"

using torqueReductionCutTable = std::array<std::array<int8_t, TORQUE_TABLE_Y_SIZE>, TORQUE_TABLE_X_SIZE>;
using torqueReductionTable = std::array<std::array<float, TORQUE_TABLE_Y_SIZE>, TORQUE_TABLE_X_SIZE>;
using torqueReductionTimeTable = torqueReductionTable;

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
    constexpr pin_input_mode_e TORQUE_REDUCTION_TRIGGER_PIN_MODE = PI_DEFAULT;
    constexpr torqueReductionTimeTable TORQUE_REDUCTION_TIME = { { {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f} } };
    constexpr bool LIMIT_TORQUE_REDUCTION_TIME = false;
    constexpr float TORQUE_REDUCTION_ARMING_RPM = 0.0f;
    constexpr float TORQUE_REDUCTION_ARMING_APP = 0.0f;
    constexpr torqueReductionCutTable TORQUE_REDUCTION_IGNITION_CUT = { { { 0, 0 } } };
    constexpr torqueReductionTable TORQUE_REDUCTION_IGNITION_RETARD = { { {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f} } };

    /* Other Sensor Inputs: */
    constexpr switch_input_pin_e CLUTCH_DOWN_PIN = Gpio::Unassigned;
    constexpr pin_input_mode_e CLUTCH_DOWN_PIN_MODE = PI_PULLUP;
    constexpr switch_input_pin_e CLUTCH_UP_PIN = Gpio::Unassigned;
    constexpr pin_input_mode_e CLUTCH_UP_PIN_MODE = PI_PULLUP;

    /* Launch Control: */
    constexpr switch_input_pin_e LAUNCH_ACTIVATE_PIN = Gpio::Unassigned;
    constexpr pin_input_mode_e LAUNCH_ACTIVATE_PIN_MODE = PI_DEFAULT;
    constexpr launchActivationMode_e LAUNCH_ACTIVATION_MODE = SWITCH_INPUT_LAUNCH;
    constexpr int LAUNCH_SPEED_THRESHOLD = 0;

    constexpr fuel_pressure_sensor_mode_e FUEL_PRESSURE_SENSOR_MODE = FPM_Absolute;

    /* Injector */
    constexpr bool INJECTOR_FLOW_AS_MASS_FLOW = false;
    constexpr float INJECTOR_FLOW = 200.0f;

    // we use `initTableFromAnotherTable` template function to handle cases when `VBAT_INJECTOR_CURVE_SIZE` is not 8 or
    // `VBAT_INJECTOR_CURVE_PRESSURE_SIZE` is not 2
    constexpr BattLagCorrTable INJECTOR_BATT_LAG_CURR = initTableFromAnotherTable<
        float,
        8,
        2,
        VBAT_INJECTOR_CURVE_SIZE,
        VBAT_INJECTOR_CURVE_PRESSURE_SIZE
    >(
        { {
#if (VBAT_INJECTOR_CURVE_PRESSURE_SIZE == 2) && (VBAT_INJECTOR_CURVE_SIZE == 8)
            { 4.240f, 2.483f, 1.739f, 1.501f, 1.308f, 1.149f, 0.964f, 0.913f },
            { 3.084f, 1.641f, 1.149f, 1.194f, 0.992f, 0.759f, 0.637f, 0.603f },
#else //(VBAT_INJECTOR_CURVE_PRESSURE_SIZE == 2) && (VBAT_INJECTOR_CURVE_SIZE == 8)
            { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
#endif //(VBAT_INJECTOR_CURVE_PRESSURE_SIZE == 2) && (VBAT_INJECTOR_CURVE_SIZE == 8)
        } },
        0.0f
    );
    constexpr float FUEL_REFERENCE_PRESSURE = 300.0f;
    constexpr injector_compensation_mode_e INJECTOR_COMPENSATION_MODE = ICM_None;

    /* Secondary injector: */
    constexpr float INJECTOR_SECONDARY_FLOW = INJECTOR_FLOW;
    constexpr BattLagCorrTable INJECTOR_SECONDARY_BATT_LAG_CURR = INJECTOR_BATT_LAG_CURR;
    constexpr float SECONDARY_INJECTOR_FUEL_REFERENCE_PRESSURE = 0.0f;
    constexpr injector_compensation_mode_e SECONDARY_INJECTOR_COMPENSATION_MODE = INJECTOR_COMPENSATION_MODE;

    /* Staged injection: */
    constexpr bool ENABLE_STAGED_INJECTION = false;

    /* Nitrous control */
    constexpr bool NITROUS_CONTROL_ENABLED = false;
    constexpr nitrous_arming_method_e NITROUS_CONTROL_ARMING_METHOD = DIGITAL_SWITCH_INPUT;
    constexpr switch_input_pin_e NITROUS_CONTROL_TRIGGER_PIN = Gpio::Unassigned;
    constexpr pin_input_mode_e NITROUS_CONTROL_TRIGGER_PIN_MODE = PI_DEFAULT;
    constexpr lua_gauge_e NITROUS_LUA_GAUGE = LUA_GAUGE_1;
    constexpr lua_gauge_meaning_e NITROUS_LUA_GAUGE_MEANING = LUA_GAUGE_LOWER_BOUND;
    constexpr float NITROUS_LUA_GAUGE_ARMING_VALUE = 0.0f;

    constexpr uint16_t NITROUS_MINIMUM_VEHICLE_SPEED = 0;
    constexpr int NITROUS_MINIMUM_TPS = 80;
    constexpr uint8_t NITROUS_MINIMUM_CLT = 60;
    constexpr uint16_t NITROUS_MAXIMUM_MAP = 0;
    constexpr float NITROUS_MAXIMUM_AFR = 14.0f;
    constexpr uint16_t NITROUS_ACTIVATION_RPM = 4000;
    constexpr uint16_t NITROUS_DEACTIVATION_RPM = 7000;
    constexpr uint16_t NITROUS_DEACTIVATION_RPM_WINDOW = 500;
    constexpr int8_t NITROUS_FUEL_ADDER_PERCENT = 0;
    constexpr float NITROUS_IGNITION_RETARD = 0.0f;

    constexpr float FUEL_LEVEL_AVERAGING_ALPHA = 0.001f;
    constexpr float FUEL_LEVEL_UPDATE_PERIOD_SEC = 0.1f;
    constexpr float FUEL_LEVEL_LOW_THRESHOLD_VOLTAGE = 0.25f;
    constexpr float FUEL_LEVEL_HIGH_THRESHOLD_VOLTAGE = 4.5f;

    constexpr float DEFAULT_LAMBDA_LOAD_BINS[] = { 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 150, 175, 200, 225, 250 };
    constexpr float DEFAULT_LAMBDA_TABLE_ROW[] = {
        1,		1,		1,		1,		// 30, 40, 50, 60 kpa
        1,		0.95,	0.92,	0.90,	// 70, 80, 90, 100 kpa
        0.89,	0.88,	0.86,	0.84,	// 110, 120, 130, 150 kpa
        0.8,	0.77,	0.75,	0.73,	// 175, 200, 225, 250 kpa
    };
}
