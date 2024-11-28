//
// Created by kifir on 11/28/24.
//

#include "pch.h"

#include "util/test_base.h"

namespace {
    struct CltConditionTestData {
        const std::optional<float> clt;
        const bool expectedCltCondition;
        const char* const context;
    };

    class NitrousCltConditionTest : public TestBase {
    protected:
        static constexpr uint8_t TEST_CLT = 51;

        void checkCltCondition(const std::vector<CltConditionTestData>& testData);
    };

    void NitrousCltConditionTest::checkCltCondition(const std::vector<CltConditionTestData>& testData) {
        for (const CltConditionTestData& item: testData) {
            updateClt(item.clt);
            EXPECT_EQ(engine->nitrousController.isCltConditionSatisfied, item.expectedCltCondition) << item.context;
        }
    }

    TEST_F(NitrousCltConditionTest, checkDefault) {
        checkCltCondition({
            { {}, false, "default" },
            { { 0 }, false, "0.0" },
            { { TEST_CLT - EPS5D }, false, "TEST_CLT - EPS5D" },
            { { TEST_CLT }, false, "TEST_CLT" },
            { { TEST_CLT + EPS5D }, false, "TEST_CLT + EPS5D" },
        });
    }

    TEST_F(NitrousCltConditionTest, checkDefaultWithDisabledNitrousControl) {
        setUpEngineConfiguration(EngineConfig().setNitrousControlEnabled({ false }));
        checkCltCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_CLT - EPS5D }, false, "TEST_CLT - EPS5D" },
            { { TEST_CLT }, false, "TEST_CLT" },
            { { TEST_CLT + EPS5D }, false, "TEST_CLT + EPS5D" },
        });
    }

    TEST_F(NitrousCltConditionTest, checkDefaultWithEnabledNitrousControl) {
        setUpEngineConfiguration(EngineConfig().setNitrousControlEnabled({ true }));
        checkCltCondition({
            { {}, true, "default" },
            { { 0.0f }, true, "0.0" },
            { { TEST_CLT - EPS5D }, true, "TEST_CLT - EPS5D" },
            { { TEST_CLT }, true, "TEST_CLT" },
            { { TEST_CLT + EPS5D }, true, "TEST_CLT + EPS5D" },
        });
    }

    TEST_F(NitrousCltConditionTest, checkZeroMinimumClt) {
        setUpEngineConfiguration(
            EngineConfig()
                .setNitrousControlEnabled({ true })
                .setNitrousMinimumClt(0)
        );
        checkCltCondition({
            { {}, true, "default" },
            { { 0.0f }, true, "0.0" },
            { { TEST_CLT - EPS5D }, true, "TEST_CLT - EPS5D" },
            { { TEST_CLT }, true, "TEST_CLT" },
            { { TEST_CLT + EPS5D }, true, "TEST_CLT + EPS5D" },
        });
    }

    TEST_F(NitrousCltConditionTest, checkMinimumClt) {
        setUpEngineConfiguration(
            EngineConfig()
                .setNitrousControlEnabled({ true })
                .setNitrousMinimumClt({ TEST_CLT })
        );
        checkCltCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_CLT - EPS5D }, false, "TEST_CLT - EPS5D" },
            { { TEST_CLT }, true, "TEST_CLT" },
            { { TEST_CLT + EPS5D }, true, "TEST_CLT + EPS5D" },
        });
    }
}