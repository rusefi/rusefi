//
// Created by kifir on 5/27/24.
//
// Tests for functionality decscribed in https://github.com/rusefi/rusefi/issues/5611
//

#include "pch.h"

constexpr float TEST_IGNITION_650 = 15.3f;
constexpr float TEST_IGNITION_800 = 15.4f;
constexpr float TEST_IGNITION_1100 = 15.5f;
constexpr float TEST_IGNITION_1400 = 15.6f;
constexpr float TEST_IGNITION_1700 = 15.7f;
constexpr float TEST_IGNITION_2000 = 15.8f;
constexpr float TEST_IGNITION_2300 = 15.9f;
constexpr float TEST_IGNITION_2600 = 16.0f;
constexpr float TEST_IGNITION_2900 = 16.1f;
constexpr float TEST_IGNITION_3200 = 16.2f;
constexpr float TEST_IGNITION_3500 = 16.3f;
constexpr float TEST_IGNITION_3800 = 16.4f;
constexpr float TEST_IGNITION_4100 = 16.5f;
constexpr float TEST_IGNITION_4400 = 16.6f;
constexpr float TEST_IGNITION_4700 = 16.7f;
constexpr float TEST_IGNITION_7000 = 16.8f;

constexpr int TEST_LAUNCH_RPM = 3800;
constexpr int TEST_LAUNCH_RPM_WINDOW = 1800;
constexpr int TEST_SMOOTH_RETARD_END_RPM = 900;
constexpr int TEST_SMOOTH_RETARD_RPM_WINDOW_START = TEST_LAUNCH_RPM - TEST_LAUNCH_RPM_WINDOW;
constexpr int TEST_SMOOTH_RETARD_RPM_WINDOW_END = TEST_LAUNCH_RPM - TEST_SMOOTH_RETARD_END_RPM;
constexpr int TEST_SMOOTH_RETARD_RPM_WINDOW = TEST_SMOOTH_RETARD_RPM_WINDOW_END - TEST_SMOOTH_RETARD_RPM_WINDOW_START;

constexpr float TEST_LAUNCH_TIMING_RETARD = 17.0f;

static void setUpTestParameters(
    const std::optional<bool> launchControlEnabled,
    const std::optional<bool> enableLaunchRetard,
    const std::optional<bool> launchSmoothRetard
) {
    for (int loadIdx = 0; loadIdx < IGN_LOAD_COUNT; loadIdx++) {
        config->ignitionTable[loadIdx][0] = TEST_IGNITION_650;
        config->ignitionTable[loadIdx][1] = TEST_IGNITION_800;
        config->ignitionTable[loadIdx][2] = TEST_IGNITION_1100;
        config->ignitionTable[loadIdx][3] = TEST_IGNITION_1400;
        config->ignitionTable[loadIdx][4] = TEST_IGNITION_1700;
        config->ignitionTable[loadIdx][5] = TEST_IGNITION_2000;
        config->ignitionTable[loadIdx][6] = TEST_IGNITION_2300;
        config->ignitionTable[loadIdx][7] = TEST_IGNITION_2600;
        config->ignitionTable[loadIdx][8] = TEST_IGNITION_2900;
        config->ignitionTable[loadIdx][9] = TEST_IGNITION_3200;
        config->ignitionTable[loadIdx][10] = TEST_IGNITION_3500;
        config->ignitionTable[loadIdx][11] = TEST_IGNITION_3800;
        config->ignitionTable[loadIdx][12] = TEST_IGNITION_4100;
        config->ignitionTable[loadIdx][13] = TEST_IGNITION_4400;
        config->ignitionTable[loadIdx][14] = TEST_IGNITION_4700;
        config->ignitionTable[loadIdx][15] = TEST_IGNITION_7000;
    }

    if (launchControlEnabled.has_value()) {
        engineConfiguration->launchControlEnabled = launchControlEnabled.value();
    } else {
        ASSERT_FALSE(engineConfiguration->launchControlEnabled); // check default value
    }
    engineConfiguration->launchRpm = TEST_LAUNCH_RPM;
    engineConfiguration->launchRpmWindow = TEST_LAUNCH_RPM_WINDOW;
    engineConfiguration->launchCorrectionsEndRpm = TEST_SMOOTH_RETARD_END_RPM;
    engineConfiguration->launchSparkCutEnable = true;
    engineConfiguration->launchTimingRetard = TEST_LAUNCH_TIMING_RETARD;

    if (enableLaunchRetard.has_value()) {
        engineConfiguration->enableLaunchRetard = enableLaunchRetard.value();
    } else {
        ASSERT_FALSE(engineConfiguration->enableLaunchRetard); // check default value
    }
    if (launchSmoothRetard.has_value()) {
        engineConfiguration->launchSmoothRetard = launchSmoothRetard.value();
    } else {
        ASSERT_FALSE(engineConfiguration->launchSmoothRetard); // check default value
    }
}

