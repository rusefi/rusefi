//
// Created by kifir on 11/18/24.
//

#pragma once

#include "batt_lag_corr_curve.h"

class EngineConfig {
public:
    EngineConfig clone() const;

    // Launch Control
    std::optional<switch_input_pin_e> getLaunchActivatePin() const { return m_launchActivatePin; }
    std::optional<bool> getLaunchActivateInverted() const { return m_launchActivateInverted; }
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
    std::optional<bool> getTorqueReductionTriggerPinInverted() const { return m_torqueReductionTriggerPinInverted; }
    std::optional<bool> getLimitTorqueReductionTime() const { return m_limitTorqueReductionTime; }
    std::optional<float> getTorqueReductionTime() const { return m_torqueReductionTime; }
    std::optional<float> getTorqueReductionArmingRpm() const { return m_torqueReductionArmingRpm; }
    std::optional<float> getTorqueReductionArmingApp() const { return m_torqueReductionArmingApp; }
    std::optional<int8_t> getTorqueReductionIgnitionCut() const { return m_torqueReductionIgnitionCut; }
    std::optional<float> getTorqueReductionIgnitionRetard() const { return m_torqueReductionIgnitionRetard; }

    std::optional<fuel_pressure_sensor_mode_e> getFuelPressureSensorMode() const { return m_fuelPressureSensorMode; };

    // Injector
    std::optional<float> getInjectorFlow() const { return m_injectorFlow; }
    std::optional<BattLagCorrCurve> getInjectorBattLagCorr() const { return m_injectorBattLagCorrCurve; }
    std::optional<bool> getInjectorFlowAsMassFlow() const { return m_injectorFlowAsMassFlow; }
    std::optional<float> getFuelReferencePressure() const { return m_fuelReferencePressure; }
    std::optional<injector_compensation_mode_e> getInjectorCompensationMode() const { return m_injectorCompensationMode; }

    // Secondary injector
    std::optional<float> getInjectorSecondaryFlow() const { return m_injectorSecondaryFlow; }
    std::optional<BattLagCorrCurve> getInjectorSecondaryBattLagCorr() const { return m_injectorSecondaryBattLagCorrCurve; }
    std::optional<float> getSecondaryInjectorFuelReferencePressure() const { return m_secondaryInjectorFuelReferencePressure; }
    std::optional<injector_compensation_mode_e> getSecondaryInjectorCompensationMode() const { return m_secondaryInjectorCompensationMode; }

    // Staged injection
    std::optional<bool> getStagedInjectionEnabled() const { return m_isStagedInjectionEnabled; }

    // We do not core about performance in tests, but we want to use builder-like style, so setters return new instance
    // of configuration:

    // Launch Control
    EngineConfig setLaunchActivatePin(std::optional<switch_input_pin_e> value);
    EngineConfig setLaunchActivateInverted(std::optional<bool> value);
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
    EngineConfig setTorqueReductionTriggerPinInverted(std::optional<bool> value);
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
    EngineConfig setInjectorBattLagCorr(std::optional<BattLagCorrCurve> battLagCorr);
    EngineConfig setFuelReferencePressure(std::optional<float> value);
    EngineConfig setInjectorCompensationMode(std::optional<injector_compensation_mode_e> value);

    // Secondary injector
    EngineConfig setInjectorSecondaryFlow(std::optional<float> flow);
    EngineConfig setInjectorSecondaryBattLagCorr(std::optional<BattLagCorrCurve> battLagCorr);
    EngineConfig setSecondaryInjectorFuelReferencePressure(std::optional<float> value);
    EngineConfig setSecondaryInjectorCompensationMode(std::optional<injector_compensation_mode_e> value);

    // Staged injection
    EngineConfig setStagedInjectionEnabled(std::optional<bool> value);
private:
    // Launch Control
    std::optional<switch_input_pin_e> m_launchActivatePin;
    std::optional<bool> m_launchActivateInverted;
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
    std::optional<bool> m_torqueReductionTriggerPinInverted;
    std::optional<bool> m_limitTorqueReductionTime;
    std::optional<float> m_torqueReductionTime;
    std::optional<float> m_torqueReductionArmingRpm;
    std::optional<float> m_torqueReductionArmingApp;
    std::optional<int8_t> m_torqueReductionIgnitionCut;
    std::optional<float> m_torqueReductionIgnitionRetard;

    std::optional<fuel_pressure_sensor_mode_e> m_fuelPressureSensorMode;

    // Injector
    std::optional<float> m_injectorFlow;
    std::optional<BattLagCorrCurve> m_injectorBattLagCorrCurve;
    std::optional<bool> m_injectorFlowAsMassFlow;;
    std::optional<float> m_fuelReferencePressure;
    std::optional<injector_compensation_mode_e> m_injectorCompensationMode;

    // Secondary injector
    std::optional<float> m_injectorSecondaryFlow;
    std::optional<BattLagCorrCurve> m_injectorSecondaryBattLagCorrCurve;
    std::optional<float> m_secondaryInjectorFuelReferencePressure;
    std::optional<injector_compensation_mode_e> m_secondaryInjectorCompensationMode;

    // Staged injection
    std::optional<bool> m_isStagedInjectionEnabled;
};