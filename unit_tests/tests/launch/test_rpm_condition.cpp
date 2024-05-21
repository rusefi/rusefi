//
// Created by kifir on 5/21/24.
//

#include "pch.h"

constexpr int TEST_LAUNCH_RPM = 2390;
constexpr int TEST_LAUNCH_RPM_WINDOW = 170;
constexpr int TEST_PRELAUNCH_RPM = TEST_LAUNCH_RPM - TEST_LAUNCH_RPM_WINDOW;

static void setUpTestParameters() {
    engineConfiguration->launchControlEnabled = true;
    engineConfiguration->launchRpm = TEST_LAUNCH_RPM;
    engineConfiguration->launchRpmWindow = TEST_LAUNCH_RPM_WINDOW;
    engineConfiguration->launchSparkCutEnable = true;
}

static void updateRpm(const int rpm) {
    Sensor::setMockValue(SensorType::Rpm, rpm);
    engine->launchController.update();
}

TEST(rpmCondition, IncreasingRpm) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    setUpTestParameters();

    /* RPM hasn't reached Launch RPM parameter - rpmCondition isn't satisfied: */
    EXPECT_FALSE(engine->launchController.rpmCondition); // check default value
    EXPECT_FALSE(engine->launchController.isAfterLaunch); // check default value

    updateRpm(TEST_PRELAUNCH_RPM - 1);
    EXPECT_FALSE(engine->launchController.rpmCondition);
    EXPECT_FALSE(engine->launchController.isAfterLaunch);

    updateRpm(TEST_PRELAUNCH_RPM);
    EXPECT_FALSE(engine->launchController.rpmCondition);
    EXPECT_FALSE(engine->launchController.isAfterLaunch);

    updateRpm(TEST_LAUNCH_RPM - 1);
    EXPECT_FALSE(engine->launchController.rpmCondition);
    EXPECT_FALSE(engine->launchController.isAfterLaunch);

    updateRpm(TEST_LAUNCH_RPM);
    /* RPM reached Launch RPM parameter - now rpmCondition is satisfied: */
    EXPECT_TRUE(engine->launchController.rpmCondition);
    EXPECT_TRUE(engine->launchController.isAfterLaunch);
}

TEST(rpmCondition, HysterisisOnDecreasingRpm) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    setUpTestParameters();

    updateRpm(TEST_LAUNCH_RPM);
    /* RPM reached Launch RPM parameter - rpmCondition is satisfied: */
    EXPECT_TRUE(engine->launchController.rpmCondition);
    EXPECT_TRUE(engine->launchController.isAfterLaunch);

    updateRpm(TEST_LAUNCH_RPM - 1);
    /* RPM is below Launch RPM parameter, but still in Launch Control Window - so rpmCondition is still satisfied: */
    EXPECT_TRUE(engine->launchController.rpmCondition);
    EXPECT_TRUE(engine->launchController.isAfterLaunch);

    updateRpm(TEST_PRELAUNCH_RPM);
    EXPECT_TRUE(engine->launchController.rpmCondition);
    EXPECT_TRUE(engine->launchController.isAfterLaunch);

    updateRpm(TEST_PRELAUNCH_RPM - 1);
    /* RPM went down below Launch Control Window parameter - now rpmCondition is not satisfied: */
    EXPECT_FALSE(engine->launchController.rpmCondition);
    EXPECT_FALSE(engine->launchController.isAfterLaunch);
}

TEST(rpmCondition, HysterisisRpmOscillation) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    setUpTestParameters();

    updateRpm(TEST_LAUNCH_RPM);
    /* RPM reached Launch RPM parameter - rpmCondition is satisfied: */
    EXPECT_TRUE(engine->launchController.rpmCondition);
    EXPECT_TRUE(engine->launchController.isAfterLaunch);

    updateRpm(TEST_LAUNCH_RPM - 1);
    EXPECT_TRUE(engine->launchController.rpmCondition);
    EXPECT_TRUE(engine->launchController.isAfterLaunch);

    updateRpm(TEST_LAUNCH_RPM + 1);
    EXPECT_TRUE(engine->launchController.rpmCondition);
    EXPECT_TRUE(engine->launchController.isAfterLaunch);

    updateRpm(TEST_PRELAUNCH_RPM);
    EXPECT_TRUE(engine->launchController.rpmCondition);
    EXPECT_TRUE(engine->launchController.isAfterLaunch);

    updateRpm(TEST_LAUNCH_RPM + 2);
    EXPECT_TRUE(engine->launchController.rpmCondition);
    EXPECT_TRUE(engine->launchController.isAfterLaunch);

    updateRpm(TEST_PRELAUNCH_RPM - 1);
    /* RPM went down below Launch Control Window parameter - now rpmCondition is not satisfied: */
    EXPECT_FALSE(engine->launchController.rpmCondition);
    EXPECT_FALSE(engine->launchController.isAfterLaunch);

    updateRpm(TEST_PRELAUNCH_RPM);
    EXPECT_FALSE(engine->launchController.rpmCondition);
    EXPECT_FALSE(engine->launchController.isAfterLaunch);

    updateRpm(TEST_LAUNCH_RPM - 1);
    EXPECT_FALSE(engine->launchController.rpmCondition);
    EXPECT_FALSE(engine->launchController.isAfterLaunch);

    /* RPM reached Launch RPM parameter again - now rpmCondition is satisfied: */
    updateRpm(TEST_LAUNCH_RPM);
    EXPECT_TRUE(engine->launchController.rpmCondition);
    EXPECT_TRUE(engine->launchController.isAfterLaunch);
}