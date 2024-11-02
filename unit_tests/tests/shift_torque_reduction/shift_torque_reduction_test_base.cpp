//
// Created by kifir on 9/30/24.
//

#include "pch.h"

#include "shift_torque_reduction_test_base.h"
#include "engine_configuration_defaults.h"

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
    configureTorqueReductionEnabled(config.getTorqueReductionEnabled());
    configureTorqueReductionActivationMode(config.getTorqueReductionActivationMode());
    configureTorqueReductionButton(config.getTriggerPin());
    configureTorqueReductionButtonInverted(config.getPinInverted());
    configureLaunchActivatePin(config.getLaunchActivatePin());
    configureLaunchActivateInverted(config.getLaunchActivateInverted());
    configureLimitTorqueReductionTime(config.getLimitTorqueReductionTime());
    configureTorqueReductionTime(config.getTorqueReductionTime());
    configureTorqueReductionArmingRpm(config.getTorqueReductionArmingRpm());
    configureTorqueReductionArmingApp(config.getTorqueReductionArmingApp());
}

void ShiftTorqueReductionTestBase::configureTorqueReductionEnabled(const std::optional<bool> torqueReductionEnabled) {
    if (torqueReductionEnabled.has_value()) {
        engineConfiguration->torqueReductionEnabled = torqueReductionEnabled.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionEnabled,
            engine_configuration_defaults::ENABLE_SHIFT_TORQUE_REDUCTION
        ); // check default value
    }
}

void ShiftTorqueReductionTestBase::configureTorqueReductionActivationMode(
    const std::optional<torqueReductionActivationMode_e> activationMode
) {
    if (activationMode.has_value()) {
        engineConfiguration->torqueReductionActivationMode = activationMode.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionActivationMode,
            engine_configuration_defaults::TORQUE_REDUCTION_ACTIVATION_MODE
        ); // check default value
    }
}

void ShiftTorqueReductionTestBase::configureTorqueReductionButton(const std::optional<switch_input_pin_e> pin) {
    if (pin.has_value()) {
        engineConfiguration->torqueReductionTriggerPin = pin.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionTriggerPin,
            engine_configuration_defaults::TORQUE_REDUCTION_TRIGGER_PIN
        ); // check default value
    }
}

void ShiftTorqueReductionTestBase::configureTorqueReductionButtonInverted(const std::optional<bool> pinInverted) {
    if (pinInverted.has_value()) {
        engineConfiguration->torqueReductionTriggerPinInverted = pinInverted.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionTriggerPinInverted,
            engine_configuration_defaults::TORQUE_REDUCTION_TRIGGER_PIN_INVERTED
        ); // check default value
    }
}

void ShiftTorqueReductionTestBase::configureLaunchActivatePin(const std::optional<switch_input_pin_e> pin) {
    if (pin.has_value()) {
        engineConfiguration->launchActivatePin = pin.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->launchActivatePin,
            engine_configuration_defaults::LAUNCH_ACTIVATE_PIN
        ); // check default value
    }
}

void ShiftTorqueReductionTestBase::configureLaunchActivateInverted(const std::optional<bool> pinInverted) {
    if (pinInverted.has_value()) {
        engineConfiguration->launchActivateInverted = pinInverted.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->launchActivateInverted,
            engine_configuration_defaults::LAUNCH_ACTIVATE_PIN_INVERTED
        ); // check default value
    }
}

void ShiftTorqueReductionTestBase::configureLimitTorqueReductionTime(std::optional<bool> limitTorqueReductionTime) {
    if (limitTorqueReductionTime.has_value()) {
        engineConfiguration->limitTorqueReductionTime = limitTorqueReductionTime.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->limitTorqueReductionTime,
            engine_configuration_defaults::LIMIT_TORQUE_REDUCTION_TIME
        ); // check default value
    }
}

void ShiftTorqueReductionTestBase::configureTorqueReductionTime(std::optional<float> timeout) {
    if (timeout.has_value()) {
        engineConfiguration->torqueReductionTime = timeout.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionTime,
            engine_configuration_defaults::TORQUE_REDUCTION_TIME
        ); // check default value
    }
}

void ShiftTorqueReductionTestBase::configureTorqueReductionArmingRpm(const std::optional<float> armingRpm) {
    if (armingRpm.has_value()) {
        engineConfiguration->torqueReductionArmingRpm = armingRpm.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionArmingRpm,
            engine_configuration_defaults::TORQUE_REDUCTION_ARMING_RPM
        ); // check default value
    }
}

void ShiftTorqueReductionTestBase::configureTorqueReductionArmingApp(const std::optional<float> armingApp) {
    if (armingApp.has_value()) {
        engineConfiguration->torqueReductionArmingApp = armingApp.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionArmingApp,
            engine_configuration_defaults::TORQUE_REDUCTION_ARMING_APP
        ); // check default value
    }
}