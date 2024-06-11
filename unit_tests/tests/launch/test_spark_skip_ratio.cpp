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

struct SparkSkipRatioTestConfig {
    const std::optional<bool> launchControlEnabled;
    const std::optional<bool> enableIgnitionCut;
    const bool satifySwitchSpeedThresholdAndTpsConditions;
};

struct RpmTestData {
    const std::string context;
    const int rpm;
    const double expectedTargetSkipRatio;
    const std::optional<LaunchCondition> expectedLaunchCondition;
};

class SparkSkipRatioTest : public testing::Test {
private:
    std::unique_ptr<EngineTestHelper> eth;
protected:
    virtual void SetUp() override;

    virtual void TearDown() override;

    static std::vector<RpmTestData> generateNoLaunchSparkSkipTestData(double expectedTargetSkipRatio);

    static const std::vector<RpmTestData> DEFAULT_NO_LAUNCH_SPARK_SKIP_TEST_DATA;

    void doTest(const SparkSkipRatioTestConfig& config, const std::vector<RpmTestData>& testData);
private:
    void setUpTestParameters(const SparkSkipRatioTestConfig& config);

    void updateRpm(const int rpm);

    void checkNoLaunchSparkSkip(const SparkSkipRatioTestConfig& config);
};

void SparkSkipRatioTest::SetUp() {
    eth = std::make_unique<EngineTestHelper>(engine_type_e::TEST_ENGINE);
}

void SparkSkipRatioTest::TearDown() {
    eth.reset();
}

std::vector<RpmTestData> SparkSkipRatioTest::generateNoLaunchSparkSkipTestData(const double expectedTargetSkipRatio) {
    return std::vector<RpmTestData> {
        {
            "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START - 1 (before entering spark skip RPM window)",
            TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START - 1,
            expectedTargetSkipRatio,
            { LaunchCondition::NotMet }
        },
        /* We've entered spark skip RPM window: */
        {
            "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START (entering spark skip RPM window)",
            TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START,
            expectedTargetSkipRatio,
            { LaunchCondition::NotMet }
        },
        {
            "rpm = (TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START + TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END) / 2 (inside spark skip RPM window)",
            (TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START + TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END) / 2,
            expectedTargetSkipRatio,
            { LaunchCondition::NotMet }
        },
        {
            "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END (leaving spark skip RPM window)",
            TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END,
            expectedTargetSkipRatio,
            { LaunchCondition::NotMet }
        },
        /* We've left spark skip RPM window: */
        {
            "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END + 1 (after spark skip RPM window)",
            TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END + 1,
            expectedTargetSkipRatio,
            { LaunchCondition::NotMet }
        },
        {
            "rpm = TEST_LAUNCH_RPM - 1 (before launch)",
            TEST_LAUNCH_RPM - 1,
            expectedTargetSkipRatio,
            { LaunchCondition::NotMet }
        },
        /* We've reached TEST_LAUNCH_RPM: */
        {
            "rpm = TEST_LAUNCH_RPM (launch condition is still not satisfied)",
            TEST_LAUNCH_RPM,
            expectedTargetSkipRatio,
            { LaunchCondition::NotMet }
        }
    };
}

const std::vector<RpmTestData> SparkSkipRatioTest::DEFAULT_NO_LAUNCH_SPARK_SKIP_TEST_DATA =
    SparkSkipRatioTest::generateNoLaunchSparkSkipTestData(0.0f);

