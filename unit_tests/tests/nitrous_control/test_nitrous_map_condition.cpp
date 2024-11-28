//
// Created by kifir on 11/28/24.
//

#include "pch.h"

#include "util/test_base.h"

namespace {
    struct MapConditionTestData {
        const std::optional<float> map;
        const bool expectedMapCondition;
        const char* const context;
    };

    class NitrousMapConditionTest : public TestBase {
    protected:
        static constexpr int TEST_MAP = 45;

        void checkMapCondition(const std::vector<MapConditionTestData>& testData);
    };

    void NitrousMapConditionTest::checkMapCondition(const std::vector<MapConditionTestData>& testData) {
        for (const MapConditionTestData& item: testData) {
            updateMap(item.map);
            EXPECT_EQ(engine->nitrousController.isMapConditionSatisfied, item.expectedMapCondition) << item.context;
        }
    }

    TEST_F(NitrousMapConditionTest, checkDefault) {
        checkMapCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_MAP - EPS5D }, false, "TEST_MAP - EPS5D" },
            { { TEST_MAP }, false, "TEST_MAP" },
            { { TEST_MAP + EPS5D }, false, "TEST_MAP + EPS5D" },
        });
    }

    TEST_F(NitrousMapConditionTest, checkDefaultWithDisabledNitrousControl) {
        setUpEngineConfiguration(EngineConfig().setNitrousControlEnabled({ false }));
        checkMapCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_MAP - EPS5D }, false, "TEST_MAP - EPS5D" },
            { { TEST_MAP }, false, "TEST_MAP" },
            { { TEST_MAP + EPS5D }, false, "TEST_MAP + EPS5D" },
        });
    }

    TEST_F(NitrousMapConditionTest, checkDefaultWithEnabledNitrousControl) {
        setUpEngineConfiguration(EngineConfig().setNitrousControlEnabled({ true }));
        checkMapCondition({
            { {}, true, "default" },
            { { 0.0f }, true, "0.0" },
            { { TEST_MAP - EPS5D }, true, "TEST_MAP - EPS5D" },
            { { TEST_MAP }, true, "TEST_MAP" },
            { { TEST_MAP + EPS5D }, true, "TEST_MAP + EPS5D" },
        });
    }

    TEST_F(NitrousMapConditionTest, checkZeroMaximumMap) {
        setUpEngineConfiguration(
            EngineConfig()
                .setNitrousControlEnabled({ true })
                .setNitrousMaximumMap(0.0f)
        );
        checkMapCondition({
            { {}, true, "default" },
            { { 0.0f }, true, "0.0" },
            { { TEST_MAP - EPS5D }, true, "TEST_MAp - EPS5D" },
            { { TEST_MAP }, true, "TEST_MAP" },
            { { TEST_MAP + EPS5D }, true, "TEST_MAP + EPS5D" },
        });
    }

    TEST_F(NitrousMapConditionTest, checkMaximumMap) {
        setUpEngineConfiguration(
            EngineConfig()
                .setNitrousControlEnabled({ true })
                .setNitrousMaximumMap({ TEST_MAP })
        );
        checkMapCondition({
            //{ {}, false, "default" },
            { { 0.0f }, true, "0.0" },
            //{ { TEST_MAP - EPS5D }, true, "TEST_MAP - EPS5D" },
            //{ { TEST_MAP }, false, "TEST_MAP" },
            //{ { TEST_MAP + EPS5D }, false, "TEST_MAP + EPS5D" },
        });
    }
}