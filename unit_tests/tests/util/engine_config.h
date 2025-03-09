//
// Created by kifir on 11/18/24.
//

#pragma once

#include "batt_lag_corr_curve.h"

class EngineConfig {
public:
    EngineConfig clone() const;

    // Other Sensor Inputs
    std::optional<switch_input_pin_e> getClutchDownPin() const { return m_clutchDownPin; }
    std::optional<pin_input_mode_e> getClutchDownPinMode() const { return m_clutchDownPinMode; }
    std::optional<switch_input_pin_e> getClutchUpPin() const { return m_clutchUpPin; }
    std::optional<pin_input_mode_e> getClutchUpPinMode() const { return m_clutchUpPinMode; }

    // Launch Control
    std::optional<switch_input_pin_e> getLaunchActivatePin() const { return m_launchActivatePin; }
    std::optional<pin_input_mode_e> getLaunchActivatePinMode() const { return m_launchActivatePinMode; }
    std::optional<bool> getLaunchControlEnabled() const { return m_launchControlEnabled; }

    std::optional<int> getLaunchRpm() const { return m_launchRpm; }
    std::optional<int> getLaunchRpmWindow() const { return m_launchRpmWindow; }
    std::optional<int> getLaunchCorrectionsEndRpm() const { return m_launchCorrectionsEndRpm; }

    std::optional<bool> getIgnitionRetardEnable() const { return m_ignitionRetardEnable; }
    std::optional<float> getIgnitionRetard() const { return m_ignitionRetard; }
    std::optional<bool> getSmoothRetardMode() const { return m_smoothRetardMode; }

    std::optional<bool> getEnableIgnitionCut() const { return m_enableIgnitionCut; }
    std::optional<int> getInitialIgnitionCut() const { return m_initialIgnitionCut; }
    std::optional<int> getFinalIgnitionCutBeforeLaunch() const { return m_finalIgnitionCutBeforeLaunch; }

    // Shift Torque Reduction (Flat Shift)
    std::optional<bool> getTorqueReductionEnabled() const { return m_isTorqueReductionEnabled; }
    std::optional<torqueReductionActivationMode_e> getTorqueReductionActivationMode() const {
        return m_torqueReductionActivationMode;
    }
    std::optional<switch_input_pin_e> getTorqueReductionTriggerPin() const { return m_torqueReductionTriggerPin; }
    std::optional<pin_input_mode_e> getTorqueReductionTriggerPinMode() const { return m_torqueReductionTriggerPinMode; }
    std::optional<bool> getLimitTorqueReductionTime() const { return m_limitTorqueReductionTime; }
    std::optional<float> getTorqueReductionTime() const { return m_torqueReductionTime; }
    std::optional<float> getTorqueReductionArmingRpm() const { return m_torqueReductionArmingRpm; }
    std::optional<float> getTorqueReductionArmingApp() const { return m_torqueReductionArmingApp; }
    std::optional<int8_t> getTorqueReductionIgnitionCut() const { return m_torqueReductionIgnitionCut; }
    std::optional<float> getTorqueReductionIgnitionRetard() const { return m_torqueReductionIgnitionRetard; }

    std::optional<fuel_pressure_sensor_mode_e> getFuelPressureSensorMode() const { return m_fuelPressureSensorMode; };

    // Injector
    std::optional<float> getInjectorFlow() const { return m_injectorFlow; }
    std::optional<BattLagCorrTable> getInjectorBattLagCorr() const { return m_injectorBattLagCorrCurve; }
    std::optional<bool> getInjectorFlowAsMassFlow() const { return m_injectorFlowAsMassFlow; }
    std::optional<float> getFuelReferencePressure() const { return m_fuelReferencePressure; }
    std::optional<injector_compensation_mode_e> getInjectorCompensationMode() const { return m_injectorCompensationMode; }

    // Secondary injector
    std::optional<float> getInjectorSecondaryFlow() const { return m_injectorSecondaryFlow; }
    std::optional<BattLagCorrTable> getInjectorSecondaryBattLagCorr() const { return m_injectorSecondaryBattLagCorrCurve; }
    std::optional<float> getSecondaryInjectorFuelReferencePressure() const { return m_secondaryInjectorFuelReferencePressure; }
    std::optional<injector_compensation_mode_e> getSecondaryInjectorCompensationMode() const { return m_secondaryInjectorCompensationMode; }

