//
// Created by kifir on 12/1/24.
//

#include "pch.h"

#include "engine_configuration_defaults.h"

#include "util/test_base.h"

namespace {
    struct RpmConditionTestData {
        const std::optional<float> rpm;
        const bool expectedRpmCondition;
        const char* const context;
    };

    class NitrousRpmConditionTest : public TestBase<> {
    protected:
        static constexpr uint16_t TEST_ACTIVATION_RPM = 239;
        static constexpr uint16_t TEST_DEACTIVATION_RPM = 932;
        static constexpr uint16_t TEST_DEACTIVATION_RPM_WINDOW = 17;
        static constexpr uint16_t TEST_DEACTIVATION_RPM_WINDOW_BEGIN
            = TEST_DEACTIVATION_RPM - TEST_DEACTIVATION_RPM_WINDOW;

        static constexpr float BEFORE_TEST_ACTIVATION_RPM = TEST_ACTIVATION_RPM - EPS5D;
        static constexpr float BEFORE_TEST_DEACTIVATION_RPM_WINDOW_BEGIN = TEST_DEACTIVATION_RPM_WINDOW_BEGIN - EPS4D;
        static constexpr float BEFORE_TEST_DEACTIVATION_RPM = TEST_DEACTIVATION_RPM - EPS4D;

        static constexpr uint16_t DEFAULT_ACTIVATION_RPM = engine_configuration_defaults::NITROUS_ACTIVATION_RPM;
        static constexpr uint16_t DEFAULT_DEACTIVATION_RPM = engine_configuration_defaults::NITROUS_DEACTIVATION_RPM;
        static constexpr uint16_t DEFAULT_DEACTIVATION_RPM_WINDOW =
            engine_configuration_defaults::NITROUS_DEACTIVATION_RPM_WINDOW;
        static constexpr uint16_t DEFAULT_DEACTIVATION_RPM_WINDOW_BEGIN =
            DEFAULT_DEACTIVATION_RPM - DEFAULT_DEACTIVATION_RPM_WINDOW;

        static constexpr float BEFORE_DEFAULT_ACTIVATION_RPM = DEFAULT_ACTIVATION_RPM - EPS3D;
        static constexpr float BEFORE_DEFAULT_DEACTIVATION_RPM_WINDOW_BEGIN =
            DEFAULT_DEACTIVATION_RPM_WINDOW_BEGIN - EPS3D;
        static constexpr float BEFORE_DEFAULT_DEACTIVATION_RPM = DEFAULT_DEACTIVATION_RPM - EPS3D;

        void checkRpmCondition(const std::vector<RpmConditionTestData>& testData);
        void checkRpmConditionIsAlwaysUnsatisfied();
    };

    void NitrousRpmConditionTest::checkRpmCondition(const std::vector<RpmConditionTestData>& testData) {
        for (const RpmConditionTestData& item: testData) {
            updateRpm(item.rpm, &TestBase::periodicSlowCallback);
            EXPECT_EQ(getModule<NitrousController>().isNitrousRpmCondition, item.expectedRpmCondition)
                << item.context;
        }
    }

