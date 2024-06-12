//
// Created by kifir on 6/7/24.
//

#include "pch.h"

#include "launch_test_base.h"

namespace {
    constexpr int TEST_LAUNCH_RPM = 3850;
    constexpr int TEST_LAUNCH_RPM_WINDOW = 1850;
    constexpr int TEST_LAUNCH_SPARK_SKIP_END_RPM = 850;
    constexpr int TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START = TEST_LAUNCH_RPM - TEST_LAUNCH_RPM_WINDOW;
    constexpr int TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END = TEST_LAUNCH_RPM - TEST_LAUNCH_SPARK_SKIP_END_RPM;

    constexpr int TEST_INITIAL_IGNITION_CUT_PERCENT = 13;
    constexpr float TEST_INITIAL_IGNITION_CUT_RATIO = 0.13f;

    constexpr int TEST_FINAL_IGNITION_CUT_PERCENT = 42;
    constexpr float TEST_FINAL_IGNITION_CUT_RATIO = 0.42f;

    class SparkSkipRatioTestConfig : public LaunchTestConfig {
    public:
        SparkSkipRatioTestConfig(
                const std::optional<bool> launchControlEnabled,
                const std::optional<bool> enableIgnitionCut,
                const bool satifySwitchSpeedThresholdAndTpsConditions
        );
    };

    SparkSkipRatioTestConfig::SparkSkipRatioTestConfig(
            const std::optional<bool> launchControlEnabled,
            const std::optional<bool> enableIgnitionCut,
            const bool satifySwitchSpeedThresholdAndTpsConditions
    ) {
        setLaunchControlEnabled(launchControlEnabled);

        setLaunchRpm({ TEST_LAUNCH_RPM });
        setLaunchRpmWindow({ TEST_LAUNCH_RPM_WINDOW });
        setLaunchCorrectionsEndRpm({ TEST_LAUNCH_SPARK_SKIP_END_RPM });

        setEnableIgnitionCut(enableIgnitionCut);
        setInitialIgnitionCut({ TEST_INITIAL_IGNITION_CUT_PERCENT });
        setFinalIgnitionCutBeforeLaunch({ TEST_FINAL_IGNITION_CUT_PERCENT });

        setSatisfyActivationSwithSpeedAndTpsConditions(satifySwitchSpeedThresholdAndTpsConditions);
    }

    struct SparkSkipRatioTestData {
        const std::string context;
        const int rpm;
        const double expectedTargetSkipRatio;
        const std::optional<LaunchCondition> expectedLaunchCondition;
    };

    class SparkSkipRatioTest : public LaunchTestBase {
    protected:
        static std::vector<SparkSkipRatioTestData> generateNoLaunchSparkSkipTestData(double expectedTargetSkipRatio);

        static const std::vector<SparkSkipRatioTestData> DEFAULT_NO_LAUNCH_SPARK_SKIP_TEST_DATA;

        void doTest(const SparkSkipRatioTestConfig &config, const std::vector<SparkSkipRatioTestData> &testData);
    };

    std::vector<SparkSkipRatioTestData>
    SparkSkipRatioTest::generateNoLaunchSparkSkipTestData(const double expectedTargetSkipRatio) {
        return std::vector<SparkSkipRatioTestData>{
                {
                        "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START - 1 (before entering spark skip RPM window)",
                        TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START - 1,
                        expectedTargetSkipRatio,
                        {LaunchCondition::NotMet}
                },
                /* We've entered spark skip RPM window: */
                {
                        "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START (entering spark skip RPM window)",
                        TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START,
                        expectedTargetSkipRatio,
                        {LaunchCondition::NotMet}
                },
                {
                        "rpm = (TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START + TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END) / 2 (inside spark skip RPM window)",
                        (TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START + TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END) / 2,
                        expectedTargetSkipRatio,
                        {LaunchCondition::NotMet}
                },
                {
                        "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END (leaving spark skip RPM window)",
                        TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END,
                        expectedTargetSkipRatio,
                        {LaunchCondition::NotMet}
                },
                /* We've left spark skip RPM window: */
                {
                        "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END + 1 (after spark skip RPM window)",
                        TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END + 1,
                        expectedTargetSkipRatio,
                        {LaunchCondition::NotMet}
                },
                {
                        "rpm = TEST_LAUNCH_RPM - 1 (before launch)",
                        TEST_LAUNCH_RPM - 1,
                        expectedTargetSkipRatio,
                        {LaunchCondition::NotMet}
                },
                /* We've reached TEST_LAUNCH_RPM: */
                {
                        "rpm = TEST_LAUNCH_RPM (launch condition is still not satisfied)",
                        TEST_LAUNCH_RPM,
                        expectedTargetSkipRatio,
                        {LaunchCondition::NotMet}
                }
        };
    }

