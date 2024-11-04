//
// Created by kifir on 6/12/24.
//

#include "pch.h"

#include "launch_test_base.h"

void LaunchTestBase::setUpTestConfig(const LaunchTestConfig& config) {
    getTestEngineConfiguration().configureLaunchControlEnabled(config.getLaunchControlEnabled());

    getTestEngineConfiguration().configureLaunchRpm(config.getLaunchRpm());
    getTestEngineConfiguration().configureLaunchRpmWindow(config.getLaunchRpmWindow());
    getTestEngineConfiguration().configureLaunchCorrectionsEndRpm(config.getLaunchCorrectionsEndRpm());

    getTestEngineConfiguration().configureIgnitionRetardEnable(config.getIgnitionRetardEnable());
    getTestEngineConfiguration().configureIgnitionRetard(config.getIgnitionRetard());
    getTestEngineConfiguration().configureSmoothRetardMode(config.getSmoothRetardMode());

    getTestEngineConfiguration().configureEnableIgnitionCut(config.getEnableIgnitionCut());
    getTestEngineConfiguration().configureInitialIgnitionCutPercent(config.getInitialIgnitionCut());
    getTestEngineConfiguration().configureFinalIgnitionCutPercentBeforeLaunch(config.getFinalIgnitionCutBeforeLaunch());

    if (config.getSatisfyActivationSwithSpeedAndTpsConditions()) {
        configureSatisfiedActivationSwithSpeedAndTpsConditions();
    }
}

void LaunchTestBase::configureSatisfiedActivationSwithSpeedAndTpsConditions() {
    engineConfiguration->launchActivationMode = ALWAYS_ACTIVE_LAUNCH; // to satisfy activateSwitchCondition
    engineConfiguration->launchSpeedThreshold = 0; // to satisfy speedCondition
    Sensor::setMockValue(SensorType::DriverThrottleIntent, 1.7); // to satisfy tpsCondition
}
