//
// Created by kifir on 11/4/24.
//

#pragma once

#include "pch.h"

class TestEngineConfiguration {
public:
    static TestEngineConfiguration& getInstance();

    void configureTorqueReductionEnabled(std::optional<bool> isTorqueReductionEnabled);
    void configureTorqueReductionActivationMode(std::optional<torqueReductionActivationMode_e> activationMode);
    void configureTorqueReductionButton(std::optional<switch_input_pin_e> pin);
    void configureTorqueReductionButtonInverted(std::optional<bool> pinInverted);
    void configureLaunchActivatePin(std::optional<switch_input_pin_e> pin);
    void configureLaunchActivateInverted(std::optional<bool> pinInverted);
    void configureLimitTorqueReductionTime(std::optional<bool> limitTorqueReductionTime);
    void configureTorqueReductionTime(std::optional<float> timeout);
    void configureTorqueReductionArmingRpm(std::optional<float> armingRpm);
    void configureTorqueReductionArmingApp(std::optional<float> armingApp);
private:
    TestEngineConfiguration();
    static TestEngineConfiguration instance;
};
