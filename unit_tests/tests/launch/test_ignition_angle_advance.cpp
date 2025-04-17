//
// Created by kifir on 5/27/24.
//
// Tests for functionality decscribed in https://github.com/rusefi/rusefi/issues/5611
//

#include "pch.h"

#include "launch_test_base.h"

namespace {
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

    class IgnitionAngleAdvanceTestConfig : public LaunchTestConfig {
    public:
        IgnitionAngleAdvanceTestConfig(
            const std::optional<bool> launchControlEnabled,
            const std::optional<bool> ignitionRetardEnable,
            const std::optional<bool> smoothRetardMode,
            const bool satisfySwitchSpeedThresholdAndTpsConditions
        );
    };

    IgnitionAngleAdvanceTestConfig::IgnitionAngleAdvanceTestConfig(
        const std::optional<bool> launchControlEnabled,
        const std::optional<bool> ignitionRetardEnable,
        const std::optional<bool> smoothRetardMode,
        const bool satisfySwitchSpeedThresholdAndTpsConditions
    ) {
        setLaunchControlEnabled(launchControlEnabled);

        setLaunchRpm({TEST_LAUNCH_RPM });
        setLaunchRpmWindow({TEST_LAUNCH_RPM_WINDOW });
        setLaunchCorrectionsEndRpm({TEST_SMOOTH_RETARD_END_RPM });

        setIgnitionRetardEnable({ ignitionRetardEnable });
        setIgnitionRetard({ TEST_LAUNCH_TIMING_RETARD });
        setSmoothRetardMode({ smoothRetardMode });

        setEnableIgnitionCut(true);

        setSatisfyActivationSwitchSpeedAndTpsConditions(satisfySwitchSpeedThresholdAndTpsConditions);
    }

    struct IgnitionAngleAdvanceTestData {
        const std::string context;
        const int rpm;
        const float expectedIgnitionAdvance;
        const float epsilon = EPS5D;
    };

#if (IGN_RPM_COUNT == 16) // the following test uses hardcoded test ignition table with 16 columns
    class IgnitionAngleAdvanceTest : public LaunchTestBase {
    protected:
        void doTest(
            const IgnitionAngleAdvanceTestConfig& config,
            const std::vector<IgnitionAngleAdvanceTestData> &testData
        );

        static const std::vector<IgnitionAngleAdvanceTestData> TEST_DATA_WITHOUT_LAUNCH_ANGLE_ADVANCE;
    private:
        void configureTestIgnitionTable();
    };

    void IgnitionAngleAdvanceTest::doTest(
        const IgnitionAngleAdvanceTestConfig& config,
        const std::vector<IgnitionAngleAdvanceTestData> &testData
    ) {
        configureTestIgnitionTable();
        setUpTestConfig(config);

        for (const IgnitionAngleAdvanceTestData &testDataItem: testData) {
            updateRpm(testDataItem.rpm);
            EXPECT_NEAR(
                testDataItem.expectedIgnitionAdvance,
                engine->ignitionState.baseIgnitionAdvance,
                testDataItem.epsilon
            ) << testDataItem.context;
        }
    }

    const std::vector<IgnitionAngleAdvanceTestData> IgnitionAngleAdvanceTest::TEST_DATA_WITHOUT_LAUNCH_ANGLE_ADVANCE = {
            { "TEST_IGNITION_650",  650,  TEST_IGNITION_650 },
            { "TEST_IGNITION_800",  800,  TEST_IGNITION_800 },
            { "TEST_IGNITION_1100", 1100, TEST_IGNITION_1100 },
            { "TEST_IGNITION_1400", 1400, TEST_IGNITION_1400 },
            { "TEST_IGNITION_1700", 1700, TEST_IGNITION_1700 },
            /* We've entered smooth retard RPM window: */
            { "TEST_IGNITION_2000", 2000, TEST_IGNITION_2000 },
            { "TEST_IGNITION_2300", 2300, TEST_IGNITION_2300 },
            { "TEST_IGNITION_2600", 2600, TEST_IGNITION_2600 },
            { "TEST_IGNITION_2900", 2900, TEST_IGNITION_2900 },
            /* See https://github.com/rusefi/rusefi/issues/5611#issuecomment-2137028838
             * We've left smooth retard RPM window:
             */
            { "TEST_IGNITION_3200", 3200, TEST_IGNITION_3200 },
            { "TEST_IGNITION_3500", 3500, TEST_IGNITION_3500 },
            /* We've reached TEST_LAUNCH_RPM: */
            { "TEST_IGNITION_3800", 3800, TEST_IGNITION_3800 },
            { "TEST_IGNITION_4100", 4100, TEST_IGNITION_4100 },
            { "TEST_IGNITION_4400", 4400, TEST_IGNITION_4400 },
            { "TEST_IGNITION_4700", 4700, TEST_IGNITION_4700 },
            { "TEST_IGNITION_7000", 7000, TEST_IGNITION_7000 }
    };

