//
// Created by kifir on 11/4/24.
//

#include "pch.h"

#include "flat_shift_condition_test_base.h"

FlatShiftConditionTestBase::FlatShiftConditionTestBase(
    const int8_t torqueReductionIgnitionCut,
    const float torqueReductionIgnitionRetard
) : m_torqueReductionIgnitionCut(torqueReductionIgnitionCut),
    m_torqueReductionIgnitionRetard(torqueReductionIgnitionRetard) {
}

void FlatShiftConditionTestBase::SetUp() {
    TestBase::SetUp();

    setUpEngineConfiguration(EngineConfig()
        .setTorqueReductionEnabled(true)
        .setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_BUTTON)
        .setTorqueReductionTriggerPin(TEST_TORQUE_REDUCTION_BUTTON_PIN)
        .setLimitTorqueReductionTime(false)
        .setTorqueReductionIgnitionCut(m_torqueReductionIgnitionCut)
        .setTorqueReductionIgnitionRetard(m_torqueReductionIgnitionRetard)
    );
}

void FlatShiftConditionTestBase::satisfyFlatShiftCondition() {
    setMockState(TEST_TORQUE_REDUCTION_BUTTON_PIN, true);
    updateApp(TEST_TORQUE_REDUCTION_ARMING_APP);

    periodicFastCallback();

    EXPECT_TRUE(engine->shiftTorqueReductionController.isFlatShiftConditionSatisfied);
}

void FlatShiftConditionTestBase::unsatisfyFlatShiftCondition() {
    setMockState(TEST_TORQUE_REDUCTION_BUTTON_PIN, false);

    periodicFastCallback();

    EXPECT_FALSE(engine->shiftTorqueReductionController.isFlatShiftConditionSatisfied);
}
