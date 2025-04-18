//
// Created by kifir on 11/5/24.
//

#include "pch.h"

#include "flat_shift_condition_test_base.h"

constexpr float TEST_IGNITION_RETARD = 17.4;
constexpr float TEST_TORQUE_REDUCTION_IGNITION_RETARD = 23.0f;

namespace {
    class ShiftTorqueReductionAngleAdvanceTest : public FlatShiftConditionTestBase {
    protected:
        ShiftTorqueReductionAngleAdvanceTest();

        void SetUp() override;

        void checkBaseIgnitionAdvance(const char* context, float expectedAdvance);
    private:
        void configureTestIgnitionTable();
    };

    ShiftTorqueReductionAngleAdvanceTest::ShiftTorqueReductionAngleAdvanceTest()
    : FlatShiftConditionTestBase(0, TEST_TORQUE_REDUCTION_IGNITION_RETARD) {
    }

    void ShiftTorqueReductionAngleAdvanceTest::SetUp() {
        FlatShiftConditionTestBase::SetUp();
        configureTestIgnitionTable();
        setTable(config->torqueReductionIgnitionRetardTable, TEST_TORQUE_REDUCTION_IGNITION_RETARD);
    }

    void ShiftTorqueReductionAngleAdvanceTest::checkBaseIgnitionAdvance(
        const char* const context,
        const float expectedAdvance
    ) {
        periodicFastCallback();
        EXPECT_NEAR(expectedAdvance, engine->ignitionState.baseIgnitionAdvance, EPS5D) << context;
    }

    void ShiftTorqueReductionAngleAdvanceTest::configureTestIgnitionTable() {
        IgnitionTable testIgnitionTable;
        for (IgnitionTable::size_type loadIdx = 0; loadIdx < testIgnitionTable.size(); loadIdx++) {
            const IgnitionTable::value_type& rpms = testIgnitionTable[loadIdx];
            for (IgnitionTable::size_type rpmIdx = 0; rpmIdx < rpms.size(); rpmIdx++) {
                testIgnitionTable[loadIdx][rpmIdx] = TEST_IGNITION_RETARD;
            }
        };
        getTestPersistentConfiguration().setIgnitionTable(testIgnitionTable);
    }

    TEST_F(ShiftTorqueReductionAngleAdvanceTest, checkAngleAdvance) {
        checkBaseIgnitionAdvance("Initial state", TEST_IGNITION_RETARD);

        satisfyFlatShiftCondition();
        checkBaseIgnitionAdvance("Flat Shift condition is satisfied", TEST_TORQUE_REDUCTION_IGNITION_RETARD);

        unsatisfyFlatShiftCondition();
        checkBaseIgnitionAdvance("Flat Shift condition is not satisfied", TEST_IGNITION_RETARD);
    }
}