//
// Created by kifir on 6/18/24.
//

#pragma once

#include "util/test_base.h"

class AcTestConfig {
public:
    std::optional<uint16_t> getMinAcPressure() const { return m_minAcPressure; }
    std::optional<uint16_t> getMaxAcPressure() const { return m_maxAcPressure; }
protected:
    void setMinAcPressure(const std::optional<uint16_t> value) { m_minAcPressure = value; }
    void setMaxAcPressure(const std::optional<uint16_t> value) { m_maxAcPressure = value; }
private:
    std::optional<uint16_t> m_minAcPressure;
    std::optional<uint16_t> m_maxAcPressure;
};

class AcTestBase : public TestBase {
protected:
    void updateAcPressure(float acPressure);

    void setUpTestConfig(const AcTestConfig& config);
private:
    void configureMinAcPressure(const std::optional<uint16_t> minAcPressure);
    void configureMaxAcPressure(const std::optional<uint16_t> maxAcPressure);
};
