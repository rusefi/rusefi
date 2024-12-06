//
// Created by kifir on 12/5/24.
//

#include "pch.h"

#include "nitrous_test_base.h"

void NitrousTestBase::setUpTestConfiguration(
    const std::optional<int8_t> nitrousFuelAdderPercent,
    const std::optional<float> nitrousIgnitionRetard
) {
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
        .setNitrousFuelAdderPercent(nitrousFuelAdderPercent)
        .setNitrousIgnitionRetard(nitrousIgnitionRetard)
    );
}

void NitrousTestBase::armNitrousControl() {
    setMockState(TEST_NITROUS_CONTROL_ARMING_PIN, true);
    periodicSlowCallback();

    EXPECT_TRUE(getModule<NitrousController>().isArmed);
}

void NitrousTestBase::satisfyTpsCondition() {
    updateApp(TEST_MIN_TPS, &TestBase::periodicSlowCallback);

    EXPECT_TRUE(getModule<NitrousController>().isTpsConditionSatisfied);
}

void NitrousTestBase::satisfyCltCondition() {
    updateClt(TEST_MIN_CLT, &TestBase::periodicSlowCallback);

    EXPECT_TRUE(getModule<NitrousController>().isCltConditionSatisfied);
}

void NitrousTestBase::satisfyMapCondition() {
    updateMap(TEST_MAX_MAP, &TestBase::periodicSlowCallback);

    EXPECT_TRUE(getModule<NitrousController>().isMapConditionSatisfied);
}

void NitrousTestBase::satisfyAfrCondition() {
    updateLambda1(TEST_LAMBDA1, &TestBase::periodicSlowCallback);

    EXPECT_TRUE(getModule<NitrousController>().isAfrConditionSatisfied);
}

void NitrousTestBase::satisfyRpmCondition() {
    updateRpm(TEST_ACTIVATION_RPM, &TestBase::periodicSlowCallback);

    EXPECT_TRUE(getModule<NitrousController>().isNitrousRpmConditionSatisfied);
}

void NitrousTestBase::activateNitrousControl() {
    armNitrousControl();
    satisfyTpsCondition();
    satisfyCltCondition();
    satisfyMapCondition();
    satisfyAfrCondition();
    satisfyRpmCondition();
}

void NitrousTestBase::unarmNitrousControl() {
    setMockState(TEST_NITROUS_CONTROL_ARMING_PIN, false);
    periodicSlowCallback();

    EXPECT_FALSE(getModule<NitrousController>().isArmed);
}

void NitrousTestBase::unsatisfyTpsCondition() {
    updateApp(TEST_MIN_TPS - EPS5D, &TestBase::periodicSlowCallback);

    EXPECT_FALSE(getModule<NitrousController>().isTpsConditionSatisfied);
}

void NitrousTestBase::unsatisfyCltCondition() {
    updateClt(TEST_MIN_CLT - EPS5D, &TestBase::periodicSlowCallback);

    EXPECT_FALSE(getModule<NitrousController>().isCltConditionSatisfied);
}

void NitrousTestBase::unsatisfyMapCondition() {
    updateMap(TEST_MAX_MAP + EPS5D, &TestBase::periodicSlowCallback);

    EXPECT_FALSE(getModule<NitrousController>().isMapConditionSatisfied);
}

void NitrousTestBase::unsatisfyAfrCondition() {
    updateLambda1(TEST_LAMBDA1 + EPS5D, &TestBase::periodicSlowCallback);

    EXPECT_FALSE(getModule<NitrousController>().isAfrConditionSatisfied);
}

void NitrousTestBase::unsatisfyRpmCondition() {
    updateRpm(TEST_ACTIVATION_RPM - EPS5D, &TestBase::periodicSlowCallback);

    EXPECT_FALSE(getModule<NitrousController>().isNitrousRpmConditionSatisfied);
}

void NitrousTestBase::deactivateNitrousControl() {
    unarmNitrousControl();
    unsatisfyTpsCondition();
    unsatisfyCltCondition();
    unsatisfyMapCondition();
    unsatisfyAfrCondition();
    unsatisfyRpmCondition();
}

void NitrousTestBase::checkNitrousCondition(const bool expected, const char* const context) {
    EXPECT_EQ(getModule<NitrousController>().isNitrousConditionSatisfied, expected) << context;
    EXPECT_EQ(enginePins.nitrousRelay.getLogicValue(), expected) << context;
}
