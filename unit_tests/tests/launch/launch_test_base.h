//
// Created by kifir on 6/12/24.
//

#pragma once

#include "util/test_base.h"

class LaunchTestConfig : public EngineConfig {
public:
    bool getSatisfyActivationSwithSpeedAndTpsConditions() const {
        return m_satisfyActivationSwithSpeedAndTpsConditions;
    }
protected:
    void setSatisfyActivationSwithSpeedAndTpsConditions(const bool value) {
        m_satisfyActivationSwithSpeedAndTpsConditions = value;
    }
private:
    bool m_satisfyActivationSwithSpeedAndTpsConditions { false };
};

class LaunchTestBase : public TestBase {
protected:
    void setUpTestConfig(const LaunchTestConfig& config);

private:
    void configureSatisfiedActivationSwithSpeedAndTpsConditions();
};