    void IgnitionAngleAdvanceTest::configureTestIgnitionTable() {
        IgnitionTable testIgnitionTable;
        for (int loadIdx = 0; loadIdx < IGN_LOAD_COUNT; loadIdx++) {
            testIgnitionTable[loadIdx][0] = TEST_IGNITION_650;
            testIgnitionTable[loadIdx][1] = TEST_IGNITION_800;
            testIgnitionTable[loadIdx][2] = TEST_IGNITION_1100;
            testIgnitionTable[loadIdx][3] = TEST_IGNITION_1400;
            testIgnitionTable[loadIdx][4] = TEST_IGNITION_1700;
            testIgnitionTable[loadIdx][5] = TEST_IGNITION_2000;
            testIgnitionTable[loadIdx][6] = TEST_IGNITION_2300;
            testIgnitionTable[loadIdx][7] = TEST_IGNITION_2600;
            testIgnitionTable[loadIdx][8] = TEST_IGNITION_2900;
            testIgnitionTable[loadIdx][9] = TEST_IGNITION_3200;
            testIgnitionTable[loadIdx][10] = TEST_IGNITION_3500;
            testIgnitionTable[loadIdx][11] = TEST_IGNITION_3800;
            testIgnitionTable[loadIdx][12] = TEST_IGNITION_4100;
            testIgnitionTable[loadIdx][13] = TEST_IGNITION_4400;
            testIgnitionTable[loadIdx][14] = TEST_IGNITION_4700;
            testIgnitionTable[loadIdx][15] = TEST_IGNITION_7000;
            static_assert(IGN_RPM_COUNT == 16);
        };
        getTestPersistentConfiguration().setIgnitionTable(testIgnitionTable);
    }

    TEST_F(IgnitionAngleAdvanceTest, withDisabledLaunchControlAndWithoutLaunchRetardWithSatisfiedLaunchConditions) {
        doTest(
            /* config = */ {
                /* launchControlEnabled = */ {},
                /* ignitionRetardEnable = */ {},
                /* smoothRetardMode = */ {},
                /* satisfySwitchSpeedThresholdAndTpsConditions = */ true
            },
            /* testData = */ TEST_DATA_WITHOUT_LAUNCH_ANGLE_ADVANCE
        );
    }

    TEST_F(
        IgnitionAngleAdvanceTest,
        withEnabledLaunchControlAndLaunchRetardWithoutSmoothWithSatisfiedLaunchConditions
    ) {
        doTest(
            /* config = */ {
                /* launchControlEnabled = */ { true },
                /* ignitionRetardEnable = */ { true },
                /* smoothRetardMode = */ { false },
                /* satisfySwitchSpeedThresholdAndTpsConditions = */ true
            },
            /* testData = */ {
                { "TEST_IGNITION_650",  650,  TEST_IGNITION_650 },
                { "TEST_IGNITION_800",  800,  TEST_IGNITION_800 },
                { "TEST_IGNITION_1100", 1100, TEST_IGNITION_1100 },
                { "TEST_IGNITION_1400", 1400, TEST_IGNITION_1400 },
                { "TEST_IGNITION_1700", 1700, TEST_IGNITION_1700 },
                /* We've  entered smooth retard RPM window: */
                { "TEST_IGNITION_2000", 2000, TEST_LAUNCH_TIMING_RETARD },
                { "TEST_IGNITION_2300", 2300, TEST_LAUNCH_TIMING_RETARD },
                { "TEST_IGNITION_2600", 2600, TEST_LAUNCH_TIMING_RETARD },
                { "TEST_IGNITION_2900", 2900, TEST_LAUNCH_TIMING_RETARD },
                /* See https://github.com/rusefi/rusefi/issues/5611#issuecomment-2137028838
                 * We've left smooth retard RPM window:
                 */
                { "TEST_IGNITION_3200", 3200, TEST_LAUNCH_TIMING_RETARD },
                { "TEST_IGNITION_3500", 3500, TEST_LAUNCH_TIMING_RETARD },
                /* We've reached TEST_LAUNCH_RPM: */
                { "TEST_IGNITION_3800", 3800, TEST_LAUNCH_TIMING_RETARD },
                { "TEST_IGNITION_4100", 4100, TEST_LAUNCH_TIMING_RETARD },
                { "TEST_IGNITION_4400", 4400, TEST_LAUNCH_TIMING_RETARD },
                { "TEST_IGNITION_4700", 4700, TEST_LAUNCH_TIMING_RETARD },
                { "TEST_IGNITION_7000", 7000, TEST_LAUNCH_TIMING_RETARD },
            }
        );
    }

