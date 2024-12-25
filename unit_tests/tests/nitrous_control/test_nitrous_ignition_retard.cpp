//
// Created by kifir on 12/6/24.
//

#include "pch.h"

#include "nitrous_test_base.h"

namespace {
    class NitrousIgnitionRetardTest : public NitrousTestBase {
    protected:
        static constexpr float EXPECTED_BASE_IGNITION_ADVANCE = 10.5f;
        static constexpr float TEST_NITROUS_IGNITION_RETARD = 1.78f;

        void checkBaseIgnitionAdvance(float expectedBaseIgnitionAdvance, const char* context);
    };

    void NitrousIgnitionRetardTest::checkBaseIgnitionAdvance(
        const float expectedBaseIgnitionAdvance,
        const char* const context
    ) {
        periodicFastCallback();
        EXPECT_NEAR(expectedBaseIgnitionAdvance, engine->ignitionState.baseIgnitionAdvance, EPS5D) << context;
    }

    TEST_F(NitrousIgnitionRetardTest, checkDefaultIgnitionRetardCorrection) {
        setUpTestConfiguration();
        checkBaseIgnitionAdvance(EXPECTED_BASE_IGNITION_ADVANCE, "Default");

        activateNitrousControl();
        checkBaseIgnitionAdvance(EXPECTED_BASE_IGNITION_ADVANCE, "All conditions are satisfied");

        deactivateNitrousControl();
        checkBaseIgnitionAdvance(EXPECTED_BASE_IGNITION_ADVANCE, "No conditions are satisfied");
    }

    TEST_F(NitrousIgnitionRetardTest, checkZeroIgnitionRetardCorrection) {
        setUpTestConfiguration(/* nitrousFuelAdderPercent = */ {}, /* nitrousIgnitionRetard = */ { 0.0f });
        checkBaseIgnitionAdvance(EXPECTED_BASE_IGNITION_ADVANCE, "Default");

        activateNitrousControl();
        checkBaseIgnitionAdvance(EXPECTED_BASE_IGNITION_ADVANCE, "All conditions are satisfied");

        deactivateNitrousControl();
        checkBaseIgnitionAdvance(EXPECTED_BASE_IGNITION_ADVANCE, "No conditions are satisfied");
    }

    TEST_F(NitrousIgnitionRetardTest, checkIgnitionRetardCorrection) {
        setUpTestConfiguration(
            /* nitrousFuelAdderPercent = */ {},
            /* nitrousIgnitionRetard = */ { TEST_NITROUS_IGNITION_RETARD }
        );
        checkBaseIgnitionAdvance(EXPECTED_BASE_IGNITION_ADVANCE, "Default");

        activateNitrousControl();
        checkBaseIgnitionAdvance(
            EXPECTED_BASE_IGNITION_ADVANCE - TEST_NITROUS_IGNITION_RETARD,
            "All conditions are satisfied"
        );

        deactivateNitrousControl();
        checkBaseIgnitionAdvance(EXPECTED_BASE_IGNITION_ADVANCE, "No conditions are satisfied");
    }
}