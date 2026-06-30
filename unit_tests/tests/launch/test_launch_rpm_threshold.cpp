#include "pch.h"

namespace {
    constexpr int TEST_LAUNCH_RPM = 4000;
    constexpr int TEST_THRESHOLD_RPM = 2000;

    void setUpThresholdTest(const int threshold) {
        engineConfiguration->launchControlEnabled = true;
        engineConfiguration->launchActivationMode = LUA_LAUNCH;
        engineConfiguration->launchRpm = TEST_LAUNCH_RPM;
        engineConfiguration->launchRpmWindow = 0;
        engineConfiguration->launchSpeedThreshold = 0;
        engineConfiguration->launchTpsThreshold = 0;
        engineConfiguration->launchRpmThreshold = threshold;
        Sensor::setMockValue(SensorType::DriverThrottleIntent, 5);
    }

    void update(const int rpm, const bool switchPressed) {
        Sensor::setMockValue(SensorType::Rpm, rpm);
        engine->launchController.luaLaunchState = switchPressed;
        engine->launchController.update();
    }

    TEST(launchRpmThreshold, standingLaunchLatches) {
        EngineTestHelper eth(engine_type_e::TEST_ENGINE);
        setUpThresholdTest(TEST_THRESHOLD_RPM);

        update(1500, true);
        EXPECT_TRUE(engine->launchController.isLaunchLatched);

        update(TEST_LAUNCH_RPM, true);
        EXPECT_TRUE(engine->launchController.isLaunchLatched);
        EXPECT_TRUE(engine->launchController.isLaunchCondition);
    }

    TEST(launchRpmThreshold, highRpmBlipDoesNotArm) {
        EngineTestHelper eth(engine_type_e::TEST_ENGINE);
        setUpThresholdTest(TEST_THRESHOLD_RPM);

        update(6000, false);
        update(6000, true);
        EXPECT_FALSE(engine->launchController.isLaunchLatched);
        EXPECT_FALSE(engine->launchController.isLaunchCondition);
    }

    TEST(launchRpmThreshold, zeroThresholdIsLegacy) {
        EngineTestHelper eth(engine_type_e::TEST_ENGINE);
        setUpThresholdTest(0);

        update(6000, true);
        EXPECT_TRUE(engine->launchController.isLaunchLatched);
        EXPECT_TRUE(engine->launchController.isLaunchCondition);
    }
}