    TEST_F(
        IgnitionAngleAdvanceTest,
        withEnabledLaunchControlAndLaunchRetardAndLaunchSmoothRetardWithSatisfiedLaunchConditions
    ) {
        doTest(
            /* config = */ {
               /* launchControlEnabled = */ { true },
               /* ignitionRetardEnable = */ { true },
               /* smoothRetardMode = */ { true },
               /* satisfySwitchSpeedThresholdAndTpsConditions = */ true
            },
            /* testData = */ {
                { "TEST_IGNITION_650",  650,  TEST_IGNITION_650 },
                { "TEST_IGNITION_800",  800,  TEST_IGNITION_800 },
                { "TEST_IGNITION_1100", 1100, TEST_IGNITION_1100 },
                { "TEST_IGNITION_1400", 1400, TEST_IGNITION_1400 },
                { "TEST_IGNITION_1700", 1700, TEST_IGNITION_1700 },
                /* We've entered smooth retard RPM window: */
                { "TEST_IGNITION_2000", 2000, TEST_IGNITION_2000 },
                {
                    "TEST_IGNITION_2300",
                    2300,
                    TEST_IGNITION_2300
                        + (2300.0f - TEST_SMOOTH_RETARD_RPM_WINDOW_START) *
                            (TEST_LAUNCH_TIMING_RETARD - TEST_IGNITION_2300) / TEST_SMOOTH_RETARD_RPM_WINDOW,
                    EPS2D //TODO: check if this precision loss is expected
                },
                {
                    "TEST_IGNITION_2600",
                    2600,
                    TEST_IGNITION_2600
                        + (2600.0f - TEST_SMOOTH_RETARD_RPM_WINDOW_START) *
                            (TEST_LAUNCH_TIMING_RETARD - TEST_IGNITION_2600) / TEST_SMOOTH_RETARD_RPM_WINDOW,
                    EPS2D //TODO: check if this precision loss is expected
                },
                { "TEST_IGNITION_2900", 2900, TEST_LAUNCH_TIMING_RETARD },
                /* See https://github.com/rusefi/rusefi/issues/5611#issuecomment-2137028838
                 * We've left smooth retard RPM window:
                 */
                { "TEST_IGNITION_3200", 3200, TEST_LAUNCH_TIMING_RETARD },
                { "TEST_IGNITION_3500", 3500, TEST_LAUNCH_TIMING_RETARD },
                /* We've reached TEST_LAUNCH_RPM: */
                { "TEST_IGNITION_3800", 3800, TEST_LAUNCH_TIMING_RETARD },
                { "TEST_IGNITION_4100", 4100, TEST_LAUNCH_TIMING_RETARD },
                { "TEST_IGNITION_4400", 4400, TEST_LAUNCH_TIMING_RETARD },
                { "TEST_IGNITION_4700", 4700, TEST_LAUNCH_TIMING_RETARD },
                { "TEST_IGNITION_7000", 7000, TEST_LAUNCH_TIMING_RETARD }
            }
        );
    }

    /* Tests for https://github.com/rusefi/rusefi/issues/6571: */

