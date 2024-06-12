//
// Created by kifir on 6/12/24.
//

#include "pch.h"

#include "launch_test_base.h"


void LaunchTestBase::SetUp() {
    eth = std::make_unique<EngineTestHelper>(engine_type_e::TEST_ENGINE);
}

void LaunchTestBase::TearDown() {
    eth.reset();
}

void LaunchTestBase::setUpTestConfig(const LaunchTestConfig& config) {
    configureLaunchControlEnabled(config.getLaunchControlEnabled());

    configureLaunchRpm(config.getLaunchRpm());
    configureLaunchRpmWindow(config.getLaunchRpmWindow());
    configureLaunchCorrectionsEndRpm(config.getLaunchCorrectionsEndRpm());

    configureEnableIgnitionCut(config.getEnableIgnitionCut());
    configureInitialIgnitionCutPercent(config.getInitialIgnitionCut());
    configureFinalIgnitionCutPercentBeforeLaunch(config.getFinalIgnitionCutBeforeLaunch());

    if (config.getSatisfyActivationSwithSpeedAndTpsConditions()) {
        configureSatisfiedActivationSwithSpeedAndTpsConditions();
    }
}

void LaunchTestBase::updateRpm(const int rpm) {
    Sensor::setMockValue(SensorType::Rpm, rpm);
    // run the ignition math
    engine->periodicFastCallback();
}

void LaunchTestBase::configureLaunchControlEnabled(const std::optional<bool> launchControlEnabled) {
    if (launchControlEnabled.has_value()) {
        engineConfiguration->launchControlEnabled = launchControlEnabled.value();
    } else {
        ASSERT_FALSE(engineConfiguration->launchControlEnabled); // check default value
    }
}

void LaunchTestBase::configureLaunchRpm(const std::optional<int> launchRpm) {
    if (launchRpm.has_value()) {
        engineConfiguration->launchRpm = launchRpm.value();
    } else {
        ASSERT_EQ(engineConfiguration->launchRpm, 0); // check default value
    }
}

void LaunchTestBase::configureLaunchRpmWindow(const std::optional<int> launchRpmWindow) {
    if (launchRpmWindow.has_value()) {
        engineConfiguration->launchRpmWindow = launchRpmWindow.value();
    } else {
        ASSERT_EQ(engineConfiguration->launchRpmWindow, 0); // check default value
    }
}

void LaunchTestBase::configureLaunchCorrectionsEndRpm(const std::optional<int> launchCorrectionsEndRpm) {
    if (launchCorrectionsEndRpm.has_value()) {
        engineConfiguration->launchCorrectionsEndRpm = launchCorrectionsEndRpm.value();
    } else {
        ASSERT_EQ(engineConfiguration->launchCorrectionsEndRpm, 0); // check default value
    }
}

void LaunchTestBase::configureEnableIgnitionCut(const std::optional<bool> enableIgnitionCut) {
    if (enableIgnitionCut.has_value()) {
        engineConfiguration->launchSparkCutEnable = enableIgnitionCut.value();
    } else {
        ASSERT_FALSE(engineConfiguration->launchSparkCutEnable); // check default value
    }
}

void LaunchTestBase::configureInitialIgnitionCutPercent(const std::optional<int> initialIgnitionCutPercent) {
    if (initialIgnitionCutPercent.has_value()) {
        engineConfiguration->initialIgnitionCutPercent = initialIgnitionCutPercent.value();
    } else {
        ASSERT_EQ(engineConfiguration->initialIgnitionCutPercent, 0); // check default value
    }
}

void LaunchTestBase::configureFinalIgnitionCutPercentBeforeLaunch(
    const std::optional<int> finalIgnitionCutPercentBeforeLaunch
) {
    if (finalIgnitionCutPercentBeforeLaunch.has_value()) {
        engineConfiguration->finalIgnitionCutPercentBeforeLaunch = finalIgnitionCutPercentBeforeLaunch.value();
    } else {
        ASSERT_EQ(engineConfiguration->finalIgnitionCutPercentBeforeLaunch, 0); // check default value
    }
}

void LaunchTestBase::configureSatisfiedActivationSwithSpeedAndTpsConditions() {
    engineConfiguration->launchActivationMode = ALWAYS_ACTIVE_LAUNCH; // to satisfy activateSwitchCondition
    engineConfiguration->launchSpeedThreshold = 0; // to satisfy speedCondition
    Sensor::setMockValue(SensorType::DriverThrottleIntent, 1.7); // to satisfy tpsCondition
}
