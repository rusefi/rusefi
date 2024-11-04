//
// Created by kifir on 11/4/24.
//

#include "pch.h"

#include "test_engine_configuration.h"

#include "engine_configuration_defaults.h"

TestEngineConfiguration& TestEngineConfiguration::getInstance() {
    return instance;
}

void TestEngineConfiguration::configureTorqueReductionEnabled(const std::optional<bool> torqueReductionEnabled) {
    if (torqueReductionEnabled.has_value()) {
        engineConfiguration->torqueReductionEnabled = torqueReductionEnabled.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionEnabled,
            engine_configuration_defaults::ENABLE_SHIFT_TORQUE_REDUCTION
        ); // check default value
    }
}

void TestEngineConfiguration::configureTorqueReductionActivationMode(
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

void TestEngineConfiguration::configureTorqueReductionButton(const std::optional<switch_input_pin_e> pin) {
    if (pin.has_value()) {
        engineConfiguration->torqueReductionTriggerPin = pin.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionTriggerPin,
            engine_configuration_defaults::TORQUE_REDUCTION_TRIGGER_PIN
        ); // check default value
    }
}

void TestEngineConfiguration::configureTorqueReductionButtonInverted(const std::optional<bool> pinInverted) {
    if (pinInverted.has_value()) {
        engineConfiguration->torqueReductionTriggerPinInverted = pinInverted.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionTriggerPinInverted,
            engine_configuration_defaults::TORQUE_REDUCTION_TRIGGER_PIN_INVERTED
        ); // check default value
    }
}

void TestEngineConfiguration::configureLaunchActivatePin(const std::optional<switch_input_pin_e> pin) {
    if (pin.has_value()) {
        engineConfiguration->launchActivatePin = pin.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->launchActivatePin,
            engine_configuration_defaults::LAUNCH_ACTIVATE_PIN
        ); // check default value
    }
}

void TestEngineConfiguration::configureLaunchActivateInverted(const std::optional<bool> pinInverted) {
    if (pinInverted.has_value()) {
        engineConfiguration->launchActivateInverted = pinInverted.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->launchActivateInverted,
            engine_configuration_defaults::LAUNCH_ACTIVATE_PIN_INVERTED
        ); // check default value
    }
}

void TestEngineConfiguration::configureLimitTorqueReductionTime(std::optional<bool> limitTorqueReductionTime) {
    if (limitTorqueReductionTime.has_value()) {
        engineConfiguration->limitTorqueReductionTime = limitTorqueReductionTime.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->limitTorqueReductionTime,
            engine_configuration_defaults::LIMIT_TORQUE_REDUCTION_TIME
        ); // check default value
    }
}

void TestEngineConfiguration::configureTorqueReductionTime(std::optional<float> timeout) {
    if (timeout.has_value()) {
        engineConfiguration->torqueReductionTime = timeout.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionTime,
            engine_configuration_defaults::TORQUE_REDUCTION_TIME
        ); // check default value
    }
}

void TestEngineConfiguration::configureTorqueReductionArmingRpm(const std::optional<float> armingRpm) {
    if (armingRpm.has_value()) {
        engineConfiguration->torqueReductionArmingRpm = armingRpm.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionArmingRpm,
            engine_configuration_defaults::TORQUE_REDUCTION_ARMING_RPM
        ); // check default value
    }
}

void TestEngineConfiguration::configureTorqueReductionArmingApp(const std::optional<float> armingApp) {
    if (armingApp.has_value()) {
        engineConfiguration->torqueReductionArmingApp = armingApp.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionArmingApp,
            engine_configuration_defaults::TORQUE_REDUCTION_ARMING_APP
        ); // check default value
    }
}

TestEngineConfiguration::TestEngineConfiguration() {
}

TestEngineConfiguration TestEngineConfiguration::instance;