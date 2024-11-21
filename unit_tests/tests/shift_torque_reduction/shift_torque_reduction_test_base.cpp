//
// Created by kifir on 9/30/24.
//

#include "pch.h"

#include "shift_torque_reduction_test_base.h"

ShiftTorqueReductionTestConfig ShiftTorqueReductionTestConfig::setLaunchActivatePin(
    const std::optional<switch_input_pin_e> value
) {
    m_launchActivatePin = value;
    return *this;
}

ShiftTorqueReductionTestConfig ShiftTorqueReductionTestConfig::setLaunchActivateInverted(
    const std::optional<bool> value
) {
    m_launchActivateInverted = value;
    return *this;
}

ShiftTorqueReductionTestConfig ShiftTorqueReductionTestConfig::setTorqueReductionEnabled(
    const std::optional<bool> value
) {
    m_isTorqueReductionEnabled = value;
    return *this;
}

ShiftTorqueReductionTestConfig ShiftTorqueReductionTestConfig::setTorqueReductionActivationMode(
    const std::optional<torqueReductionActivationMode_e> value
) {
    m_torqueReductionActivationMode = value;
    return *this;
}

ShiftTorqueReductionTestConfig ShiftTorqueReductionTestConfig::setTorqueReductionTriggerPin(
    const std::optional<switch_input_pin_e> value
) {
    m_torqueReductionTriggerPin = value;
    return *this;
}

ShiftTorqueReductionTestConfig ShiftTorqueReductionTestConfig::setTorqueReductionTriggerPinInverted(
    const std::optional<bool> value
) {
    m_torqueReductionTriggerPinInverted = value;
    return *this;
}

ShiftTorqueReductionTestConfig ShiftTorqueReductionTestConfig::setLimitTorqueReductionTime(std::optional<bool> value) {
    m_limitTorqueReductionTime = value;
    return *this;
}

ShiftTorqueReductionTestConfig ShiftTorqueReductionTestConfig::setTorqueReductionTime(
    const std::optional<float> value
) {
    m_torqueReductionTime = value;
    return *this;
}

ShiftTorqueReductionTestConfig ShiftTorqueReductionTestConfig::setTorqueReductionArmingRpm(std::optional<float> value) {
    m_torqueReductionArmingRpm = value;
    return *this;
}

ShiftTorqueReductionTestConfig ShiftTorqueReductionTestConfig::setTorqueReductionArmingApp(std::optional<float> value) {
    m_torqueReductionArmingApp = value;
    return *this;
}

ShiftTorqueReductionTestConfig ShiftTorqueReductionTestConfig::setTorqueReductionIgnitionCut(
    const std::optional<int8_t> value
) {
    m_torqueReductionIgnitionCut = value;
    return *this;
}

ShiftTorqueReductionTestConfig ShiftTorqueReductionTestConfig::setTorqueReductionIgnitionRetard(
    const std::optional<float> value
) {
    m_torqueReductionIgnitionRetard = value;
    return *this;
}

void ShiftTorqueReductionTestBase::setUpTestConfig(const ShiftTorqueReductionTestConfig& config) {
	// Launch Control
    getTestEngineConfiguration().configureLaunchActivatePin(config.getLaunchActivatePin());
    getTestEngineConfiguration().configureLaunchActivateInverted(config.getLaunchActivateInverted());

	// Shift Torque Reduction (Flat Shift)
    getTestEngineConfiguration().configureTorqueReductionEnabled(config.getTorqueReductionEnabled());
    getTestEngineConfiguration().configureTorqueReductionActivationMode(config.getTorqueReductionActivationMode());
    getTestEngineConfiguration().configureTorqueReductionTriggerPin(config.getTorqueReductionTriggerPin());
    getTestEngineConfiguration().configureTorqueReductionButtonInverted(config.getTorqueReductionTriggerPinInverted());
    getTestEngineConfiguration().configureLimitTorqueReductionTime(config.getLimitTorqueReductionTime());
    getTestEngineConfiguration().configureTorqueReductionTime(config.getTorqueReductionTime());
    getTestEngineConfiguration().configureTorqueReductionArmingRpm(config.getTorqueReductionArmingRpm());
    getTestEngineConfiguration().configureTorqueReductionArmingApp(config.getTorqueReductionArmingApp());
    getTestEngineConfiguration().configureTorqueReductionIgnitionCut(config.getTorqueReductionIgnitionCut());
    getTestEngineConfiguration().configureTorqueReductionIgnitionRetard(config.getTorqueReductionIgnitionRetard());
}
