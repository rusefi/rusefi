//
// Created by kifir on 9/30/24.
//

#include "pch.h"

#include "shift_torque_reduction_test_base.h"

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

ShiftTorqueReductionTestConfig ShiftTorqueReductionTestConfig::setTriggerPin(
    const std::optional<switch_input_pin_e> value
) {
    m_torqueReductionTriggerPin = value;
    return *this;
}

ShiftTorqueReductionTestConfig ShiftTorqueReductionTestConfig::setPinInverted(const std::optional<bool> value) {
    m_pinInverted = value;
    return *this;
}

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

void ShiftTorqueReductionTestBase::setUpTestConfig(const ShiftTorqueReductionTestConfig& config) {
    getTestEngineConfiguration().configureTorqueReductionEnabled(config.getTorqueReductionEnabled());
    getTestEngineConfiguration().configureTorqueReductionActivationMode(config.getTorqueReductionActivationMode());
    getTestEngineConfiguration().configureTorqueReductionButton(config.getTriggerPin());
    getTestEngineConfiguration().configureTorqueReductionButtonInverted(config.getPinInverted());
    getTestEngineConfiguration().configureLaunchActivatePin(config.getLaunchActivatePin());
    getTestEngineConfiguration().configureLaunchActivateInverted(config.getLaunchActivateInverted());
    getTestEngineConfiguration().configureLimitTorqueReductionTime(config.getLimitTorqueReductionTime());
    getTestEngineConfiguration().configureTorqueReductionTime(config.getTorqueReductionTime());
    getTestEngineConfiguration().configureTorqueReductionArmingRpm(config.getTorqueReductionArmingRpm());
    getTestEngineConfiguration().configureTorqueReductionArmingApp(config.getTorqueReductionArmingApp());
}
