//
// Created by kifir on 11/4/24.
//

#pragma once

#include "util/test_base.h"

class FlatShiftConditionTestBase : public TestBase<> {
protected:
    static constexpr switch_input_pin_e TEST_TORQUE_REDUCTION_BUTTON_PIN = Gpio::G10;
    static constexpr float TEST_TORQUE_REDUCTION_ARMING_APP = 7.89;

    FlatShiftConditionTestBase(int8_t torqueReductionIgnitionCut, float m_torqueReductionIgnitionRetard);

    void SetUp() override;

    void satisfyFlatShiftCondition();
    void unsatisfyFlatShiftCondition();
private:
    const int8_t m_torqueReductionIgnitionCut;
    const float m_torqueReductionIgnitionRetard;
};