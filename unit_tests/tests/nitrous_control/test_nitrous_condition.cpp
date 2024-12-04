//
// Created by kifir on 12/2/24.
//

#include "pch.h"

#include "util/test_base.h"

namespace {
    class NitrousConditionTest : public TestBase {
    protected:
        static constexpr switch_input_pin_e TEST_NITROUS_CONTROL_ARMING_PIN = Gpio::A13;
        static constexpr int TEST_MIN_TPS = 34;
        static constexpr uint8_t TEST_MIN_CLT = 51;
        static constexpr int TEST_MAX_MAP = 45;

        static constexpr float TEST_MAXIMUM_AFR = 12.3;
        static constexpr float TEST_LAMBDA1 = TEST_MAXIMUM_AFR / STOICH_RATIO;

        static constexpr uint16_t TEST_ACTIVATION_RPM = 239;
        static constexpr uint16_t TEST_DEACTIVATION_RPM = 932;
        static constexpr uint16_t TEST_DEACTIVATION_RPM_WINDOW = 17;

        void SetUp() override;

        void satisfyAllConditions();
    private:
        void armNitrousControl();
        void satisfyTpsCondition();
        void satisfyCltCondition();
        void satisfyMapCondition();
        void satisfyAfrCondition();
        void satisfyRpmCondition();
    };

    void NitrousConditionTest::SetUp() {
        TestBase::SetUp();

        setUpEngineConfiguration(EngineConfig()
            .setNitrousControlEnabled({ true })
            .setNitrousControlArmingMethod({ DIGITAL_SWITCH_INPUT })
            .setNitrousControlTriggerPin({ TEST_NITROUS_CONTROL_ARMING_PIN })
            .setNitrousMinimumTps({ TEST_MIN_TPS })
            .setNitrousMinimumClt({ TEST_MIN_CLT })
            .setNitrousMaximumMap({ TEST_MAX_MAP })
            .setNitrousMaximumAfr({ TEST_MAXIMUM_AFR })
            .setNitrousActivationRpm({ TEST_ACTIVATION_RPM })
            .setNitrousDeactivationRpm({ TEST_DEACTIVATION_RPM })
            .setNitrousDeactivationRpmWindow({ TEST_DEACTIVATION_RPM_WINDOW })
        );

        satisfyAllConditions();
    }

    void NitrousConditionTest::satisfyAllConditions() {
        armNitrousControl();
        satisfyTpsCondition();
        satisfyCltCondition();
        satisfyMapCondition();
        satisfyAfrCondition();
        satisfyRpmCondition();

        EXPECT_TRUE(getModule<NitrousController>().isNitrousRpmConditionSatisfied);
    }

    void NitrousConditionTest::armNitrousControl() {
        setMockState(TEST_NITROUS_CONTROL_ARMING_PIN, true);
        periodicSlowCallback();

        EXPECT_TRUE(getModule<NitrousController>().isArmed);
    }

    void NitrousConditionTest::satisfyTpsCondition() {
        updateApp(TEST_MIN_TPS, &TestBase::periodicSlowCallback);

        EXPECT_TRUE(getModule<NitrousController>().isTpsConditionSatisfied);
    }

    void NitrousConditionTest::satisfyCltCondition() {
        updateClt(TEST_MIN_CLT, &TestBase::periodicSlowCallback);

        EXPECT_TRUE(getModule<NitrousController>().isCltConditionSatisfied);
    }

    void NitrousConditionTest::satisfyMapCondition() {
        updateMap(TEST_MAX_MAP, &TestBase::periodicSlowCallback);

        EXPECT_TRUE(getModule<NitrousController>().isMapConditionSatisfied);
    }

    void NitrousConditionTest::satisfyAfrCondition() {
        updateLambda1(TEST_LAMBDA1, &TestBase::periodicSlowCallback);

        EXPECT_TRUE(getModule<NitrousController>().isAfrConditionSatisfied);
    }

    void NitrousConditionTest::satisfyRpmCondition() {
        updateRpm(TEST_ACTIVATION_RPM, &TestBase::periodicSlowCallback);

        EXPECT_TRUE(getModule<NitrousController>().isNitrousRpmConditionSatisfied);
    }

    TEST_F(NitrousConditionTest, checkWithoutArmedNitrousControl) {
        setMockState(TEST_NITROUS_CONTROL_ARMING_PIN, false);
        periodicSlowCallback();

        EXPECT_FALSE(getModule<NitrousController>().isArmed);
        EXPECT_FALSE(getModule<NitrousController>().isNitrousConditionSatisfied);
    }

    TEST_F(NitrousConditionTest, checkWithoutSatisfiedTpsCondition) {
        updateApp(TEST_MIN_TPS - EPS5D, &TestBase::periodicSlowCallback);

        EXPECT_FALSE(getModule<NitrousController>().isTpsConditionSatisfied);
        EXPECT_FALSE(getModule<NitrousController>().isNitrousConditionSatisfied);
    }

    TEST_F(NitrousConditionTest, checkWithoutSatisfiedCltCondition) {
        updateClt(TEST_MIN_CLT - EPS5D, &TestBase::periodicSlowCallback);

        EXPECT_FALSE(getModule<NitrousController>().isCltConditionSatisfied);
        EXPECT_FALSE(getModule<NitrousController>().isNitrousConditionSatisfied);
    }

    TEST_F(NitrousConditionTest, checkWithoutSatisfiedMapCondition) {
        updateMap(TEST_MAX_MAP + EPS5D, &TestBase::periodicSlowCallback);

        EXPECT_FALSE(getModule<NitrousController>().isMapConditionSatisfied);
        EXPECT_FALSE(getModule<NitrousController>().isNitrousConditionSatisfied);
    }

    TEST_F(NitrousConditionTest, checkWithoutSatisfiedAfrCondition) {
        updateLambda1(TEST_LAMBDA1 + EPS5D, &TestBase::periodicSlowCallback);

        EXPECT_FALSE(getModule<NitrousController>().isAfrConditionSatisfied);
        EXPECT_FALSE(getModule<NitrousController>().isNitrousConditionSatisfied);
    }

    TEST_F(NitrousConditionTest, checkWithoutSatisfiedRpmCondition) {
        updateRpm(TEST_ACTIVATION_RPM - EPS5D, &TestBase::periodicSlowCallback);

        EXPECT_FALSE(getModule<NitrousController>().isNitrousRpmConditionSatisfied);
        EXPECT_FALSE(getModule<NitrousController>().isNitrousConditionSatisfied);
    }
}