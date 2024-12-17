//
// Created by kifir on 11/28/24.
//

#include "pch.h"

#include "engine_configuration_defaults.h"

#include "util/test_base.h"

namespace {
    struct AfrConditionTestData {
        const std::optional<float> lambda1;
        const bool expectedAfrCondition;
        const char* const context;
    };

    class NitrousAfrConditionTest : public TestBase<> {
    protected:
        static constexpr float TEST_DEFAULT_LAMBDA1 = engine_configuration_defaults::NITROUS_MAXIMUM_AFR / STOICH_RATIO;

        static constexpr float TEST_MAXIMUM_AFR = 12.3;
        static constexpr float TEST_LAMBDA1 = TEST_MAXIMUM_AFR / STOICH_RATIO;

        void checkAfrCondition(const std::vector<AfrConditionTestData>& testData);
    };

    void NitrousAfrConditionTest::checkAfrCondition(const std::vector<AfrConditionTestData>& testData) {
        for (const AfrConditionTestData& item: testData) {
            updateLambda1(item.lambda1, &TestBase::periodicSlowCallback);
            EXPECT_EQ(getModule<NitrousController>().isNitrousAfrCondition, item.expectedAfrCondition)
                << item.context;
        }
    }

    TEST_F(NitrousAfrConditionTest, checkDefault) {
        checkAfrCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_LAMBDA1 - EPS5D }, false, "TEST_LAMBDA1 - EPS5D" },
            { { TEST_LAMBDA1 }, false, "TEST_LAMBDA1" },
            { { TEST_LAMBDA1 + EPS5D }, false, "TEST_LAMBDA1 + EPS5D" },
        });
    }

    TEST_F(NitrousAfrConditionTest, checkDefaultWithDisabledNitrousControl) {
        setUpEngineConfiguration(EngineConfig().setNitrousControlEnabled({ false }));
        checkAfrCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_LAMBDA1 - EPS5D }, false, "TEST_LAMBDA1 - EPS5D" },
            { { TEST_LAMBDA1 }, false, "TEST_LAMBDA1" },
            { { TEST_LAMBDA1 + EPS5D }, false, "TEST_LAMBDA1 + EPS5D" },
        });
    }

    TEST_F(NitrousAfrConditionTest, checkDefaultWithEnabledNitrousControl) {
        setUpEngineConfiguration(EngineConfig().setNitrousControlEnabled({ true }));
        checkAfrCondition({
            { {}, false, "default" },
            { { 0.0f }, true, "0.0" },
            { { TEST_DEFAULT_LAMBDA1 - EPS5D }, true, "TEST_DEFAULT_LAMBDA1 - EPS5D" },
            { { TEST_DEFAULT_LAMBDA1 }, true, "TEST_DEFAULT_LAMBDA1" },
            { { TEST_DEFAULT_LAMBDA1 + EPS5D }, false, "TEST_DEFAULT_LAMBDA1 + EPS5D" },
        });
    }

    TEST_F(NitrousAfrConditionTest, checkZeroMaximumAfr) {
        setUpEngineConfiguration(
            EngineConfig()
                .setNitrousControlEnabled({ true })
                .setNitrousMaximumAfr(0.0f)
        );
        checkAfrCondition({
            { {}, true, "default" },
            { { 0.0f }, true, "0.0" },
            { { TEST_LAMBDA1 - EPS5D }, true, "TEST_LAMBDA1 - EPS5D" },
            { { TEST_LAMBDA1 }, true, "TEST_LAMBDA1" },
            { { TEST_LAMBDA1 + EPS5D }, true, "TEST_LAMBDA1 + EPS5D" },
        });
    }

    TEST_F(NitrousAfrConditionTest, checkMaximumAfr) {
        setUpEngineConfiguration(
            EngineConfig()
                .setNitrousControlEnabled({ true })
                .setNitrousMaximumAfr({ TEST_MAXIMUM_AFR })
        );
        checkAfrCondition({
            { {}, false, "default" },
            { { 0.0f }, true, "0.0" },
            { { TEST_LAMBDA1 - EPS5D }, true, "TEST_LAMBDA1 - EPS5D" },
            { { TEST_LAMBDA1 }, true, "TEST_LAMBDA1" },
            { { TEST_LAMBDA1 + EPS5D }, false, "TEST_LAMBDA1 + EPS5D" },
        });
    }
}