    const std::vector<SparkSkipRatioTestData> SparkSkipRatioTest::DEFAULT_NO_LAUNCH_SPARK_SKIP_TEST_DATA =
            SparkSkipRatioTest::generateNoLaunchSparkSkipTestData(0.0f);

    void SparkSkipRatioTest::doTest(
            const SparkSkipRatioTestConfig &config,
            const std::vector<SparkSkipRatioTestData> &testData
    ) {
        setUpTestConfig(config);

        for (const SparkSkipRatioTestData &testDataItem: testData) {
            updateRpm(testDataItem.rpm);
            if (testDataItem.expectedLaunchCondition.has_value()) {
                switch (testDataItem.expectedLaunchCondition.value()) {
                    case LaunchCondition::NotMet: {
                        EXPECT_FALSE(engine->launchController.isPreLaunchCondition) << testDataItem.context;
                        EXPECT_FALSE(engine->launchController.isLaunchCondition) << testDataItem.context;
                        break;
                    }
                    case LaunchCondition::PreLaunch: {
                        EXPECT_TRUE(engine->launchController.isPreLaunchCondition) << testDataItem.context;
                        EXPECT_FALSE(engine->launchController.isLaunchCondition) << testDataItem.context;
                        break;
                    }
                    case LaunchCondition::Launch: {
                        EXPECT_FALSE(engine->launchController.isPreLaunchCondition) << testDataItem.context;
                        EXPECT_TRUE(engine->launchController.isLaunchCondition) << testDataItem.context;
                        break;
                    }
                }
            }
            EXPECT_NEAR(engine->hardSparkLimiter.getTargetSkipRatio(), testDataItem.expectedTargetSkipRatio, EPS5D)
                                << testDataItem.context;
        }
    }

    TEST_F(SparkSkipRatioTest, raisingRpmToLaunchCondition) {
        doTest(
                /* config = */ {
                                       /* launchControlEnabled = */ {true},
                                       /* enableIgnitionCut = */ {true},
                                       /* satifySwitchSpeedThresholdAndTpsConditions = */ true
                               },
                /* testData = */ {
                                       {
                                               "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START - 1 (before entering spark skip RPM window)",
                                               TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START - 1,
                                               0.0f,
                                               {LaunchCondition::NotMet}
                                       },
                                       /* We've entered spark skip RPM window: */
                                       {
                                               "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START (entering spark skip RPM window)",
                                               TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START,
                                               TEST_INITIAL_IGNITION_CUT_RATIO,
                                               {LaunchCondition::PreLaunch}
                                       },
                                       {
                                               "rpm = (TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START + TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END) / 2 (inside spark skip RPM window)",
                                               (TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START +
                                                TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END) / 2,
                                               (TEST_INITIAL_IGNITION_CUT_RATIO + TEST_FINAL_IGNITION_CUT_RATIO) / 2,
                                               {LaunchCondition::PreLaunch}
                                       },
                                       {
                                               "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END (leaving spark skip RPM window)",
                                               TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END,
                                               TEST_FINAL_IGNITION_CUT_RATIO,
                                               {LaunchCondition::PreLaunch}
                                       },
                                       /* We've left spark skip RPM window: */
                                       {
                                               "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END + 1 (after spark skip RPM window)",
                                               TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END + 1,
                                               TEST_FINAL_IGNITION_CUT_RATIO,
                                               {LaunchCondition::PreLaunch}
                                       },
                                       {
                                               "rpm = TEST_LAUNCH_RPM - 1 (before launch)",
                                               TEST_LAUNCH_RPM - 1,
                                               TEST_FINAL_IGNITION_CUT_RATIO,
                                               {LaunchCondition::PreLaunch}
                                       },
                                       /* We've reached TEST_LAUNCH_RPM: */
                                       {
                                               "rpm = TEST_LAUNCH_RPM (launching)",
                                               TEST_LAUNCH_RPM,
                                               1.0f,
                                               {LaunchCondition::Launch}
                                       }
                               }
        );
    }

    TEST_F(SparkSkipRatioTest, raisingRpmWithoutLaunchCondition) {
        doTest(
                /* config = */ {
                                       /* launchControlEnabled = */ {true},
                                       /* enableIgnitionCut = */ {true},
                                       /* satifySwitchSpeedThresholdAndTpsConditions = */ false
                               },
                /* testData = */ DEFAULT_NO_LAUNCH_SPARK_SKIP_TEST_DATA
        );
    }

