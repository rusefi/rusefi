//
// Created by kifir on 6/7/24.
//

#include "pch.h"

constexpr int TEST_LAUNCH_RPM = 3850;
constexpr int TEST_LAUNCH_RPM_WINDOW = 1850;
constexpr int TEST_LAUNCH_SPARK_SKIP_END_RPM = 850;
constexpr int TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START = TEST_LAUNCH_RPM - TEST_LAUNCH_RPM_WINDOW;
constexpr int TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END = TEST_LAUNCH_RPM - TEST_LAUNCH_SPARK_SKIP_END_RPM;
constexpr int TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END - TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START;

constexpr int TEST_INITIAL_IGNITION_CUT_PERCENT = 13;
constexpr float TEST_INITIAL_IGNITION_CUT_RATIO = 0.13f;

constexpr int TEST_FINAL_IGNITION_CUT_PERCENT = 42;
constexpr float TEST_FINAL_IGNITION_CUT_RATIO = 0.42f;

static void setUpTestParameters(
    const std::optional<bool> launchControlEnabled,
    const std::optional<bool> enableIgnitionCut
) {
    if (launchControlEnabled.has_value()) {
        engineConfiguration->launchControlEnabled = launchControlEnabled.value();
    } else {
        ASSERT_FALSE(engineConfiguration->launchControlEnabled); // check default value
    }
    if (enableIgnitionCut.has_value()) {
        engineConfiguration->launchSparkCutEnable = enableIgnitionCut.value();
    } else {
        ASSERT_FALSE(engineConfiguration->launchSparkCutEnable); // check default value
    }
    engineConfiguration->launchRpm = TEST_LAUNCH_RPM;
    engineConfiguration->launchRpmWindow = TEST_LAUNCH_RPM_WINDOW;
    engineConfiguration->launchCorrectionsEndRpm = TEST_LAUNCH_SPARK_SKIP_END_RPM;

    engineConfiguration->initialIgnitionCutPercent = TEST_INITIAL_IGNITION_CUT_PERCENT;
    engineConfiguration->finalIgnitionCutPercentBeforeLaunch = TEST_FINAL_IGNITION_CUT_PERCENT;

    engineConfiguration->launchActivationMode = ALWAYS_ACTIVE_LAUNCH; // to satisfy activateSwitchCondition
    engineConfiguration->launchSpeedThreshold = 0; // to satisfy speedCondition
    Sensor::setMockValue(SensorType::DriverThrottleIntent, 1.7); // to satisfy tpsCondition
}

static void updateRpm(const int rpm, EngineTestHelper& eth) {
    Sensor::setMockValue(SensorType::Rpm, rpm);
    for (int i = 0; i < 2; i++) {\
        /*
         * We need to repeat the following code twice because method EngineState::periodicFastCallback updates launch
         * controller after updating spark skip - can we fix it safely?
         */
        eth.moveTimeForwardSec(1);
        // run the ignition math
        engine->periodicFastCallback();
    }
}

TEST(skipSparkRatio, raisingRpm) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    setUpTestParameters({ true }, { true });

    updateRpm(TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START - 1, eth);
    EXPECT_FALSE(engine->launchController.isLaunchCondition);
    EXPECT_EQ(engine->hardSparkLimiter.getTargetSkipRatio(), 0.0f);

    updateRpm(TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START, eth);
    /* We've entered spark skip RPM window: */
    EXPECT_FALSE(engine->launchController.isLaunchCondition);
    EXPECT_EQ(engine->hardSparkLimiter.getTargetSkipRatio(), TEST_INITIAL_IGNITION_CUT_RATIO);

    updateRpm(TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END, eth);
    EXPECT_FALSE(engine->launchController.isLaunchCondition);
    EXPECT_EQ(engine->hardSparkLimiter.getTargetSkipRatio(), TEST_FINAL_IGNITION_CUT_RATIO);

    updateRpm((TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END - TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START) / 2, eth);
    EXPECT_FALSE(engine->launchController.isLaunchCondition);
    EXPECT_EQ(
        engine->hardSparkLimiter.getTargetSkipRatio(),
        (TEST_FINAL_IGNITION_CUT_RATIO - TEST_FINAL_IGNITION_CUT_RATIO) / 2
    );

    updateRpm(TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END + 1, eth);
    /* We've left spark skip RPM window: */
    EXPECT_FALSE(engine->launchController.isLaunchCondition);
    EXPECT_EQ(engine->hardSparkLimiter.getTargetSkipRatio(), TEST_FINAL_IGNITION_CUT_RATIO);

    updateRpm(TEST_LAUNCH_RPM - 1, eth);
    EXPECT_FALSE(engine->launchController.isLaunchCondition);
    EXPECT_EQ(engine->hardSparkLimiter.getTargetSkipRatio(), TEST_FINAL_IGNITION_CUT_RATIO);

    /* We've reached TEST_LAUNCH_RPM: */
    updateRpm(TEST_LAUNCH_RPM, eth);
    EXPECT_TRUE(engine->launchController.isLaunchCondition);
    EXPECT_EQ(engine->hardSparkLimiter.getTargetSkipRatio(), 1.0);
}