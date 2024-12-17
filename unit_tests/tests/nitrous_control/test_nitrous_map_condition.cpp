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

    class NitrousMapConditionTest : public TestBase<> {
    protected:
        static constexpr int TEST_MAX_MAP = 45;

        void checkMapCondition(const std::vector<MapConditionTestData>& testData);
    };

    void NitrousMapConditionTest::checkMapCondition(const std::vector<MapConditionTestData>& testData) {
        for (const MapConditionTestData& item: testData) {
            updateMap(item.map, &TestBase::periodicSlowCallback);
            EXPECT_EQ(getModule<NitrousController>().isNitrousMapCondition, item.expectedMapCondition)
                << item.context;
        }
    }

    TEST_F(NitrousMapConditionTest, checkDefault) {
        checkMapCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_MAX_MAP - EPS5D }, false, "TEST_MAX_MAP - EPS5D" },
            { { TEST_MAX_MAP }, false, "TEST_MAX_MAP" },
            { { TEST_MAX_MAP + EPS5D }, false, "TEST_MAX_MAP + EPS5D" },
        });
    }

    TEST_F(NitrousMapConditionTest, checkDefaultWithDisabledNitrousControl) {
        setUpEngineConfiguration(EngineConfig().setNitrousControlEnabled({ false }));
        checkMapCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_MAX_MAP - EPS5D }, false, "TEST_MAX_MAP - EPS5D" },
            { { TEST_MAX_MAP }, false, "TEST_MAX_MAP" },
            { { TEST_MAX_MAP + EPS5D }, false, "TEST_MAX_MAP + EPS5D" },
        });
    }

    TEST_F(NitrousMapConditionTest, checkDefaultWithEnabledNitrousControl) {
        setUpEngineConfiguration(EngineConfig().setNitrousControlEnabled({ true }));
        checkMapCondition({
            { {}, true, "default" },
            { { 0.0f }, true, "0.0" },
            { { TEST_MAX_MAP - EPS5D }, true, "TEST_MAX_MAP - EPS5D" },
            { { TEST_MAX_MAP }, true, "TEST_MAX_MAP" },
            { { TEST_MAX_MAP + EPS5D }, true, "TEST_MAX_MAP + EPS5D" },
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
            { { TEST_MAX_MAP - EPS5D }, true, "TEST_MAX_MAP - EPS5D" },
            { { TEST_MAX_MAP }, true, "TEST_MAX_MAP" },
            { { TEST_MAX_MAP + EPS5D }, true, "TEST_MAX_MAP + EPS5D" },
        });
    }

    TEST_F(NitrousMapConditionTest, checkMaximumMap) {
        setUpEngineConfiguration(
            EngineConfig()
                .setNitrousControlEnabled({ true })
                .setNitrousMaximumMap({ TEST_MAX_MAP })
        );
        checkMapCondition({
            { {}, false, "default" },
            { { 0.0f }, true, "0.0" },
            { { TEST_MAX_MAP - EPS5D }, true, "TEST_MAX_MAP - EPS5D" },
            { { TEST_MAX_MAP }, true, "TEST_MAX_MAP" },
            { { TEST_MAX_MAP + EPS5D }, false, "TEST_MAX_MAP + EPS5D" },
        });
    }
}