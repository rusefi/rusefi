//
// Created by kifir on 11/4/24.
//

#pragma once

#include "batt_lag_corr_curve.h"

class TestEngineConfiguration {
public:
    static TestEngineConfiguration& getInstance();

    // Launch Control Settings
    void configureLaunchControlEnabled(std::optional<bool> launchControlEnabled);

    void configureLaunchActivationMode(std::optional<launchActivationMode_e> launchActivationMode);
    void configureLaunchSpeedThreshold(std::optional<int> launchSpeedThreshold);

    void configureLaunchRpm(std::optional<int> launchRpm);
    void configureLaunchRpmWindow(std::optional<int> launchRpmWindow);
    void configureLaunchCorrectionsEndRpm(std::optional<int> launchCorrectionsEndRpm);

    void configureIgnitionRetardEnable(std::optional<bool> ignitionRetardEnable);
    void configureIgnitionRetard(std::optional<float> ignitionRetard);
    void configureSmoothRetardMode(std::optional<bool> smoothRetardMode);

    void configureEnableIgnitionCut(std::optional<bool> enableIgnitionCut);
    void configureInitialIgnitionCutPercent(std::optional<int> initialIgnitionCut);
    void configureFinalIgnitionCutPercentBeforeLaunch(std::optional<int> finalIgnitionCutBeforeLaunch);

    // Shift Torque Reduction (Flat Shift)
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
    void configureTorqueReductionIgnitionCut(std::optional<int8_t> ignitionCut);
    void configureTorqueReductionIgnitionRetard(std::optional<int8_t> ignitionRetard);

    // Injector
    void configureInjectorFlow(std::optional<float> flow);
    void configureInjectorBattLagCorr(std::optional<BattLagCorrCurve> battLagCorr);
    void configureInjectorFlowAsMassFlow(std::optional<bool> injectorFlowAsMassFlow);

    // Secondary Injector
    void configureInjectorSecondaryFlow(std::optional<float> flow);
    void configureInjectorSecondaryBattLagCorr(std::optional<BattLagCorrCurve> battLagCorr);

    // Staged injection
    void configureEnableStagedInjection(std::optional<bool> isStagedInjectionEnabled);
private:
    TestEngineConfiguration();
    static TestEngineConfiguration instance;
};
