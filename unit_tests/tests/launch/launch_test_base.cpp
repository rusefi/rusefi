//
// Created by kifir on 6/12/24.
//

#include "pch.h"

#include "launch_test_base.h"

void LaunchTestBase::setUpTestConfig(const LaunchTestConfig& testConfig) {
    setUpEngineConfiguration(testConfig);
    if (testConfig.getSatisfyActivationSwitchSpeedAndTpsConditions()) {
        configureSatisfiedActivationSwitchSpeedAndTpsConditions();
    }
}

void LaunchTestBase::configureSatisfiedActivationSwitchSpeedAndTpsConditions() {
    getTestEngineConfiguration()
        .configureLaunchActivationMode(ALWAYS_ACTIVE_LAUNCH);  // to satisfy activateSwitchCondition
	getTestEngineConfiguration().configureLaunchSpeedThreshold(0); // to satisfy speedCondition
    engineConfiguration->launchTpsThreshold = 0;
    Sensor::setMockValue(SensorType::DriverThrottleIntent, 1.7); // to satisfy tpsCondition
}
