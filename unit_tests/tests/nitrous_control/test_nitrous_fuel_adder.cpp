//
// Created by kifir on 12/5/24.
//

#include "pch.h"

#include "nitrous_test_base.h"

namespace {
    class NitrousFuelAdderTest : public NitrousTestBase {
    protected:
        static constexpr int8_t TEST_NITROUS_FUEL_ADDER_PERCENT = 17;
        static constexpr float DEFAULT_FUEL_CORRECTION = 1.009999f;
        static const CltFuelCorrCurve TEST_CLT_FUEL_CORR;

        void checkTotalFuelCorrection(
            bool expectedNitrousCondition,
            float expectedTotalFuelCorrection,
            const char* context
        );
    public:
        static CltFuelCorrCurve generateTestCltFuelCorrCurve();
    };

    void NitrousFuelAdderTest::checkTotalFuelCorrection(
        const bool expectedNitrousCondition,
        const float expectedTotalFuelCorrection,
        const char* const context
    ) {
        periodicFastCallback(); // to recalculate total fuel correction
        checkNitrousCondition(expectedNitrousCondition, context);
        EXPECT_NEAR(engine->fuelComputer.totalFuelCorrection, expectedTotalFuelCorrection, EPS5D) << context;
    }

    CltFuelCorrCurve NitrousFuelAdderTest::generateTestCltFuelCorrCurve() {
        CltFuelCorrCurve result;
        result.fill(DEFAULT_FUEL_CORRECTION);
        return result;
    }

    const CltFuelCorrCurve NitrousFuelAdderTest::TEST_CLT_FUEL_CORR = NitrousFuelAdderTest::generateTestCltFuelCorrCurve();

    TEST_F(NitrousFuelAdderTest, checkFuelCorrection) {
        setUpTestConfiguration({ TEST_NITROUS_FUEL_ADDER_PERCENT });
        getTestPersistentConfiguration().setCltFuelCorrCurve(TEST_CLT_FUEL_CORR); // constant fuel CLT correction

        checkTotalFuelCorrection(false, DEFAULT_FUEL_CORRECTION, "Default");

        activateNitrousControl();
        checkTotalFuelCorrection(
            true,
            DEFAULT_FUEL_CORRECTION * (1.0f + TEST_NITROUS_FUEL_ADDER_PERCENT / 100.0f),
            "All conditions are satisfied"
        );

        deactivateNitrousControl();
        checkTotalFuelCorrection(false, DEFAULT_FUEL_CORRECTION, "No conditions are satisfied");
    }
}