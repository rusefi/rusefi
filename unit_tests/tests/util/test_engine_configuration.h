//
// Created by kifir on 11/4/24.
//

#pragma once

#include "batt_lag_corr_curve.h"

class TestEngineConfiguration {
public:
    static TestEngineConfiguration& getInstance();

    // Other Sensor Inputs
    void configureClutchDownPin(std::optional<switch_input_pin_e> pin);
    void configureClutchDownPinMode(const std::optional<pin_input_mode_e> pinMode);
    void configureClutchUpPin(std::optional<switch_input_pin_e> pin);
    void configureClutchUpPinMode(std::optional<pin_input_mode_e> pinMode);

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
    void configureTorqueReductionTriggerPin(std::optional<switch_input_pin_e> pin);
    void configureTorqueReductionButtonMode(std::optional<pin_input_mode_e> pinMode);
    void configureLaunchActivatePin(std::optional<switch_input_pin_e> pin);
    void configureLaunchActivateMode(std::optional<pin_input_mode_e> pinMode);
    void configureLimitTorqueReductionTime(std::optional<bool> limitTorqueReductionTime);
    void configureTorqueReductionTime(std::optional<float> timeout);
    void configureTorqueReductionArmingRpm(std::optional<float> armingRpm);
    void configureTorqueReductionArmingApp(std::optional<float> armingApp);
    void configureTorqueReductionIgnitionCut(std::optional<int8_t> ignitionCut);
    void configureTorqueReductionIgnitionRetard(std::optional<int8_t> ignitionRetard);

    void configureFuelPressureSensorMode(std::optional<fuel_pressure_sensor_mode_e> fuelPressureSensorMode);

    // Injector
    void configureInjectorFlow(std::optional<float> flow);
    void configureInjectorBattLagCorr(std::optional<BattLagCorrTable> battLagCorr);
    void configureInjectorFlowAsMassFlow(std::optional<bool> injectorFlowAsMassFlow);
    void configureFuelReferencePressure(std::optional<float> fuelReferencePressure);
    void configureInjectorCompensationMode(std::optional<injector_compensation_mode_e> injectorCompensationMode);

    // Secondary Injector
    void configureInjectorSecondaryFlow(std::optional<float> flow);
    void configureInjectorSecondaryBattLagCorr(std::optional<BattLagCorrTable> battLagCorr);
    void configureSecondaryInjectorFuelReferencePressure(std::optional<float> secondaryInjectorFuelReferencePressure);
    void configureSecondaryInjectorCompensationMode(
        std::optional<injector_compensation_mode_e> secondaryInjectorCompensationMode
    );

    // Staged injection
    void configureEnableStagedInjection(std::optional<bool> isStagedInjectionEnabled);

    // Nitrous control
    void configureNitrousControlEnabled(std::optional<bool> nitrousControlEnabled);
    void configureNitrousControlArmingMethod(std::optional<nitrous_arming_method_e> armingMethod);
    void configureNitrousControlTriggerPin(std::optional<switch_input_pin_e> triggerPin);
    void configureNitrousControlTriggerPinMode(std::optional<pin_input_mode_e> pinMode);
    void configureNitrousLuaGauge(std::optional<lua_gauge_e> luaGauge);
    void configureNitrousLuaGaugeMeaning(std::optional<lua_gauge_meaning_e> luaGaugeMeaning);
    void configureNitrousLuaGaugeArmingValue(std::optional<float> luaGaugeArmingValue);

    void configureNitrousMinimumVehicleSpeed(std::optional<uint16_t> nitrousMinimumVehicleSpeed);
    void configureNitrousMinimumTps(std::optional<int> nitrousMinimumTps);
    void configureNitrousMinimumClt(std::optional<uint8_t> nitrousMinimumClt);
    void configureNitrousMaximumMap(std::optional<uint16_t> nitrousMaximumMap);
    void configureNitrousMaximumAfr(std::optional<float> nitrousMaximumAfr);
    void configureNitrousActivationRpm(std::optional<uint16_t> nitrousActivationRpm);
    void configureNitrousDeactivationRpm(std::optional<uint16_t> nitrousDeactivationRpm);
    void configureNitrousDeactivationRpmWindow(std::optional<uint16_t> nitrousDeactivationRpmWindow);
    void configureNitrousFuelAdderPercent(std::optional<int8_t> nitrousFuelAdderPercent);
    void configureNitrousIgnitionRetard(std::optional<float> nitrousIgnitionRetard);

    void configureFuelLevelAveragingAlpha(std::optional<float> alpha);
    void configureFuelLevelUpdatePeriodSec(std::optional<float> seconds);
    void configureFuelLevelLowThresholdVoltage(std::optional<float> thresholdVoltage);
    void configureFuelLevelHighThresholdVoltage(std::optional<float> thresholdVoltage);
private:
    TestEngineConfiguration();
    static TestEngineConfiguration instance;
};
