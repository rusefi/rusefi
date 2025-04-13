//
// Created by kifir on 6/12/24.
//

#pragma once

#include "util/test_base.h"

class LaunchTestConfig : public EngineConfig {
public:
    bool getSatisfyActivationSwitchSpeedAndTpsConditions() const {
        return m_satisfyActivationSwitchSpeedAndTpsConditions;
    }
protected:
    void setSatisfyActivationSwitchSpeedAndTpsConditions(const bool value) {
        m_satisfyActivationSwitchSpeedAndTpsConditions = value;
    }
private:
    bool m_satisfyActivationSwitchSpeedAndTpsConditions { false };
};

class LaunchTestBase : public TestBase<> {
protected:
    void setUpTestConfig(const LaunchTestConfig& config);

private:
    void configureSatisfiedActivationSwitchSpeedAndTpsConditions();
};
