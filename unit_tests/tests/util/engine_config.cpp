//
// Created by kifir on 11/18/24.
//

#include "pch.h"

#include "engine_config.h"

EngineConfig EngineConfig::clone() const {
    return *this;
}

EngineConfig EngineConfig::setClutchDownPin(const std::optional<switch_input_pin_e> value) {
    m_clutchDownPin = value;
    return *this;
}

EngineConfig EngineConfig::setClutchDownPinMode(const std::optional<pin_input_mode_e> value) {
    m_clutchDownPinMode = value;
    return *this;
}

EngineConfig EngineConfig::setClutchUpPin(const std::optional<switch_input_pin_e> value) {
    m_clutchUpPin = value;
    return *this;
}

EngineConfig EngineConfig::setClutchUpPinMode(const std::optional<pin_input_mode_e> value) {
    m_clutchUpPinMode = value;
    return *this;
}

EngineConfig EngineConfig::setLaunchActivatePin(const std::optional<switch_input_pin_e> value) {
    m_launchActivatePin = value;
    return *this;
}

EngineConfig EngineConfig::setLaunchActivatePinMode(const std::optional<pin_input_mode_e> value) {
    m_launchActivatePinMode = value;
    return *this;
}

EngineConfig EngineConfig::setLaunchControlEnabled(const std::optional<bool> value) {
    m_launchControlEnabled = value;
    return *this;
}

EngineConfig EngineConfig::setLaunchRpm(const std::optional<int> value) {
    m_launchRpm = value;
    return *this;
}

EngineConfig EngineConfig::setLaunchRpmWindow(const std::optional<int> value) {
    m_launchRpmWindow = value;
    return *this;
}

EngineConfig EngineConfig::setLaunchCorrectionsEndRpm(const std::optional<int> value) {
    m_launchCorrectionsEndRpm = value;
    return *this;
}

EngineConfig EngineConfig::setIgnitionRetardEnable(const std::optional<bool> value) {
    m_ignitionRetardEnable = value;
    return *this;
}

EngineConfig EngineConfig::setIgnitionRetard(const std::optional<float> value) {
    m_ignitionRetard = value;
    return *this;
}

EngineConfig EngineConfig::setSmoothRetardMode(const std::optional<bool> value) {
    m_smoothRetardMode = value;
    return *this;
}

EngineConfig EngineConfig::setEnableIgnitionCut(const std::optional<bool> value) {
    m_enableIgnitionCut = value;
    return *this;
}

EngineConfig EngineConfig::setInitialIgnitionCut(const std::optional<int> value) {
    m_initialIgnitionCut = value;
    return *this;
}

EngineConfig EngineConfig::setFinalIgnitionCutBeforeLaunch(const std::optional<int> value) {
    m_finalIgnitionCutBeforeLaunch = value;
    return *this;
}

EngineConfig EngineConfig::setTorqueReductionEnabled(const std::optional<bool> value) {
    m_isTorqueReductionEnabled = value;
    return *this;
}

EngineConfig EngineConfig::setTorqueReductionActivationMode(
    const std::optional<torqueReductionActivationMode_e> value
) {
    m_torqueReductionActivationMode = value;
    return *this;
}

EngineConfig EngineConfig::setTorqueReductionTriggerPin(const std::optional<switch_input_pin_e> value) {
    m_torqueReductionTriggerPin = value;
    return *this;
}

EngineConfig EngineConfig::setTorqueReductionTriggerPinMode(const std::optional<pin_input_mode_e> value) {
    m_torqueReductionTriggerPinMode = value;
    return *this;
}

EngineConfig EngineConfig::setLimitTorqueReductionTime(std::optional<bool> value) {
    m_limitTorqueReductionTime = value;
    return *this;
}

EngineConfig EngineConfig::setTorqueReductionTime(const std::optional<float> value) {
    m_torqueReductionTime = value;
    return *this;
}

EngineConfig EngineConfig::setTorqueReductionArmingRpm(std::optional<float> value) {
    m_torqueReductionArmingRpm = value;
    return *this;
}

EngineConfig EngineConfig::setTorqueReductionArmingApp(std::optional<float> value) {
    m_torqueReductionArmingApp = value;
    return *this;
}

EngineConfig EngineConfig::setTorqueReductionIgnitionCut(const std::optional<int8_t> value) {
    m_torqueReductionIgnitionCut = value;
    return *this;
}

EngineConfig EngineConfig::setTorqueReductionIgnitionRetard(const std::optional<float> value) {
    m_torqueReductionIgnitionRetard = value;
    return *this;
}

EngineConfig EngineConfig::setFuelPressureSensorMode(std::optional<fuel_pressure_sensor_mode_e> value) {
    m_fuelPressureSensorMode = value;
    return *this;
}

EngineConfig EngineConfig::setInjectorFlowAsMassFlow(const std::optional<bool> injectorFlowAsMassFlow) {
    m_injectorFlowAsMassFlow = injectorFlowAsMassFlow;
    return *this;
}

EngineConfig EngineConfig::setInjectorFlow(const std::optional<float> flow) {
    m_injectorFlow = flow;
    return *this;
}