    TEST_F(SparkSkipRatioTest, raisingRpmWithDisabledIgnitionCut) {
        doTest(
                /* config = */ {
                                       /* launchControlEnabled = */ {true},
                                       /* enableIgnitionCut = */ {false},
                                       /* satifySwitchSpeedThresholdAndTpsConditions = */ true
                               },
                /* testData = */ {
                                       {
                                               "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START - 1 (before entering spark skip RPM window)",
                                               TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START - 1,
                                               0.0f,
                                               {LaunchCondition::NotMet}
                                       },
                                       /* We've entered spark skip RPM window: */
                                       {
                                               "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START (entering spark skip RPM window)",
                                               TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START,
                                               0.0f,
                                               {LaunchCondition::PreLaunch}
                                       },
                                       {
                                               "rpm = (TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START + TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END) / 2 (inside spark skip RPM window)",
                                               (TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START +
                                                TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END) / 2,
                                               0.0f,
                                               {LaunchCondition::PreLaunch}
                                       },
                                       {
                                               "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END (leaving spark skip RPM window)",
                                               TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END,
                                               0.0f,
                                               {LaunchCondition::PreLaunch}
                                       },
                                       /* We've left spark skip RPM window: */
                                       {
                                               "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END + 1 (after spark skip RPM window)",
                                               TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END + 1,
                                               0.0f,
                                               {LaunchCondition::PreLaunch}
                                       },
                                       {
                                               "rpm = TEST_LAUNCH_RPM - 1 (before launch)",
                                               TEST_LAUNCH_RPM - 1,
                                               0.0f,
                                               {LaunchCondition::PreLaunch}
                                       },
                                       /* We've reached TEST_LAUNCH_RPM: */
                                       {
                                               "rpm = TEST_LAUNCH_RPM (launch condition is still not satisfied)",
                                               TEST_LAUNCH_RPM,
                                               0.0f,
                                               {LaunchCondition::Launch}
                                       }
                               }
        );
    }

    TEST_F(SparkSkipRatioTest, raisingRpmWithDisabledLaunchControl) {
        doTest(
                /* config = */ {
                                       /* launchControlEnabled = */ {false},
                                       /* enableIgnitionCut = */ {true},
                                       /* satifySwitchSpeedThresholdAndTpsConditions = */ true
                               },
                /* testData = */ DEFAULT_NO_LAUNCH_SPARK_SKIP_TEST_DATA
        );
    }

    TEST_F(SparkSkipRatioTest, raisingRpmWithDisabledLaunchControlAndWithoutLaunchCondition) {
        doTest(
                /* config = */ {
                                       /* launchControlEnabled = */ {false},
                                       /* enableIgnitionCut = */ {true},
                                       /* satifySwitchSpeedThresholdAndTpsConditions = */ false
                               },
                /* testData = */ DEFAULT_NO_LAUNCH_SPARK_SKIP_TEST_DATA
        );
    }

    TEST_F(SparkSkipRatioTest, raisingRpmWithDisabledLaunchControlAndDisabledIgnitionCut) {
        doTest(
                /* config = */ {
                                       /* launchControlEnabled = */ {false},
                                       /* enableIgnitionCut = */ {false},
                                       /* satifySwitchSpeedThresholdAndTpsConditions = */ true
                               },
                /* testData = */ DEFAULT_NO_LAUNCH_SPARK_SKIP_TEST_DATA
        );
    }

    TEST_F(SparkSkipRatioTest, raisingRpmWithDisabledLaunchControlAndDisabledIgnitionCutAndWithoutLaunchCondition) {
        doTest(
                /* config = */ {
                                       /* launchControlEnabled = */ {false},
                                       /* enableIgnitionCut = */ {false},
                                       /* satifySwitchSpeedThresholdAndTpsConditions = */ false
                               },
                /* testData = */ DEFAULT_NO_LAUNCH_SPARK_SKIP_TEST_DATA
        );
    }

    TEST_F(SparkSkipRatioTest, raisingRpmWithDisabledIgnitionCutAndWithoutLaunchCondition) {
        doTest(
                /* config = */ {
                                       /* launchControlEnabled = */ {true},
                                       /* enableIgnitionCut = */ {false},
                                       /* satifySwitchSpeedThresholdAndTpsConditions = */ false
                               },
                /* testData = */ DEFAULT_NO_LAUNCH_SPARK_SKIP_TEST_DATA
        );
    }
}