void SparkSkipRatioTest::doTest(
    const SparkSkipRatioTestConfig& config,
    const std::vector<RpmTestData>& testData
) {
    setUpTestParameters(config);

    for (const RpmTestData& testDataItem: testData) {
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

void SparkSkipRatioTest::setUpTestParameters(const SparkSkipRatioTestConfig& config) {
    if (config.launchControlEnabled.has_value()) {
        engineConfiguration->launchControlEnabled = config.launchControlEnabled.value();
    } else {
        ASSERT_FALSE(engineConfiguration->launchControlEnabled); // check default value
    }
    if (config.enableIgnitionCut.has_value()) {
        engineConfiguration->launchSparkCutEnable = config.enableIgnitionCut.value();
    } else {
        ASSERT_FALSE(engineConfiguration->launchSparkCutEnable); // check default value
    }
    engineConfiguration->launchRpm = TEST_LAUNCH_RPM;
    engineConfiguration->launchRpmWindow = TEST_LAUNCH_RPM_WINDOW;
    engineConfiguration->launchCorrectionsEndRpm = TEST_LAUNCH_SPARK_SKIP_END_RPM;

    engineConfiguration->initialIgnitionCutPercent = TEST_INITIAL_IGNITION_CUT_PERCENT;
    engineConfiguration->finalIgnitionCutPercentBeforeLaunch = TEST_FINAL_IGNITION_CUT_PERCENT;

    if (config.satifySwitchSpeedThresholdAndTpsConditions) {
        engineConfiguration->launchActivationMode = ALWAYS_ACTIVE_LAUNCH; // to satisfy activateSwitchCondition
        engineConfiguration->launchSpeedThreshold = 0; // to satisfy speedCondition
        Sensor::setMockValue(SensorType::DriverThrottleIntent, 1.7); // to satisfy tpsCondition
    }
}

void SparkSkipRatioTest::updateRpm(const int rpm) {
    Sensor::setMockValue(SensorType::Rpm, rpm);
    // run the ignition math
    engine->periodicFastCallback();
}

TEST_F(SparkSkipRatioTest, raisingRpmToLaunchCondition) {
    doTest(
        /* config = */ {
            /* launchControlEnabled = */ { true },
            /* enableIgnitionCut = */ { true },
            /* satifySwitchSpeedThresholdAndTpsConditions = */ true
        },
        /* testData = */ {
            {
                "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START - 1 (before entering spark skip RPM window)",
                TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START - 1,
                0.0f,
                { LaunchCondition::NotMet }
            },
            /* We've entered spark skip RPM window: */
            {
                "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START (entering spark skip RPM window)",
                TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START,
                TEST_INITIAL_IGNITION_CUT_RATIO,
                { LaunchCondition::PreLaunch }
            },
            {
                "rpm = (TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START + TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END) / 2 (inside spark skip RPM window)",
                (TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START + TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END) / 2,
                (TEST_INITIAL_IGNITION_CUT_RATIO + TEST_FINAL_IGNITION_CUT_RATIO) / 2,
                { LaunchCondition::PreLaunch }
            },
            {
                "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END (leaving spark skip RPM window)",
                TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END,
                TEST_FINAL_IGNITION_CUT_RATIO,
                { LaunchCondition::PreLaunch }
            },
            /* We've left spark skip RPM window: */
            {
                "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END + 1 (after spark skip RPM window)",
                TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END + 1,
                TEST_FINAL_IGNITION_CUT_RATIO,
                { LaunchCondition::PreLaunch }
            },
            {
                "rpm = TEST_LAUNCH_RPM - 1 (before launch)",
                TEST_LAUNCH_RPM - 1,
                TEST_FINAL_IGNITION_CUT_RATIO,
                { LaunchCondition::PreLaunch }
            },
            /* We've reached TEST_LAUNCH_RPM: */
            {
                "rpm = TEST_LAUNCH_RPM (launching)",
                TEST_LAUNCH_RPM,
                1.0f,
                { LaunchCondition::Launch }
            }
        }
    );
}

TEST_F(SparkSkipRatioTest, raisingRpmWithoutLaunchCondition) {
    doTest(
        /* config = */ {
            /* launchControlEnabled = */ { true },
            /* enableIgnitionCut = */ { true },
            /* satifySwitchSpeedThresholdAndTpsConditions = */ false
        },
        /* testData = */ DEFAULT_NO_LAUNCH_SPARK_SKIP_TEST_DATA
    );
}

TEST_F(SparkSkipRatioTest, raisingRpmWithDisabledIgnitionCut) {
    doTest(
        /* config = */ {
            /* launchControlEnabled = */ { true },
            /* enableIgnitionCut = */ { false },
            /* satifySwitchSpeedThresholdAndTpsConditions = */ true
        },
        /* testData = */ {
            {
                "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START - 1 (before entering spark skip RPM window)",
                TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START - 1,
                0.0f,
                { LaunchCondition::NotMet }
            },
            /* We've entered spark skip RPM window: */
            {
                "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START (entering spark skip RPM window)",
                TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START,
                0.0f,
                { LaunchCondition::PreLaunch }
            },
            {
                "rpm = (TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START + TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END) / 2 (inside spark skip RPM window)",
                (TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_START + TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END) / 2,
                0.0f,
                { LaunchCondition::PreLaunch }
            },
            {
                "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END (leaving spark skip RPM window)",
                TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END,
                0.0f,
                { LaunchCondition::PreLaunch }
            },
            /* We've left spark skip RPM window: */
            {
                "rpm = TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END + 1 (after spark skip RPM window)",
                TEST_LAUNCH_SPARK_SKIP_RPM_WINDOW_END + 1,
                0.0f,
                { LaunchCondition::PreLaunch }
            },
            {
                "rpm = TEST_LAUNCH_RPM - 1 (before launch)",
                TEST_LAUNCH_RPM - 1,
                0.0f,
                { LaunchCondition::PreLaunch }
            },
            /* We've reached TEST_LAUNCH_RPM: */
            {
                "rpm = TEST_LAUNCH_RPM (launch condition is still not satisfied)",
                TEST_LAUNCH_RPM,
                0.0f,
                { LaunchCondition::Launch }
            }
        }
    );
}

TEST_F(SparkSkipRatioTest, raisingRpmWithDisabledLaunchControl) {
    doTest(
        /* config = */ {
            /* launchControlEnabled = */ { false },
            /* enableIgnitionCut = */ { true },
            /* satifySwitchSpeedThresholdAndTpsConditions = */ true
        },
        /* testData = */ DEFAULT_NO_LAUNCH_SPARK_SKIP_TEST_DATA
    );
}

TEST_F(SparkSkipRatioTest, raisingRpmWithDisabledLaunchControlAndWithoutLaunchCondition) {
    doTest(
        /* config = */ {
            /* launchControlEnabled = */ { false },
            /* enableIgnitionCut = */ { true },
            /* satifySwitchSpeedThresholdAndTpsConditions = */ false
        },
        /* testData = */ DEFAULT_NO_LAUNCH_SPARK_SKIP_TEST_DATA
    );
}

TEST_F(SparkSkipRatioTest, raisingRpmWithDisabledLaunchControlAndDisabledIgnitionCut) {
    doTest(
        /* config = */ {
            /* launchControlEnabled = */ { false },
            /* enableIgnitionCut = */ { false },
            /* satifySwitchSpeedThresholdAndTpsConditions = */ true
        },
        /* testData = */ DEFAULT_NO_LAUNCH_SPARK_SKIP_TEST_DATA
    );
}

TEST_F(SparkSkipRatioTest, raisingRpmWithDisabledLaunchControlAndDisabledIgnitionCutAndWithoutLaunchCondition) {
    doTest(
        /* config = */ {
            /* launchControlEnabled = */ { false },
            /* enableIgnitionCut = */ { false },
            /* satifySwitchSpeedThresholdAndTpsConditions = */ false
        },
        /* testData = */ DEFAULT_NO_LAUNCH_SPARK_SKIP_TEST_DATA
    );
}

TEST_F(SparkSkipRatioTest, raisingRpmWithDisabledIgnitionCutAndWithoutLaunchCondition) {
    doTest(
        /* config = */ {
            /* launchControlEnabled = */ { true },
            /* enableIgnitionCut = */ { false },
            /* satifySwitchSpeedThresholdAndTpsConditions = */ false
        },
        /* testData = */ DEFAULT_NO_LAUNCH_SPARK_SKIP_TEST_DATA
    );
}