    TEST_F(
        IgnitionAngleAdvanceTest,
        withDisabledLaunchControlAndLaunchRetardWithoutSmoothWithSatisfiedLaunchConditions
    ) {
        doTest(
            /* config = */ {
                /* launchControlEnabled = */ { false },
                /* ignitionRetardEnable = */ { true },
                /* smoothRetardMode = */ { false },
                /* satisfySwitchSpeedThresholdAndTpsConditions = */ true
            },
            /* testData = */ TEST_DATA_WITHOUT_LAUNCH_ANGLE_ADVANCE
        );
    }

    TEST_F(IgnitionAngleAdvanceTest, withDisabledLaunchControlAndLaunchRetardAndSmoothWithSatisfiedLaunchConditions) {
        doTest(
            /* config = */ {
                /* launchControlEnabled = */ { false },
                /* ignitionRetardEnable = */ { true },
                /* smoothRetardMode = */ { true },
                /* satisfySwitchSpeedThresholdAndTpsConditions = */ true
            },
            /* testData = */ TEST_DATA_WITHOUT_LAUNCH_ANGLE_ADVANCE
        );
    }

    /* Tests with unsatisfied launch conditions: */

    TEST_F(
        IgnitionAngleAdvanceTest,
        withDisabledLaunchControlAndWithoutLaunchRetardWithDisabledSmoothRetardWithoutSatisfiedLaunchConditions
    ) {
        doTest(
            /* config = */ {
                /* launchControlEnabled = */ { false },
                /* ignitionRetardEnable = */ { false },
                /* smoothRetardMode = */ { false },
                /* satisfySwitchSpeedThresholdAndTpsConditions = */ false
            },
            /* testData = */ TEST_DATA_WITHOUT_LAUNCH_ANGLE_ADVANCE
        );
    }

    TEST_F(
        IgnitionAngleAdvanceTest,
        withEnabledLaunchControlAndLaunchRetardWithoutSmoothAndSatisfiedLaunchConditions
    ) {
        doTest(
            /* config = */ {
                /* launchControlEnabled = */ { true },
                /* ignitionRetardEnable = */ { true },
                /* smoothRetardMode = */ { false },
                /* satisfySwitchSpeedThresholdAndTpsConditions = */ false
            },
            /* testData = */ TEST_DATA_WITHOUT_LAUNCH_ANGLE_ADVANCE
        );
    }

    TEST_F(
        IgnitionAngleAdvanceTest,
        withEnabledLaunchControlAndLaunchRetardAndLaunchSmoothRetardWithoutSatisfiedLaunchConditions
    ) {
        doTest(
            /* config = */ {
                /* launchControlEnabled = */ { true },
                /* ignitionRetardEnable = */ { true },
                /* smoothRetardMode = */ { true },
                /* satisfySwitchSpeedThresholdAndTpsConditions = */ false
            },
            /* testData = */ TEST_DATA_WITHOUT_LAUNCH_ANGLE_ADVANCE
        );
    }

    TEST_F(
        IgnitionAngleAdvanceTest,
        withDisabledLaunchControlAndLaunchRetardWithoutSmoothAndWithoutSatisfiedLaunchConditions
    ) {
        doTest(
                /* config = */ {
                                       /* launchControlEnabled = */ { false },
                                       /* ignitionRetardEnable = */ { true },
                                       /* smoothRetardMode = */ { false },
                                       /* satisfySwitchSpeedThresholdAndTpsConditions = */ false
                               },
                /* testData = */ TEST_DATA_WITHOUT_LAUNCH_ANGLE_ADVANCE
        );
    }

    TEST_F(
        IgnitionAngleAdvanceTest,
        withDisabledLaunchControlAndLaunchRetardAndSmoothAndWithoutSatisfiedLaunchConditions
    ) {
        doTest(
                /* config = */ {
                                       /* launchControlEnabled = */ { false },
                                       /* ignitionRetardEnable = */ { true },
                                       /* smoothRetardMode = */ { true },
                                       /* satisfySwitchSpeedThresholdAndTpsConditions = */ false
                               },
                /* testData = */ TEST_DATA_WITHOUT_LAUNCH_ANGLE_ADVANCE
        );
    }
#endif //(IGN_RPM_COUNT == 16)
}
