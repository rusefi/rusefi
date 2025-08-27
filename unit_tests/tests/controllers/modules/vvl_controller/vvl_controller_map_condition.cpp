/*
 * @file vvl_controller_map_condition.cpp
 *
 * @date: ago 26, 2025
 * @author FDSoftware, kifir
 */

#include "pch.h"

#include "util/test_base.h"

namespace {
    static constexpr int TEST_MAX_MAP = 45;
    struct MapConditionTestData {
        const std::optional<float> map;
        const bool expectedMapCondition;
        const char* const context;
    };

    void checkMapCondition(const std::vector<MapConditionTestData>& testData) {
        for (const MapConditionTestData& item: testData) {
            Sensor::setMockValue(SensorType::Map, item.map.value_or(0.0f));
            engine->module<VvlController>().unmock().onSlowCallback();

            EXPECT_EQ(engine->module<VvlController>().unmock().isVvlMapCondition, item.expectedMapCondition)
                << item.context;
        }
    }

    TEST(VvlMapConditionTest, checkDefaultWithDisabledVvlControl) {
        EngineTestHelper eth(engine_type_e::TEST_ENGINE);
        engineConfiguration->vvlControlEnabled = false;

        checkMapCondition({
            { { 0.0f }, false, "0.0" },
            { { TEST_MAX_MAP - EPS5D }, false, "TEST_MAX_MAP - EPS5D" },
            { { TEST_MAX_MAP }, false, "TEST_MAX_MAP" },
            { { TEST_MAX_MAP + EPS5D }, false, "TEST_MAX_MAP + EPS5D" },
        });
    }

    TEST(VvlMapConditionTest, checkZeroMaximumMap) {
        EngineTestHelper eth(engine_type_e::TEST_ENGINE);
        engineConfiguration->vvlControlEnabled = true;

        checkMapCondition({
            { { 0.0f }, true, "0.0" },
            { { TEST_MAX_MAP - EPS5D }, true, "TEST_MAX_MAP - EPS5D" },
            { { TEST_MAX_MAP }, true, "TEST_MAX_MAP" },
            { { TEST_MAX_MAP + EPS5D }, true, "TEST_MAX_MAP + EPS5D" },
        });
    }

    TEST(VvlMapConditionTest, checkMaximumMap) {
        EngineTestHelper eth(engine_type_e::TEST_ENGINE);
        engineConfiguration->vvlControlEnabled = true;
        engineConfiguration->vvlController.maximumMap = TEST_MAX_MAP;

        checkMapCondition({
            { { 0.0f }, true, "0.0" },
            { { TEST_MAX_MAP - EPS5D }, true, "TEST_MAX_MAP - EPS5D" },
            { { TEST_MAX_MAP }, true, "TEST_MAX_MAP" },
            { { TEST_MAX_MAP + EPS5D }, false, "TEST_MAX_MAP + EPS5D" },
        });
    }
}