    // Staged injection
    std::optional<bool> getStagedInjectionEnabled() const { return m_isStagedInjectionEnabled; }

    // Nitrous control
    std::optional<bool> getNitrousControlEnabled() const { return m_nitrousControlEnabled; }
    std::optional<nitrous_arming_method_e> getNitrousControlArmingMethod() const { return m_nitrousControlArmingMethod; }
    std::optional<switch_input_pin_e> getNitrousControlTriggerPin() const { return m_nitrousControlTriggerPin; }
    std::optional<pin_input_mode_e> getNitrousControlTriggerPinMode() const { return m_nitrousControlTriggerPinMode; }
    std::optional<lua_gauge_e> getNitrousLuaGauge() const { return m_nitrousLuaGauge; }
    std::optional<lua_gauge_meaning_e> getNitrousLuaGaugeMeaning() const { return m_nitrousLuaGaugeMeaning; }
    std::optional<float> getNitrousLuaGaugeArmingValue() const { return m_nitrousLuaGaugeArmingValue; }

    std::optional<uint16_t> getNitrousMinimumVehicleSpeed() const { return m_nitrousMinimumVehicleSpeed; }
    std::optional<int> getNitrousMinimumTps() const { return  m_nitrousMinimumTps; }
    std::optional<uint8_t> getNitrousMinimumClt() const { return  m_nitrousMinimumClt; }
    std::optional<uint16_t> getNitrousMaximumMap() const { return  m_nitrousMaximumMap; }
    std::optional<float> getNitrousMaximumAfr() const { return  m_nitrousMaximumAfr; }
    std::optional<uint16_t> getNitrousActivationRpm() const { return  m_nitrousActivationRpm; }
    std::optional<uint16_t> getNitrousDeactivationRpm() const { return  m_nitrousDeactivationRpm; }
    std::optional<uint16_t> getNitrousDeactivationRpmWindow() const { return  m_nitrousDeactivationRpmWindow; }
    std::optional<int8_t> getNitrousFuelAdderPercent() const { return m_nitrousFuelAdderPercent; }
    std::optional<float> getNitrousIgnitionRetard() const { return m_nitrousIgnitionRetard; }

    std::optional<float> getFuelLevelAveragingAlpha() const { return m_fuelLevelAveragingAlpha; }
    std::optional<float> getFuelLevelUpdatePeriodSec() const { return m_fuelLevelUpdatePeriodSec; }
    std::optional<float> getFuelLevelLowThresholdVoltage() const { return m_fuelLevelLowThresholdVoltage; }
    std::optional<float> getFuelLevelHighThresholdVoltage() const { return  m_fuelLevelHighThresholdVoltage; }

    // We do not core about performance in tests, but we want to use builder-like style, so setters return new instance
    // of configuration:

    // Other Sensor Inputs
    EngineConfig setClutchDownPin(std::optional<switch_input_pin_e> value);
    EngineConfig setClutchDownPinMode(const std::optional<pin_input_mode_e> value);
    EngineConfig setClutchUpPin(std::optional<switch_input_pin_e> value);
    EngineConfig setClutchUpPinMode(std::optional<pin_input_mode_e> value);

    // Launch Control
    EngineConfig setLaunchActivatePin(std::optional<switch_input_pin_e> value);
    EngineConfig setLaunchActivatePinMode(const std::optional<pin_input_mode_e> value);
    EngineConfig setLaunchControlEnabled(std::optional<bool> value);
    EngineConfig setLaunchRpm(std::optional<int> value);
    EngineConfig setLaunchRpmWindow(std::optional<int> value);
    EngineConfig setLaunchCorrectionsEndRpm(std::optional<int> value);
    EngineConfig setIgnitionRetardEnable(std::optional<bool> value);
    EngineConfig setIgnitionRetard(std::optional<float> value);
    EngineConfig setSmoothRetardMode(std::optional<bool> value);
    EngineConfig setEnableIgnitionCut(std::optional<bool> value);
    EngineConfig setInitialIgnitionCut(std::optional<int> value);
    EngineConfig setFinalIgnitionCutBeforeLaunch(std::optional<int> value);

