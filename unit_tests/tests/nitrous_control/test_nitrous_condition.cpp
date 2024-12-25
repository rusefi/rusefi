//
// Created by kifir on 12/2/24.
//

#include "pch.h"

#include "nitrous_test_base.h"

namespace {
    class NitrousConditionTest : public NitrousTestBase {
    protected:
        void SetUp() override;
    };

    void NitrousConditionTest::SetUp() {
        TestBase::SetUp();

        setUpTestConfiguration();

        EXPECT_FALSE(getModule<NitrousController>().isNitrousArmed);
        EXPECT_FALSE(getModule<NitrousController>().isNitrousSpeedCondition);
        EXPECT_FALSE(getModule<NitrousController>().isNitrousTpsCondition);
        EXPECT_FALSE(getModule<NitrousController>().isNitrousCltCondition);
        EXPECT_FALSE(getModule<NitrousController>().isNitrousMapCondition);
        EXPECT_FALSE(getModule<NitrousController>().isNitrousAfrCondition);
        EXPECT_FALSE(getModule<NitrousController>().isNitrousRpmCondition);

        checkNitrousCondition(false, "No conditions are satisfied");

        armNitrousControl();
        checkNitrousCondition(false, "Armed condition is satisfied");

        satisfySpeedCondition();
        checkNitrousCondition(false, "Armed + Speed conditions are satisfied");

        satisfyTpsCondition();
        checkNitrousCondition(false, "Armed + Speed + TPS conditions are satisfied");

        satisfyCltCondition();
        checkNitrousCondition(false, "Armed + Speed + TPS + CLT conditions are satisfied");

        satisfyMapCondition();
        checkNitrousCondition(false, "Armed + Speed + TPS + CLT + MAP conditions are satisfied");

        satisfyAfrCondition();
        checkNitrousCondition(false, "Armed + Speed + TPS + CLT + MAP + AFR conditions are satisfied");

        satisfyRpmCondition();
        checkNitrousCondition(
            true,
            "Armed + Speed + TPS + CLT + MAP + AFR + RPM conditions are satisfied"
        );
    }

    TEST_F(NitrousConditionTest, checkWithoutArmedNitrousControl) {
        unarmNitrousControl();
        checkNitrousCondition(false, "Without armed condition");
    }

    TEST_F(NitrousConditionTest, checkWithoutSatisfiedSpeedCondition) {
        unsatisfySpeedCondition();
        checkNitrousCondition(false, "Without speed condition");
    }

    TEST_F(NitrousConditionTest, checkWithoutSatisfiedTpsCondition) {
        unsatisfyTpsCondition();
        checkNitrousCondition(false, "Without TPS condition");
    }

    TEST_F(NitrousConditionTest, checkWithoutSatisfiedCltCondition) {
        unsatisfyCltCondition();
        checkNitrousCondition(false, "Without CLT condition");
    }

    TEST_F(NitrousConditionTest, checkWithoutSatisfiedMapCondition) {
        unsatisfyMapCondition();
        checkNitrousCondition(false, "Without MAP condition");
    }

    TEST_F(NitrousConditionTest, checkWithoutSatisfiedAfrCondition) {
        unsatisfyAfrCondition();
        checkNitrousCondition(false, "Without AFR condition");
    }

    TEST_F(NitrousConditionTest, checkWithoutSatisfiedRpmCondition) {
        unsatisfyRpmCondition();
        checkNitrousCondition(false, "Without RPM condition");
    }
}