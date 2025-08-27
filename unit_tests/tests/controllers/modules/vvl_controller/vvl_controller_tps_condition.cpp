/*
 * @file vvl_controller_tps_condition.cpp
 *
 * @date: ago 26, 2025
 * @author FDSoftware, kifir
 */

#include "pch.h"

#include "engine_configuration_defaults.h"

#include "util/test_base.h"

namespace {
    static constexpr int TEST_MIN_TPS = 34;
    struct TpsConditionTestData {
        const std::optional<float> tps;
        const bool expectedTpsCondition;
        const char* const context;
    };

    void checkTpsCondition(const std::vector<TpsConditionTestData>& testData) {
        for (const TpsConditionTestData& item: testData) {
            Sensor::setMockValue(SensorType::DriverThrottleIntent, item.tps.value_or(0.0f));
            engine->module<VvlController>().unmock().onSlowCallback();
            EXPECT_EQ(engine->module<VvlController>().unmock().isVvlTpsCondition, item.expectedTpsCondition)
                << item.context;
        }
    }

    TEST(VvlTpsConditionTest, checkDefaultWithDisabledVvlControl) {
        EngineTestHelper eth(engine_type_e::TEST_ENGINE);
        engineConfiguration->vvlControlEnabled = false;

        checkTpsCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_MIN_TPS - EPS5D }, false, "TEST_MIN_TPS - EPS5D" },
            { { TEST_MIN_TPS }, false, "TEST_MIN_TPS" },
            { { TEST_MIN_TPS + EPS5D }, false, "TEST_MIN_TPS + EPS5D" },
        });
    }

    TEST(VvlTpsConditionTest, checkDefaultWithEnabledNitrousControl) {
        EngineTestHelper eth(engine_type_e::TEST_ENGINE);
        engineConfiguration->vvlControlEnabled = true;
        engineConfiguration->vvlController.minimumTps = TEST_MIN_TPS;

        checkTpsCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_MIN_TPS - EPS5D }, false, "VVL_MINIMUM_TPS - EPS5D" },
            { { TEST_MIN_TPS }, true, "VVL_MINIMUM_TPS" },
            { { TEST_MIN_TPS + EPS5D }, true, "VVL_MINIMUM_TPS + EPS5D" },
        });
    }

    TEST(VvlTpsConditionTest, checkZeroMinimumTps) {
        EngineTestHelper eth(engine_type_e::TEST_ENGINE);
        engineConfiguration->vvlControlEnabled = true;
        engineConfiguration->vvlController.minimumTps = 0;

        checkTpsCondition({
            { {}, true, "default" },
            { { 0.0f }, true, "0.0" },
            { { TEST_MIN_TPS - EPS5D }, true, "TEST_MIN_TPS - EPS5D" },
            { { TEST_MIN_TPS }, true, "TEST_MIN_TPS" },
            { { TEST_MIN_TPS + EPS5D }, true, "TEST_MIN_TPS + EPS5D" },
        });
    }
}