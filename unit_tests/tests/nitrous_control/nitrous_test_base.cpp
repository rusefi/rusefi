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
        .setNitrousMinimumVehicleSpeed({ TEST_MIN_VEHICLE_SPEED })
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

    EXPECT_TRUE(getModule<NitrousController>().isNitrousArmed);
}

void NitrousTestBase::satisfySpeedCondition() {
    updateVehicleSpeed(TEST_MIN_VEHICLE_SPEED, &TestBase::periodicSlowCallback);

    EXPECT_TRUE(getModule<NitrousController>().isNitrousSpeedCondition);
}

void NitrousTestBase::satisfyTpsCondition() {
    updateApp(TEST_MIN_TPS, &TestBase::periodicSlowCallback);

    EXPECT_TRUE(getModule<NitrousController>().isNitrousTpsCondition);
}

void NitrousTestBase::satisfyCltCondition() {
    updateClt(TEST_MIN_CLT, &TestBase::periodicSlowCallback);

    EXPECT_TRUE(getModule<NitrousController>().isNitrousCltCondition);
}

void NitrousTestBase::satisfyMapCondition() {
    updateMap(TEST_MAX_MAP, &TestBase::periodicSlowCallback);

    EXPECT_TRUE(getModule<NitrousController>().isNitrousMapCondition);
}

void NitrousTestBase::satisfyAfrCondition() {
    updateLambda1(TEST_LAMBDA1, &TestBase::periodicSlowCallback);

    EXPECT_TRUE(getModule<NitrousController>().isNitrousAfrCondition);
}

void NitrousTestBase::satisfyRpmCondition() {
    updateRpm(TEST_ACTIVATION_RPM, &TestBase::periodicSlowCallback);

    EXPECT_TRUE(getModule<NitrousController>().isNitrousRpmCondition);
}

void NitrousTestBase::activateNitrousControl() {
    armNitrousControl();
    satisfySpeedCondition();
    satisfyTpsCondition();
    satisfyCltCondition();
    satisfyMapCondition();
    satisfyAfrCondition();
    satisfyRpmCondition();
}

void NitrousTestBase::unarmNitrousControl() {
    setMockState(TEST_NITROUS_CONTROL_ARMING_PIN, false);
    periodicSlowCallback();

    EXPECT_FALSE(getModule<NitrousController>().isNitrousArmed);
}

void NitrousTestBase::unsatisfySpeedCondition() {
    updateVehicleSpeed(TEST_MIN_VEHICLE_SPEED - EPS5D, &TestBase::periodicSlowCallback);

    EXPECT_FALSE(getModule<NitrousController>().isNitrousSpeedCondition);
}

void NitrousTestBase::unsatisfyTpsCondition() {
    updateApp(TEST_MIN_TPS - EPS5D, &TestBase::periodicSlowCallback);

    EXPECT_FALSE(getModule<NitrousController>().isNitrousTpsCondition);
}

void NitrousTestBase::unsatisfyCltCondition() {
    updateClt(TEST_MIN_CLT - EPS5D, &TestBase::periodicSlowCallback);

    EXPECT_FALSE(getModule<NitrousController>().isNitrousCltCondition);
}

void NitrousTestBase::unsatisfyMapCondition() {
    updateMap(TEST_MAX_MAP + EPS5D, &TestBase::periodicSlowCallback);

    EXPECT_FALSE(getModule<NitrousController>().isNitrousMapCondition);
}

void NitrousTestBase::unsatisfyAfrCondition() {
    updateLambda1(TEST_LAMBDA1 + EPS5D, &TestBase::periodicSlowCallback);

    EXPECT_FALSE(getModule<NitrousController>().isNitrousAfrCondition);
}

void NitrousTestBase::unsatisfyRpmCondition() {
    updateRpm(TEST_ACTIVATION_RPM - EPS5D, &TestBase::periodicSlowCallback);

    EXPECT_FALSE(getModule<NitrousController>().isNitrousRpmCondition);
}

void NitrousTestBase::deactivateNitrousControl() {
    unarmNitrousControl();
    unsatisfySpeedCondition();
    unsatisfyTpsCondition();
    unsatisfyCltCondition();
    unsatisfyMapCondition();
    unsatisfyAfrCondition();
    unsatisfyRpmCondition();
}

void NitrousTestBase::checkNitrousCondition(const bool expected, const char* const context) {
    EXPECT_EQ(getModule<NitrousController>().isNitrousCondition, expected) << context;
    EXPECT_EQ(enginePins.nitrousRelay.getLogicValue(), expected) << context;
}