    // Shift Torque Reduction (Flat Shift)
    EngineConfig setTorqueReductionEnabled(std::optional<bool> value);
    EngineConfig setTorqueReductionActivationMode(std::optional<torqueReductionActivationMode_e> value);
    EngineConfig setTorqueReductionTriggerPin(std::optional<switch_input_pin_e> value);
    EngineConfig setTorqueReductionTriggerPinMode(std::optional<pin_input_mode_e> value);
    EngineConfig setLimitTorqueReductionTime(std::optional<bool> value);
    EngineConfig setTorqueReductionTime(std::optional<float> value);
    EngineConfig setTorqueReductionArmingRpm(std::optional<float> value);
    EngineConfig setTorqueReductionArmingApp(std::optional<float> value);
    EngineConfig setTorqueReductionIgnitionCut(std::optional<int8_t> value);
    EngineConfig setTorqueReductionIgnitionRetard(std::optional<float> value);

    EngineConfig setFuelPressureSensorMode(std::optional<fuel_pressure_sensor_mode_e> value);

    // Injector
    EngineConfig setInjectorFlowAsMassFlow(std::optional<bool> injectorFlowAsMassFlow);
    EngineConfig setInjectorFlow(std::optional<float> flow);
    EngineConfig setInjectorBattLagCorr(std::optional<BattLagCorrTable> battLagCorr);
    EngineConfig setFuelReferencePressure(std::optional<float> value);
    EngineConfig setInjectorCompensationMode(std::optional<injector_compensation_mode_e> value);

    // Secondary injector
    EngineConfig setInjectorSecondaryFlow(std::optional<float> flow);
    EngineConfig setInjectorSecondaryBattLagCorr(std::optional<BattLagCorrTable> battLagCorr);
    EngineConfig setSecondaryInjectorFuelReferencePressure(std::optional<float> value);
    EngineConfig setSecondaryInjectorCompensationMode(std::optional<injector_compensation_mode_e> value);

    // Staged injection
    EngineConfig setStagedInjectionEnabled(std::optional<bool> value);

    // Nitrous control
    EngineConfig setNitrousControlEnabled(std::optional<bool> value);
    EngineConfig setNitrousControlArmingMethod(std::optional<nitrous_arming_method_e> value);
    EngineConfig setNitrousControlTriggerPin(std::optional<switch_input_pin_e> value);
    EngineConfig setNitrousControlTriggerPinMode(std::optional<pin_input_mode_e> value);
    EngineConfig setNitrousLuaGauge(std::optional<lua_gauge_e> value);
    EngineConfig setNitrousLuaGaugeMeaning(std::optional<lua_gauge_meaning_e> value);
    EngineConfig setNitrousLuaGaugeArmingValue(std::optional<float> value);

    EngineConfig setNitrousMinimumVehicleSpeed(std::optional<uint16_t> value);
    EngineConfig setNitrousMinimumTps(std::optional<int> value);
    EngineConfig setNitrousMinimumClt(std::optional<uint8_t> value);
    EngineConfig setNitrousMaximumMap(std::optional<uint16_t> value);
    EngineConfig setNitrousMaximumAfr(std::optional<float> value);
    EngineConfig setNitrousActivationRpm(std::optional<uint16_t> value);
    EngineConfig setNitrousDeactivationRpm(std::optional<uint16_t> value);
    EngineConfig setNitrousDeactivationRpmWindow(std::optional<uint16_t> value);
    EngineConfig setNitrousFuelAdderPercent(std::optional<int8_t> value);
    EngineConfig setNitrousIgnitionRetard(std::optional<float> value);

    EngineConfig setFuelLevelAveragingAlpha(std::optional<float> value);
    EngineConfig setFuelLevelUpdatePeriodSec(std::optional<float> value);
    EngineConfig setFuelLevelLowThresholdVoltage(std::optional<float> value);
    EngineConfig setFuelLevelHighThresholdVoltage(std::optional<float> value);
private:
    // Other Sensor Inputs
    std::optional<switch_input_pin_e> m_clutchDownPin;
    std::optional<pin_input_mode_e> m_clutchDownPinMode;
    std::optional<switch_input_pin_e> m_clutchUpPin;
    std::optional<pin_input_mode_e> m_clutchUpPinMode;

