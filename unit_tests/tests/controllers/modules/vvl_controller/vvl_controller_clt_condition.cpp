/*
 * @file vvl_controller_clt_condition.cpp
 *
 * @date: ago 26, 2025
 * @author FDSoftware, kifir
 */

#include "pch.h"

#include "engine_configuration_defaults.h"

#include "util/test_base.h"

namespace {
    static constexpr uint8_t TEST_MIN_CLT = 51;
    struct CltConditionTestData {
        const std::optional<float> clt;
        const bool expectedCltCondition;
        const char* const context;
    };

    void checkCltCondition(const std::vector<CltConditionTestData>& testData) {
        for (const CltConditionTestData& item: testData) {
            Sensor::setMockValue(SensorType::Clt, item.clt.value_or(0.0f));
            engine->module<VvlController>().unmock().onSlowCallback();
            EXPECT_EQ(engine->module<VvlController>().unmock().isVvlCltCondition, item.expectedCltCondition)
                << item.context;
        }
    }

     TEST(VvlCltConditionTest, checkDefaultWithDisabledVvlControl) {
        EngineTestHelper eth(engine_type_e::TEST_ENGINE);
        engineConfiguration->vvlControlEnabled = false;

        checkCltCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_MIN_CLT - EPS5D }, false, "TEST_MIN_CLT - EPS5D" },
            { { TEST_MIN_CLT }, false, "TEST_MIN_CLT" },
            { { TEST_MIN_CLT + EPS5D }, false, "TEST_MIN_CLT + EPS5D" },
        });
    }

    TEST(VvlCltConditionTest, checkZeroMinimumClt) {
        EngineTestHelper eth(engine_type_e::TEST_ENGINE);
        engineConfiguration->vvlControlEnabled = true;
        engineConfiguration->vvlController.minimumClt = 0;

        checkCltCondition({
            { {}, true, "default" },
            { { 0.0f }, true, "0.0" },
            { { TEST_MIN_CLT - EPS5D }, true, "TEST_MIN_CLT - EPS5D" },
            { { TEST_MIN_CLT }, true, "TEST_MIN_CLT" },
            { { TEST_MIN_CLT + EPS5D }, true, "TEST_MIN_CLT + EPS5D" },
        });
    }

   TEST(VvlCltConditionTest, checkMinimumClt) {
        EngineTestHelper eth(engine_type_e::TEST_ENGINE);
        engineConfiguration->vvlControlEnabled = true;
        engineConfiguration->vvlController.minimumClt = TEST_MIN_CLT;

        checkCltCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_MIN_CLT - EPS5D }, false, "TEST_MIN_CLT - EPS5D" },
            { { TEST_MIN_CLT }, true, "TEST_MIN_CLT" },
            { { TEST_MIN_CLT + EPS5D }, true, "TEST_MIN_CLT + EPS5D" },
        });
    }
}