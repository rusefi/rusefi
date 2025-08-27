/*
 * @file vvl_controller_afr_condition.cpp
 *
 * @date: ago 27, 2025
 * @author FDSoftware, kifir
 */

#include "pch.h"

#include "engine_configuration_defaults.h"

#include "util/test_base.h"

namespace {
    static constexpr float TEST_DEFAULT_LAMBDA1 = engine_configuration_defaults::NITROUS_MAXIMUM_AFR / STOICH_RATIO;

    static constexpr float TEST_MAXIMUM_AFR = 12.3;
    static constexpr float TEST_LAMBDA1 = TEST_MAXIMUM_AFR / STOICH_RATIO;

    struct AfrConditionTestData {
        const std::optional<float> lambda1;
        const bool expectedAfrCondition;
        const char* const context;
    };

    void checkAfrCondition(const std::vector<AfrConditionTestData>& testData) {
        for (const AfrConditionTestData& item: testData) {
            Sensor::setMockValue(SensorType::Lambda1, item.lambda1.value_or(0.0f));
            engine->module<VvlController>().unmock().onSlowCallback();

             EXPECT_EQ(engine->module<VvlController>().unmock().isVvlAfrCondition, item.expectedAfrCondition)
                << item.context;
        }
    }

    TEST(VvlAfrConditionTest, checkDefaultWithDisabledVvlControl) {
        EngineTestHelper eth(engine_type_e::TEST_ENGINE);
        engineConfiguration->vvlControlEnabled = false;

        checkAfrCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_LAMBDA1 - EPS5D }, false, "TEST_LAMBDA1 - EPS5D" },
            { { TEST_LAMBDA1 }, false, "TEST_LAMBDA1" },
            { { TEST_LAMBDA1 + EPS5D }, false, "TEST_LAMBDA1 + EPS5D" },
        });
    }

    TEST(VvlAfrConditionTest, checkZeroMaximumAfr) {
        EngineTestHelper eth(engine_type_e::TEST_ENGINE);
        engineConfiguration->vvlControlEnabled = true;

        checkAfrCondition({
            { { 0.0f }, true, "0.0" },
            { { TEST_LAMBDA1 - EPS5D }, true, "TEST_LAMBDA1 - EPS5D" },
            { { TEST_LAMBDA1 }, true, "TEST_LAMBDA1" },
            { { TEST_LAMBDA1 + EPS5D }, true, "TEST_LAMBDA1 + EPS5D" },
        });
    }

    TEST(VvlAfrConditionTest, checkMaximumAfr) {
        EngineTestHelper eth(engine_type_e::TEST_ENGINE);
        engineConfiguration->vvlControlEnabled = true;
        engineConfiguration->vvlController.maximumAfr = TEST_MAXIMUM_AFR;

        checkAfrCondition({
            { { 0.0f }, true, "0.0" },
            { { TEST_LAMBDA1 - EPS5D }, true, "TEST_LAMBDA1 - EPS5D" },
            { { TEST_LAMBDA1 }, true, "TEST_LAMBDA1" },
            { { TEST_LAMBDA1 + EPS5D }, false, "TEST_LAMBDA1 + EPS5D" },
        });
    }
}