    // Launch Control
    std::optional<switch_input_pin_e> m_launchActivatePin;
    std::optional<pin_input_mode_e> m_launchActivatePinMode;
    std::optional<bool> m_launchControlEnabled;
    std::optional<int> m_launchRpm;
    std::optional<int> m_launchRpmWindow;
    std::optional<int> m_launchCorrectionsEndRpm;
    std::optional<bool> m_ignitionRetardEnable;
    std::optional<float> m_ignitionRetard;
    std::optional<bool> m_smoothRetardMode;
    std::optional<bool> m_enableIgnitionCut;
    std::optional<int> m_initialIgnitionCut;
    std::optional<int> m_finalIgnitionCutBeforeLaunch;

    // Shift Torque Reduction (Flat Shift)
    std::optional<bool> m_isTorqueReductionEnabled;
    std::optional<torqueReductionActivationMode_e> m_torqueReductionActivationMode;
    std::optional<switch_input_pin_e> m_torqueReductionTriggerPin;
    std::optional<pin_input_mode_e> m_torqueReductionTriggerPinMode;
    std::optional<bool> m_limitTorqueReductionTime;
    std::optional<float> m_torqueReductionTime;
    std::optional<float> m_torqueReductionArmingRpm;
    std::optional<float> m_torqueReductionArmingApp;
    std::optional<int8_t> m_torqueReductionIgnitionCut;
    std::optional<float> m_torqueReductionIgnitionRetard;

    std::optional<fuel_pressure_sensor_mode_e> m_fuelPressureSensorMode;

    // Injector
    std::optional<float> m_injectorFlow;
    std::optional<BattLagCorrTable> m_injectorBattLagCorrCurve;
    std::optional<bool> m_injectorFlowAsMassFlow;;
    std::optional<float> m_fuelReferencePressure;
    std::optional<injector_compensation_mode_e> m_injectorCompensationMode;

    // Secondary injector
    std::optional<float> m_injectorSecondaryFlow;
    std::optional<BattLagCorrTable> m_injectorSecondaryBattLagCorrCurve;
    std::optional<float> m_secondaryInjectorFuelReferencePressure;
    std::optional<injector_compensation_mode_e> m_secondaryInjectorCompensationMode;

    // Staged injection
    std::optional<bool> m_isStagedInjectionEnabled;

    // Nitrous control
    std::optional<bool> m_nitrousControlEnabled;
    std::optional<nitrous_arming_method_e> m_nitrousControlArmingMethod;
    std::optional<switch_input_pin_e> m_nitrousControlTriggerPin;
    std::optional<pin_input_mode_e> m_nitrousControlTriggerPinMode;
    std::optional<lua_gauge_e> m_nitrousLuaGauge;
    std::optional<lua_gauge_meaning_e> m_nitrousLuaGaugeMeaning;
    std::optional<float> m_nitrousLuaGaugeArmingValue;

    std::optional<uint16_t> m_nitrousMinimumVehicleSpeed;
    std::optional<int> m_nitrousMinimumTps;
    std::optional<uint8_t> m_nitrousMinimumClt;
    std::optional<uint16_t> m_nitrousMaximumMap;
    std::optional<float> m_nitrousMaximumAfr;
    std::optional<uint16_t> m_nitrousActivationRpm;
    std::optional<uint16_t> m_nitrousDeactivationRpm;
    std::optional<uint16_t> m_nitrousDeactivationRpmWindow;
    std::optional<int8_t> m_nitrousFuelAdderPercent;
    std::optional<float> m_nitrousIgnitionRetard;

    std::optional<float> m_fuelLevelAveragingAlpha;
    std::optional<float> m_fuelLevelUpdatePeriodSec;
    std::optional<float> m_fuelLevelLowThresholdVoltage;
    std::optional<float> m_fuelLevelHighThresholdVoltage;
};