EngineConfig EngineConfig::setInjectorBattLagCorr(const std::optional<BattLagCorrTable> battLagCorr) {
    m_injectorBattLagCorrCurve = battLagCorr;
    return *this;
}

EngineConfig EngineConfig::setFuelReferencePressure(const std::optional<float> value) {
    m_fuelReferencePressure = value;
    return *this;
}

EngineConfig EngineConfig::setInjectorCompensationMode(const std::optional<injector_compensation_mode_e> value) {
    m_injectorCompensationMode = value;
    return *this;
}

EngineConfig EngineConfig::setInjectorSecondaryFlow(const std::optional<float> flow) {
    m_injectorSecondaryFlow = flow;
    return *this;
}

EngineConfig EngineConfig::setInjectorSecondaryBattLagCorr(const std::optional<BattLagCorrTable> battLagCorr) {
    m_injectorSecondaryBattLagCorrCurve = battLagCorr;
    return *this;
}

EngineConfig EngineConfig::setStagedInjectionEnabled(const std::optional<bool> value) {
    m_isStagedInjectionEnabled = value;
    return *this;
}

EngineConfig EngineConfig::setSecondaryInjectorFuelReferencePressure(const std::optional<float> value) {
    m_secondaryInjectorFuelReferencePressure = value;
    return *this;
}

EngineConfig EngineConfig::setSecondaryInjectorCompensationMode(
    const std::optional<injector_compensation_mode_e> value
) {
    m_secondaryInjectorCompensationMode = value;
    return *this;
}

EngineConfig EngineConfig::setNitrousControlEnabled(const std::optional<bool> value) {
    m_nitrousControlEnabled = value;
    return *this;
}

EngineConfig EngineConfig::setNitrousControlArmingMethod(const std::optional<nitrous_arming_method_e> value) {
    m_nitrousControlArmingMethod = value;
    return *this;
}

EngineConfig EngineConfig::setNitrousControlTriggerPin(const std::optional<switch_input_pin_e> value) {
    m_nitrousControlTriggerPin = value;
    return *this;
}

EngineConfig EngineConfig::setNitrousControlTriggerPinMode(const std::optional<pin_input_mode_e> value) {
    m_nitrousControlTriggerPinMode = value;
    return *this;
}

EngineConfig EngineConfig::setNitrousLuaGauge(const std::optional<lua_gauge_e> value) {
    m_nitrousLuaGauge = value;
    return *this;
}

EngineConfig EngineConfig::setNitrousLuaGaugeMeaning(const std::optional<lua_gauge_meaning_e> value) {
    m_nitrousLuaGaugeMeaning = value;
    return *this;
}

EngineConfig EngineConfig::setNitrousLuaGaugeArmingValue(const std::optional<float> value) {
    m_nitrousLuaGaugeArmingValue = value;
    return *this;
}

EngineConfig EngineConfig::setNitrousMinimumVehicleSpeed(const std::optional<uint16_t> value) {
    m_nitrousMinimumVehicleSpeed = value;
    return *this;
}

EngineConfig EngineConfig::setNitrousMinimumTps(const std::optional<int> value) {
    m_nitrousMinimumTps = value;
    return *this;
}

EngineConfig EngineConfig::setNitrousMinimumClt(const std::optional<uint8_t> value) {
    m_nitrousMinimumClt = value;
    return *this;
}

EngineConfig EngineConfig::setNitrousMaximumMap(const std::optional<uint16_t> value) {
    m_nitrousMaximumMap = value;
    return *this;
}

EngineConfig EngineConfig::setNitrousMaximumAfr(const std::optional<float> value) {
    m_nitrousMaximumAfr = value;
    return *this;
}

EngineConfig EngineConfig::setNitrousActivationRpm(const std::optional<uint16_t> value) {
    m_nitrousActivationRpm = value;
    return *this;
}

EngineConfig EngineConfig::setNitrousDeactivationRpm(const std::optional<uint16_t> value) {
    m_nitrousDeactivationRpm = value;
    return *this;
}

EngineConfig EngineConfig::setNitrousDeactivationRpmWindow(const std::optional<uint16_t> value) {
    m_nitrousDeactivationRpmWindow = value;
    return *this;
}

EngineConfig EngineConfig::setNitrousFuelAdderPercent(const std::optional<int8_t> value) {
    m_nitrousFuelAdderPercent = value;
    return *this;
}

EngineConfig EngineConfig::setNitrousIgnitionRetard(const std::optional<float> value) {
    m_nitrousIgnitionRetard = value;
    return *this;
}

EngineConfig EngineConfig::setFuelLevelAveragingAlpha(const std::optional<float> value) {
    m_fuelLevelAveragingAlpha = value;
    return *this;
}

EngineConfig EngineConfig::setFuelLevelUpdatePeriodSec(const std::optional<float> value) {
    m_fuelLevelUpdatePeriodSec = value;
    return *this;
}

EngineConfig EngineConfig::setFuelLevelLowThresholdVoltage(const std::optional<float> value) {
    m_fuelLevelLowThresholdVoltage = value;
    return *this;
}

EngineConfig EngineConfig::setFuelLevelHighThresholdVoltage(const std::optional<float> value) {
    m_fuelLevelHighThresholdVoltage = value;
    return *this;
}