    void NitrousRpmConditionTest::checkRpmConditionIsAlwaysUnsatisfied() {
        checkRpmCondition({
            { {0}, false, "rpm = 0" },
            { { BEFORE_TEST_ACTIVATION_RPM }, false, "rpm = BEFORE_TEST_ACTIVATION_RPM" },
            { { TEST_ACTIVATION_RPM }, false, "rpm = TEST_ACTIVATION_RPM" },
            { { BEFORE_TEST_ACTIVATION_RPM }, false, "rpm = BEFORE_TEST_ACTIVATION_RPM" },
            { { TEST_DEACTIVATION_RPM }, false, "rpm = TEST_DEACTIVATION_RPM" },
            { { TEST_DEACTIVATION_RPM + 1 }, false, "rpm = TEST_DEACTIVATION_RPM + 1" },
            { { DEFAULT_ACTIVATION_RPM }, false, "rpm = DEFAULT_ACTIVATION_RPM" },
            { { DEFAULT_DEACTIVATION_RPM }, false, "rpm = DEFAULT_DEACTIVATION_RPM" },
            { { TEST_DEACTIVATION_RPM_WINDOW_BEGIN }, false, "rpm = TEST_DEACTIVATION_RPM_WINDOW_BEGIN" },
            { { BEFORE_TEST_DEACTIVATION_RPM_WINDOW_BEGIN }, false, "rpm = BEFORE_TEST_DEACTIVATION_RPM_WINDOW_BEGIN" },
            { { TEST_DEACTIVATION_RPM_WINDOW_BEGIN }, false, "rpm = TEST_DEACTIVATION_RPM_WINDOW_BEGIN (again)" },
            { { BEFORE_TEST_DEACTIVATION_RPM }, false, "rpm = BEFORE_TEST_DEACTIVATION_RPM (again)" },
            { { TEST_DEACTIVATION_RPM }, false, "rpm = TEST_DEACTIVATION_RPM (again)" },
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

        checkRpmCondition({
            { { 0 }, false, "rpm = 0" },
            { { BEFORE_DEFAULT_ACTIVATION_RPM }, false, "rpm = BEFORE_DEFAULT_ACTIVATION_RPM" },
            { { DEFAULT_ACTIVATION_RPM }, true, "rpm = DEFAULT_ACTIVATION_RPM" },
            { { BEFORE_DEFAULT_DEACTIVATION_RPM }, true, "rpm = BEFORE_DEFAULT_DEACTIVATION_RPM" },
            { { DEFAULT_DEACTIVATION_RPM }, false, "rpm = DEFAULT_DEACTIVATION_RPM" },
            {
                { DEFAULT_DEACTIVATION_RPM_WINDOW_BEGIN },
                false,
                "rpm = DEFAULT_DEACTIVATION_RPM_WINDOW_BEGIN (still in window)"
            },
            {
                { BEFORE_DEFAULT_DEACTIVATION_RPM_WINDOW_BEGIN },
                true,
                "rpm = BEFORE_DEFAULT_DEACTIVATION_RPM_WINDOW_BEGIN"
            },
            {
                { DEFAULT_DEACTIVATION_RPM_WINDOW_BEGIN },
                true,
                "rpm = DEFAULT_DEACTIVATION_RPM_WINDOW_BEGIN (returning to the window)"
            },
            { { BEFORE_DEFAULT_DEACTIVATION_RPM }, true, "rpm = BEFORE_DEFAULT_DEACTIVATION_RPM (again)" },
            { { DEFAULT_DEACTIVATION_RPM }, false, "rpm = NITROUS_DEACTIVATION_RPM (again)" },
        });
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
            { { BEFORE_TEST_ACTIVATION_RPM }, false, "rpm = BEFORE_TEST_ACTIVATION_RPM" },
            { { TEST_ACTIVATION_RPM }, true, "rpm = TEST_ACTIVATION_RPM" },
            { { BEFORE_TEST_DEACTIVATION_RPM }, true, "rpm = BEFORE_TEST_DEACTIVATION_RPM" },
            { { TEST_DEACTIVATION_RPM }, false, "rpm = TEST_DEACTIVATION_RPM" },
            {
                { TEST_DEACTIVATION_RPM_WINDOW_BEGIN },
                false,
                "rpm = TEST_DEACTIVATION_RPM_WINDOW_BEGIN (still in window)"
            },
            { { BEFORE_TEST_DEACTIVATION_RPM_WINDOW_BEGIN }, true, "rpm = BEFORE_TEST_DEACTIVATION_RPM_WINDOW_BEGIN" },
            {
                { TEST_DEACTIVATION_RPM_WINDOW_BEGIN },
                true,
                "rpm = TEST_DEACTIVATION_RPM_WINDOW_BEGIN (returning to the window)"
            },
            { { BEFORE_TEST_DEACTIVATION_RPM }, true, "rpm = BEFORE_TEST_DEACTIVATION_RPM (again)" },
            { { TEST_DEACTIVATION_RPM }, false, "rpm = TEST_DEACTIVATION_RPM (again)" },
        });
    }
}