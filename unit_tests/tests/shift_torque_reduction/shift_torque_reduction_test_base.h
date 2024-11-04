//
// Created by kifir on 9/30/24.
//

#pragma once

#include "util/test_base.h"

class ShiftTorqueReductionTestConfig {
public:
    std::optional<bool> getTorqueReductionEnabled() const { return m_isTorqueReductionEnabled; }
    std::optional<torqueReductionActivationMode_e> getTorqueReductionActivationMode() const {
        return m_torqueReductionActivationMode;
    }
    std::optional<switch_input_pin_e> getTriggerPin() const { return m_torqueReductionTriggerPin; }
    std::optional<bool> getPinInverted() const { return m_pinInverted; }
    std::optional<switch_input_pin_e> getLaunchActivatePin() const { return m_launchActivatePin; }
    std::optional<bool> getLaunchActivateInverted() const { return m_launchActivateInverted; }
    std::optional<bool> getLimitTorqueReductionTime() const { return m_limitTorqueReductionTime; }
    std::optional<float> getTorqueReductionTime() const { return m_torqueReductionTime; }
    std::optional<float> getTorqueReductionArmingRpm() const { return m_torqueReductionArmingRpm; }
    std::optional<float> getTorqueReductionArmingApp() const { return m_torqueReductionArmingApp; }
    std::optional<int8_t> getTorqueReductionIgnitionCut() const { return m_torqueReductionIgnitionCut; }

    // We do not core about performance in tests, but we want to use builder-like style, so setters return new instance
    // of configuration:
    ShiftTorqueReductionTestConfig setTorqueReductionEnabled(std::optional<bool> value);
    ShiftTorqueReductionTestConfig setTorqueReductionActivationMode(
        std::optional<torqueReductionActivationMode_e> value
    );
    ShiftTorqueReductionTestConfig setTriggerPin(std::optional<switch_input_pin_e> value);
    ShiftTorqueReductionTestConfig setPinInverted(std::optional<bool> value);
    ShiftTorqueReductionTestConfig setLaunchActivatePin(std::optional<switch_input_pin_e> value);
    ShiftTorqueReductionTestConfig setLaunchActivateInverted(std::optional<bool> value);
    ShiftTorqueReductionTestConfig setLimitTorqueReductionTime(std::optional<bool> value);
    ShiftTorqueReductionTestConfig setTorqueReductionTime(std::optional<float> value);
    ShiftTorqueReductionTestConfig setTorqueReductionArmingRpm(std::optional<float> value);
    ShiftTorqueReductionTestConfig setTorqueReductionArmingApp(std::optional<float> value);
    ShiftTorqueReductionTestConfig setTorqueReductionIgnitionCut(std::optional<int8_t> value);
private:
    std::optional<bool> m_isTorqueReductionEnabled;
    std::optional<torqueReductionActivationMode_e> m_torqueReductionActivationMode;
    std::optional<switch_input_pin_e> m_torqueReductionTriggerPin;
    std::optional<bool> m_pinInverted;
    std::optional<switch_input_pin_e> m_launchActivatePin;
    std::optional<bool> m_launchActivateInverted;
    std::optional<bool> m_limitTorqueReductionTime;
    std::optional<float> m_torqueReductionTime;
    std::optional<float> m_torqueReductionArmingRpm;
    std::optional<float> m_torqueReductionArmingApp;
    std::optional<int8_t> m_torqueReductionIgnitionCut;
};

class ShiftTorqueReductionTestBase : public TestBase {
protected:
    void setUpTestConfig(const ShiftTorqueReductionTestConfig& config);
};
