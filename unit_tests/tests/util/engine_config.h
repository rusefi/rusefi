//
// Created by kifir on 11/18/24.
//

#pragma once

#include "batt_lag_corr_curve.h"

class EngineConfig {
public:
    EngineConfig clone() const;

    // Injector
    std::optional<float> getInjectorFlow() const { return m_injectorFlow; }
    std::optional<BattLagCorrCurve> getInjectorBattLagCorr() const { return m_injectorBattLagCorrCurve; }
    std::optional<bool> getInjectorFlowAsMassFlow() const { return m_injectorFlowAsMassFlow; }

    // Secondary injector
    std::optional<float> getInjectorSecondaryFlow() const { return m_injectorSecondaryFlow; }
    std::optional<BattLagCorrCurve> getInjectorSecondaryBattLagCorr() const { return m_injectorSecondaryBattLagCorrCurve; }

    // Staged injection
    std::optional<bool> getStagedInjectionEnabled() const { return m_isStagedInjectionEnabled; }

    // We do not core about performance in tests, but we want to use builder-like style, so setters return new instance
    // of configuration:

    // Injector
    EngineConfig setInjectorFlowAsMassFlow(std::optional<bool> injectorFlowAsMassFlow);
    EngineConfig setInjectorFlow(std::optional<float> flow);
    EngineConfig setInjectorBattLagCorr(std::optional<BattLagCorrCurve> battLagCorr);

    // Secondary injector
    EngineConfig setInjectorSecondaryFlow(std::optional<float> flow);
    EngineConfig setInjectorSecondaryBattLagCorr(std::optional<BattLagCorrCurve> battLagCorr);

    // Staged injection
    EngineConfig setStagedInjectionEnabled(std::optional<bool> value);
private:
    // Injector
    std::optional<float> m_injectorFlow;
    std::optional<BattLagCorrCurve> m_injectorBattLagCorrCurve;
    std::optional<bool> m_injectorFlowAsMassFlow;;

    // Secondary injector
    std::optional<float> m_injectorSecondaryFlow;
    std::optional<BattLagCorrCurve> m_injectorSecondaryBattLagCorrCurve;

    // Staged injection
    std::optional<bool> m_isStagedInjectionEnabled;
};