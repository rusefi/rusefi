//
// Created by kifir on 6/18/24.
//

#pragma once

#include "util/test_base.h"

class AcTestConfig {
public:
    std::optional<float> getAcDelay() const { return m_acDelay; }
    std::optional<uint16_t> getMinAcPressure() const { return m_minAcPressure; }
    std::optional<uint16_t> getMaxAcPressure() const { return m_maxAcPressure; }
    std::optional<float> getAcPressureEnableHysteresis() const { return m_AcPressureEnableHysteresis; }
protected:
    void setAcDelay(const std::optional<float> value) { m_acDelay = value; }
    void setMinAcPressure(const std::optional<uint16_t> value) { m_minAcPressure = value; }
    void setMaxAcPressure(const std::optional<uint16_t> value) { m_maxAcPressure = value; }
    void setAcPressureEnableHysteresis(const std::optional<uint16_t> value) { m_AcPressureEnableHysteresis = value; }
private:
    std::optional<float> m_acDelay;
    std::optional<uint16_t> m_minAcPressure;
    std::optional<uint16_t> m_maxAcPressure;
    std::optional<float> m_AcPressureEnableHysteresis;
};

class AcTestBase : public TestBase {
protected:
    constexpr static float DEFAULT_AC_DELAY = 0.5f;
    constexpr static uint16_t DEFAULT_MIN_AC_PRESSURE = 100;
    constexpr static uint16_t DEFAULT_MAX_AC_PRESSURE = 300;
    constexpr static float DEFAULT_AC_PRESSURE_ENABLE_HYST = 0.0f;

    void updateAcPressure(float acPressure);

    void setUpTestConfig(const AcTestConfig& config);
private:
    void configureAcDelay(std::optional<float> acDelay);
    void configureMinAcPressure(std::optional<uint16_t> minAcPressure);
    void configureMaxAcPressure(std::optional<uint16_t> maxAcPressure);
    void configureAcPressureEnableHysteresis(std::optional<float> acPressureEnableHysteresis);
};
