//
// Created by kifir on 11/4/24.
//

#include "pch.h"

#include "flat_shift_condition_test_base.h"

namespace {
	class InvalidFlatShiftTpsSensor final : public Sensor {
	public:
		InvalidFlatShiftTpsSensor() : Sensor(SensorType::Tps1) {}
		~InvalidFlatShiftTpsSensor() { unregister(); }

		SensorResult get() const override {
			SensorResult result = payload;
			result.Valid = false;
			return result;
		}

		void showInfo(const char*) const override {}
		float payload = 0;
	};

    constexpr float TEST_LUA_SOFT_SPARK_SKIP = 1.23f;
    constexpr float TEST_LUA_HARD_SPARK_SKIP = 4.56f;

    constexpr int8_t TEST_TORQUE_REDUCTION_IGNITION_CUT = 17;

    class ShiftTorqueReductionSparkSkipRatioTest : public FlatShiftConditionTestBase {
    protected:
        ShiftTorqueReductionSparkSkipRatioTest();

        void SetUp() override;

        void checkTargetSkipRatios(
            const char* context,
            float expectedHardSparkLimiterTargetSkipRatio = TEST_LUA_HARD_SPARK_SKIP
        );
    };

    ShiftTorqueReductionSparkSkipRatioTest::ShiftTorqueReductionSparkSkipRatioTest()
    : FlatShiftConditionTestBase(TEST_TORQUE_REDUCTION_IGNITION_CUT, 0.0f) {
    }

    void ShiftTorqueReductionSparkSkipRatioTest::SetUp() {
        FlatShiftConditionTestBase::SetUp();

        getTestLuaScriptExecutor().setSparkSkipRatio(TEST_LUA_SOFT_SPARK_SKIP);
        getTestLuaScriptExecutor().setSparkHardSkipRatio(TEST_LUA_HARD_SPARK_SKIP);
    }

    void ShiftTorqueReductionSparkSkipRatioTest::checkTargetSkipRatios(
        const char* const context,
        const float expectedHardSparkLimiterTargetSkipRatio
    ) {
        periodicFastCallback();
        EXPECT_EQ(engine->softSparkLimiter.getTargetSkipRatio(), TEST_LUA_SOFT_SPARK_SKIP) << context;
        EXPECT_EQ(engine->hardSparkLimiter.getTargetSkipRatio(), expectedHardSparkLimiterTargetSkipRatio) << context;
    }

    TEST_F(ShiftTorqueReductionSparkSkipRatioTest, checkHardSparkLimiterTargetSkipRatio) {
        checkTargetSkipRatios("Initial state");

        satisfyFlatShiftCondition();
        checkTargetSkipRatios(
            "Flat Shift condition is satisfied",
            TEST_LUA_HARD_SPARK_SKIP + TEST_TORQUE_REDUCTION_IGNITION_CUT / 100.0f
        );

        unsatisfyFlatShiftCondition();
        checkTargetSkipRatios("Flat Shift condition is not satisfied");
    }

	TEST(ShiftTorqueReduction, InvalidFlatShiftAxisPayloadCannotChangeSparkCutOrTiming) {
		EngineTestHelper eth(engine_type_e::TEST_ENGINE);
		engineConfiguration->torqueReductionEnabled = true;
		config->torqueReductionCutXaxis = GPPWM_Tps;
		config->torqueReductionIgnitionRetardXaxis = GPPWM_Tps;
		setLinearCurve(config->torqueReductionCutXBins, 0, 100, 1);
		setLinearCurve(config->torqueReductionIgnitionRetardXBins, 0, 100, 1);
		setLinearCurve(config->torqueReductionCutGearBins, 0, 5, 1);
		setLinearCurve(config->torqueReductionIgnitionRetardGearBins, 0, 5, 1);
		for (size_t gear = 0; gear < TORQUE_TABLE_Y_SIZE; gear++) {
			for (size_t axis = 0; axis < TORQUE_TABLE_X_SIZE; axis++) {
				config->torqueReductionIgnitionCutTable[gear][axis] =
					config->torqueReductionCutXBins[axis];
				config->torqueReductionIgnitionRetardTable[gear][axis] =
					20 - config->torqueReductionIgnitionRetardXBins[axis] * 0.3f;
			}
		}
		Sensor::setMockValue(SensorType::DetectedGear, 2);
		Sensor::resetMockValue(SensorType::Tps1);
		InvalidFlatShiftTpsSensor sensor;
		ASSERT_TRUE(sensor.Register());
		engine->shiftTorqueReductionController.isFlatShiftConditionSatisfied = true;
		sensor.payload = 0;
		ASSERT_FALSE(Sensor::get(SensorType::Tps1));
		EXPECT_FLOAT_EQ(0, engine->shiftTorqueReductionController.getSparkSkipRatio());
		EXPECT_FLOAT_EQ(0, engine->shiftTorqueReductionController.getTorqueReductionIgnitionRetard());
		sensor.payload = 100;
		ASSERT_FALSE(Sensor::get(SensorType::Tps1));
		EXPECT_FLOAT_EQ(0, engine->shiftTorqueReductionController.getSparkSkipRatio());
		EXPECT_FLOAT_EQ(0, engine->shiftTorqueReductionController.getTorqueReductionIgnitionRetard());

		engineConfiguration->torqueReductionActivationMode = TORQUE_REDUCTION_BUTTON;
		engineConfiguration->torqueReductionTriggerPin = Gpio::Unassigned;
		engineConfiguration->limitTorqueReductionTime = true;
		config->torqueReductionTimeXaxis = GPPWM_Zero;
		setTable(config->torqueReductionTimeTable, 1000);
		engine->engineState.lua.torqueReductionState = true;
		engine->shiftTorqueReductionController.update();
		ASSERT_TRUE(engine->shiftTorqueReductionController.isTimeConditionSatisfied);
		config->torqueReductionTimeXaxis = GPPWM_Tps;
		engine->shiftTorqueReductionController.update();
		EXPECT_FALSE(engine->shiftTorqueReductionController.isTimeConditionSatisfied);
		engineConfiguration->limitTorqueReductionTime = false;
		engine->shiftTorqueReductionController.update();
		EXPECT_TRUE(engine->shiftTorqueReductionController.isTimeConditionSatisfied);
	}
}
