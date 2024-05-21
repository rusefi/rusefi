//
// Created by kifir on 5/22/24.
//
// Tests for ticket https://github.com/rusefi/rusefi/issues/6533
//

#include "pch.h"

constexpr int TEST_LAUNCH_RPM = 2500;
constexpr int TEST_LAUNCH_RPM_WINDOW = 1000;

static void setUpTestParameters() {
    engineConfiguration->launchControlEnabled = true;
    engineConfiguration->launchRpm = TEST_LAUNCH_RPM;
    engineConfiguration->launchRpmWindow = TEST_LAUNCH_RPM_WINDOW;
    engineConfiguration->launchSparkCutEnable = true;

    engineConfiguration->launchActivationMode = ALWAYS_ACTIVE_LAUNCH; // to satisfy activateSwitchCondition
    engineConfiguration->launchSpeedThreshold = 0; // to satisfy speedCondition
    Sensor::setMockValue(SensorType::DriverThrottleIntent, 1.7); // to satisfy tpsCondition
}

static void updateRpm(const int rpm) {
    Sensor::setMockValue(SensorType::Rpm, rpm);
    engine->launchController.update();

    //isn't retardThresholdRpm redundant? Shold we use engineConfiguration->launchRpm instead?
    EXPECT_EQ(engine->launchController.retardThresholdRpm, engineConfiguration->launchRpm);
}

static void testRetardThresholdRpm() {
    updateRpm(TEST_LAUNCH_RPM);
    EXPECT_TRUE(engine->launchController.isLaunchCondition);
    EXPECT_FALSE(engine->launchController.isLaunchSparkRpmRetardCondition());

    updateRpm(TEST_LAUNCH_RPM + 1);
    EXPECT_TRUE(engine->launchController.isLaunchCondition);
    EXPECT_TRUE(engine->launchController.isLaunchSparkRpmRetardCondition());
}

TEST(rpmCondition, retardThresholdRpmWithDisabledLaunchRetard) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    setUpTestParameters();

    ASSERT_FALSE(engineConfiguration->enableLaunchRetard); // default value

    testRetardThresholdRpm();
}

TEST(rpmCondition, retardThresholdRpmWithEnabledLaunchRetard) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    setUpTestParameters();

    engineConfiguration->enableLaunchRetard = true;

    testRetardThresholdRpm();
}