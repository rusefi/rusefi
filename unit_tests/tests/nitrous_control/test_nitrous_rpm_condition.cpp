//
// Created by kifir on 12/1/24.
//

#include "pch.h"

#include "util/test_base.h"

namespace {
    struct RpmConditionTestData {
        const std::optional<float> rpm;
        const bool expectedRpmCondition;
        const char* const context;
    };

    class NitrousRpmConditionTest : public TestBase {
    protected:
        static constexpr uint16_t TEST_ACTIVATION_RPM = 239;
        static constexpr uint16_t TEST_DEACTIVATION_RPM = 932;
        static constexpr uint16_t TEST_DEACTIVATION_RPM_WINDOW = 17;

        void checkRpmCondition(const std::vector<RpmConditionTestData>& testData);
        void checkRpmConditionIsAlwaysUnsatisfied();
    };

    void NitrousRpmConditionTest::checkRpmCondition(const std::vector<RpmConditionTestData>& testData) {
        for (const RpmConditionTestData& item: testData) {
            updateRpm(item.rpm);
            EXPECT_EQ(engine->nitrousController.isNitrousRpmConditionSatisfied, item.expectedRpmCondition)
                << item.context;
        }
    }

    void NitrousRpmConditionTest::checkRpmConditionIsAlwaysUnsatisfied() {
        checkRpmCondition({
            { {0}, false, "rpm = 0" },
            { { TEST_ACTIVATION_RPM - 1 }, false, "rpm = TEST_ACTIVATION_RPM - 1" },
            { { TEST_ACTIVATION_RPM }, false, "rpm = TEST_ACTIVATION_RPM" },
            { { TEST_DEACTIVATION_RPM - 1 }, false, "rpm = TEST_DEACTIVATION_RPM - 1" },
            { { TEST_DEACTIVATION_RPM }, false, "rpm = TEST_DEACTIVATION_RPM" },
            {
                { TEST_DEACTIVATION_RPM - TEST_DEACTIVATION_RPM_WINDOW },
                false,
                "rpm = TEST_DEACTIVATION_RPM - TEST_DEACTIVATION_RPM_WINDOW"
            },
            {
                { TEST_DEACTIVATION_RPM - TEST_DEACTIVATION_RPM_WINDOW - 1 },
                false,
                "rpm = TEST_DEACTIVATION_RPM - TEST_DEACTIVATION_RPM_WINDOW - 1"
            },
            {
                { TEST_DEACTIVATION_RPM - TEST_DEACTIVATION_RPM_WINDOW },
                false,
                "rpm = TEST_DEACTIVATION_RPM - TEST_DEACTIVATION_RPM_WINDOW (again)"
            },
            {
                { TEST_DEACTIVATION_RPM - 1 },
                false,
                "rpm = TEST_DEACTIVATION_RPM - 1 (again)"
            },
            {
                { TEST_DEACTIVATION_RPM },
                false,
                "rpm = TEST_DEACTIVATION_RPM (again)"
            },
        });
    }

    TEST_F(NitrousRpmConditionTest, checkDefault) {
        checkRpmConditionIsAlwaysUnsatisfied();
    }

    TEST_F(NitrousRpmConditionTest, checkDefaultWithDisabledNitrousControl) {
        setUpEngineConfiguration(EngineConfig().setNitrousControlEnabled({ false }));
        checkRpmConditionIsAlwaysUnsatisfied();
    }

    TEST_F(NitrousRpmConditionTest, checkDefaultWithEnabledNitrousControl) {
        setUpEngineConfiguration(EngineConfig().setNitrousControlEnabled({ true }));
        checkRpmConditionIsAlwaysUnsatisfied();
    }

    TEST_F(NitrousRpmConditionTest, checkActivationAndDeactivation) {
        setUpEngineConfiguration(
            EngineConfig()
                .setNitrousControlEnabled({ true })
                .setNitrousActivationRpm({ TEST_ACTIVATION_RPM })
                .setNitrousDeactivationRpm({ TEST_DEACTIVATION_RPM })
                .setNitrousDeactivationRpmWindow({ TEST_DEACTIVATION_RPM_WINDOW })
        );

        checkRpmCondition({
            { { 0 }, false, "rpm = 0" },
            { { TEST_ACTIVATION_RPM - 1 }, false, "rpm = TEST_ACTIVATION_RPM - 1" },
            { { TEST_ACTIVATION_RPM }, true, "rpm = TEST_ACTIVATION_RPM" },
            { { TEST_DEACTIVATION_RPM - 1 }, true, "rpm = TEST_DEACTIVATION_RPM - 1" },
            { { TEST_DEACTIVATION_RPM }, false, "rpm = TEST_DEACTIVATION_RPM" },
            {
                { TEST_DEACTIVATION_RPM - TEST_DEACTIVATION_RPM_WINDOW },
                false,
                "rpm = TEST_DEACTIVATION_RPM - TEST_DEACTIVATION_RPM_WINDOW (still in window)"
            },
            {
                { TEST_DEACTIVATION_RPM - TEST_DEACTIVATION_RPM_WINDOW - 1 },
                true,
                "rpm = TEST_DEACTIVATION_RPM - TEST_DEACTIVATION_RPM_WINDOW - 1"
            },
            {
                { TEST_DEACTIVATION_RPM - TEST_DEACTIVATION_RPM_WINDOW },
                true,
                "rpm = TEST_DEACTIVATION_RPM - TEST_DEACTIVATION_RPM_WINDOW (returning to the window)"
            },
            {
                { TEST_DEACTIVATION_RPM - 1 },
                true,
                "rpm = TEST_DEACTIVATION_RPM - 1 (again)"
            },
            {
                { TEST_DEACTIVATION_RPM },
                false,
                "rpm = TEST_DEACTIVATION_RPM (again)"
            },
        });
    }
}