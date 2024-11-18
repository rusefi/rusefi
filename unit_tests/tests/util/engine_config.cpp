//
// Created by kifir on 11/18/24.
//

#include "pch.h"

#include "engine_config.h"

EngineConfig EngineConfig::clone() const {
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

EngineConfig EngineConfig::setInjectorBattLagCorr(const std::optional<BattLagCorrCurve> battLagCorr) {
    m_injectorBattLagCorrCurve = battLagCorr;
    return *this;
}

EngineConfig EngineConfig::setInjectorSecondaryFlow(const std::optional<float> flow) {
    m_injectorSecondaryFlow = flow;
    return *this;
}

EngineConfig EngineConfig::setInjectorSecondaryBattLagCorr(const std::optional<BattLagCorrCurve> battLagCorr) {
    m_injectorSecondaryBattLagCorrCurve = battLagCorr;
    return *this;
}

EngineConfig EngineConfig::setStagedInjectionEnabled(const std::optional<bool> value) {
    m_isStagedInjectionEnabled = value;
    return *this;
}