static void setUpTestParametersWithEnabledLaunchControl(
    const std::optional<bool> enableLaunchRetard = {},
    const std::optional<bool> launchSmoothRetard = {}
) {
    setUpTestParameters(
        /* launchControlEnabled = */ { true },
        /* enableLaunchRetard = */ enableLaunchRetard,
        /* launchSmoothRetard = */ launchSmoothRetard
    );
}

static float getTestAdvance(const float rpm) {
    return getAdvance(rpm, 500.0f); // we don't care of load in this test
}

static void updateRpm(const int rpm) {
    Sensor::setMockValue(SensorType::Rpm, rpm);
    // run the ignition math
    engine->periodicFastCallback();
}

/* Checks that angle advance is not affected by `Launch Control` functionality: */
static void checkAngleAdvanceWithoutLaunchControl(
    const std::optional<bool> launchControlEnabled = {},
    const std::optional<bool> enableLaunchRetard = {},
    const std::optional<bool> launchSmoothRetard = {}
) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    setUpTestParameters(
        /* launchControlEnabled = */ launchControlEnabled,
        /* enableLaunchRetard = */ enableLaunchRetard,
        /* launchSmoothRetard = */ launchSmoothRetard
    );

    updateRpm(650);
    EXPECT_NEAR(TEST_IGNITION_650, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(800);
    EXPECT_NEAR(TEST_IGNITION_800, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(1100);
    EXPECT_NEAR(TEST_IGNITION_1100, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(1400);
    EXPECT_NEAR(TEST_IGNITION_1400, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(1700);
    EXPECT_NEAR(TEST_IGNITION_1700, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(2000);
    /* We've entered smooth retard RPM window: */
    EXPECT_NEAR(TEST_IGNITION_2000, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(2300);
    EXPECT_NEAR(TEST_IGNITION_2300, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(2600);
    EXPECT_NEAR(TEST_IGNITION_2600, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(2900);
    EXPECT_NEAR(TEST_IGNITION_2900, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(3200);
    /* We've left smooth retard RPM window: */
    EXPECT_NEAR(TEST_IGNITION_3200, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(3500);
    EXPECT_NEAR(TEST_IGNITION_3500, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(3800);
    /* We've reached TEST_LAUNCH_RPM: */
    EXPECT_NEAR(TEST_IGNITION_3800, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(4100);
    EXPECT_NEAR(TEST_IGNITION_4100, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(4400);
    EXPECT_NEAR(TEST_IGNITION_4400, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(4700);
    EXPECT_NEAR(TEST_IGNITION_4700, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(7000);
    EXPECT_NEAR(TEST_IGNITION_7000, engine->ignitionState.baseIgnitionAdvance, EPS5D);
}

TEST(ignitionAngleAdvance, withEnabledLaunchControlAndWithoutLaunchRetard) {
    checkAngleAdvanceWithoutLaunchControl();
}

TEST(ignitionAngleAdvance, withEnabledLaunchControlAndLaunchRetardWithoutSmooth) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    setUpTestParametersWithEnabledLaunchControl(
        /* enableLaunchRetard = */ std::make_optional<bool>(true)
    );

    updateRpm(650);
    EXPECT_NEAR(TEST_IGNITION_650, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(800);
    EXPECT_NEAR(TEST_IGNITION_800, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(1100);
    EXPECT_NEAR(TEST_IGNITION_1100, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(1400);
    EXPECT_NEAR(TEST_IGNITION_1400, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(1700);
    EXPECT_NEAR(TEST_IGNITION_1700, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(2000);
    /* We've entered smooth retard RPM window: */
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(2300);
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(2600);
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(2900);
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(3200);
    /* See https://github.com/rusefi/rusefi/issues/5611#issuecomment-2137028838
     * We've left smooth retard RPM window:
     */
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(3500);
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(3800);
    /* We've reached TEST_LAUNCH_RPM: */
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(4100);
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(4400);
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(4700);
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(7000);
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);
}

TEST(ignitionAngleAdvance, withEnabledLaunchControlAndLaunchRetardAndLaunchSmoothRetard) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    setUpTestParametersWithEnabledLaunchControl(
        /* enableLaunchRetard = */ std::make_optional<bool>(true),
        /* launchSmoothRetard = */ std::make_optional<bool>(true)
    );

    updateRpm(650);
    EXPECT_NEAR(TEST_IGNITION_650, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(800);
    EXPECT_NEAR(TEST_IGNITION_800, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(1100);
    EXPECT_NEAR(TEST_IGNITION_1100, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(1400);
    EXPECT_NEAR(TEST_IGNITION_1400, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(1700);
    EXPECT_NEAR(TEST_IGNITION_1700, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(2000);
    /* We've entered smooth retard RPM window: */
    EXPECT_NEAR(TEST_IGNITION_2000, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(2300);
    EXPECT_NEAR(
        TEST_IGNITION_2300
                + (2300.0f - TEST_SMOOTH_RETARD_RPM_WINDOW_START) * (TEST_LAUNCH_TIMING_RETARD - TEST_IGNITION_2300)
                        / TEST_SMOOTH_RETARD_RPM_WINDOW,
        engine->ignitionState.baseIgnitionAdvance,
        EPS2D //TODO: check if this precision loss is expected
    );

    updateRpm(2600);
    EXPECT_NEAR(
        TEST_IGNITION_2600
                + (2600.0f - TEST_SMOOTH_RETARD_RPM_WINDOW_START) * (TEST_LAUNCH_TIMING_RETARD - TEST_IGNITION_2600)
                        / TEST_SMOOTH_RETARD_RPM_WINDOW,
        engine->ignitionState.baseIgnitionAdvance,
        EPS2D //TODO: check if this precision loss is expected
    );

    updateRpm(2900);
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(3200);
    /* See https://github.com/rusefi/rusefi/issues/5611#issuecomment-2136956050
     * We've left smooth retard RPM window:
     */
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(3500);
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(3800);
    /* See https://github.com/rusefi/rusefi/issues/5611#issuecomment-2136958259
     * We've reached TEST_LAUNCH_RPM:
     */
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(4100);
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(4400);
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(4700);
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);

    updateRpm(7000);
    EXPECT_NEAR(TEST_LAUNCH_TIMING_RETARD, engine->ignitionState.baseIgnitionAdvance, EPS5D);
}

/* Tests for https://github.com/rusefi/rusefi/issues/6571: */

TEST(ignitionAngleAdvance, withDisabledLaunchControlAndLaunchRetardWithoutSmooth) {
    checkAngleAdvanceWithoutLaunchControl(
        /* launchControlEnabled = */ { false },
        /* enableLaunchRetard = */ { true },
        /* launchSmoothRetard = */ { false }
    );
}

TEST(ignitionAngleAdvance, withDisabledLaunchControlAndLaunchRetardAndSmooth) {
    checkAngleAdvanceWithoutLaunchControl(
        /* launchControlEnabled = */ { false },
        /* enableLaunchRetard = */ { true },
        /* launchSmoothRetard = */ { true }
    );
}