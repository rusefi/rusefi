//
// Created by kifir on 11/28/24.
//

#include "pch.h"

#include "util/test_base.h"

namespace {
    struct TpsConditionTestData {
        const std::optional<float> tps;
        const bool expectedTpsCondition;
        const char* const context;
    };

    class NitrousTpsConditionTest : public TestBase {
    protected:
        static constexpr int TEST_MIN_TPS = 34;

        void checkTpsCondition(const std::vector<TpsConditionTestData>& testData);
    };

    void NitrousTpsConditionTest::checkTpsCondition(const std::vector<TpsConditionTestData>& testData) {
        for (const TpsConditionTestData& item: testData) {
            updateApp(item.tps);
            EXPECT_EQ(engine->nitrousController.isTpsConditionSatisfied, item.expectedTpsCondition) << item.context;
        }
    }

    TEST_F(NitrousTpsConditionTest, checkDefault) {
        checkTpsCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_MIN_TPS - EPS5D }, false, "TEST_MIN_TPS - EPS5D" },
            { { TEST_MIN_TPS }, false, "TEST_MIN_TPS" },
            { { TEST_MIN_TPS + EPS5D }, false, "TEST_MIN_TPS + EPS5D" },
        });
    }

    TEST_F(NitrousTpsConditionTest, checkDefaultWithDisabledNitrousControl) {
        setUpEngineConfiguration(EngineConfig().setNitrousControlEnabled({ false }));
        checkTpsCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_MIN_TPS - EPS5D }, false, "TEST_MIN_TPS - EPS5D" },
            { { TEST_MIN_TPS }, false, "TEST_MIN_TPS" },
            { { TEST_MIN_TPS + EPS5D }, false, "TEST_MIN_TPS + EPS5D" },
        });
    }

    TEST_F(NitrousTpsConditionTest, checkDefaultWithEnabledNitrousControl) {
        setUpEngineConfiguration(EngineConfig().setNitrousControlEnabled({ true }));
        checkTpsCondition({
            { {}, true, "default" },
            { { 0.0f }, true, "0.0" },
            { { TEST_MIN_TPS - EPS5D }, true, "TEST_MIN_TPS - EPS5D" },
            { { TEST_MIN_TPS }, true, "TEST_MIN_TPS" },
            { { TEST_MIN_TPS + EPS5D }, true, "TEST_MIN_TPS + EPS5D" },
        });
    }

    TEST_F(NitrousTpsConditionTest, checkZeroMinimumTps) {
        setUpEngineConfiguration(
            EngineConfig()
                .setNitrousControlEnabled({ true })
                .setNitrousMinimumTps(0.0f)
        );
        checkTpsCondition({
            { {}, true, "default" },
            { { 0.0f }, true, "0.0" },
            { { TEST_MIN_TPS - EPS5D }, true, "TEST_MIN_TPS - EPS5D" },
            { { TEST_MIN_TPS }, true, "TEST_MIN_TPS" },
            { { TEST_MIN_TPS + EPS5D }, true, "TEST_MIN_TPS + EPS5D" },
        });
    }

    TEST_F(NitrousTpsConditionTest, checkMinimumTps) {
        setUpEngineConfiguration(
            EngineConfig()
                .setNitrousControlEnabled({ true })
                .setNitrousMinimumTps({ TEST_MIN_TPS })
        );
        checkTpsCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_MIN_TPS - EPS5D }, false, "TEST_MIN_TPS - EPS5D" },
            { { TEST_MIN_TPS }, true, "TEST_MIN_TPS" },
            { { TEST_MIN_TPS + EPS5D }, true, "TEST_MIN_TPS + EPS5D" },
        });
    }
}