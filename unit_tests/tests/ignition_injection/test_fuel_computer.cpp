#include "engine_test_helper.h"

// sneaky...
#define protected public
#include "fuel_computer.h"
#include "mocks.h"

#include "gtest/gtest.h"

using ::testing::FloatEq;

class MockFuelComputer : public FuelComputerBase {
public:
	MOCK_METHOD(float, getStoichiometricRatio, (), (const, override));
	MOCK_METHOD(float, getTargetLambda, (int rpm, float load), (const, override));
	MOCK_METHOD(float, getTargetLambdaLoadAxis, (float defaultLoad), (const, override));
};

TEST(FuelComputer, getCycleFuel) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	MockFuelComputer dut;
	INJECT_ENGINE_REFERENCE(&dut);

	EXPECT_CALL(dut, getTargetLambdaLoadAxis(FloatEq(0.8f)))
		.WillOnce(Return(0.8f));
	EXPECT_CALL(dut, getStoichiometricRatio())
		.WillOnce(Return(3.0f));
	EXPECT_CALL(dut, getTargetLambda(1000, FloatEq(0.8f)))
		.WillOnce(Return(5.0f));

	auto result = dut.getCycleFuel(7.0f, 1000, 0.8f);
	EXPECT_FLOAT_EQ(result, 7.0f / (5 * 3));
}

TEST(FuelComputer, LambdaLookup) {
	MockVp3d lambdaTable;
	FuelComputer dut(lambdaTable);

	EXPECT_CALL(lambdaTable, getValue(1500, FloatEq(0.7f)))
		.WillOnce(Return(0.85f));

	EXPECT_FLOAT_EQ(dut.getTargetLambda(1500, 0.7f), 0.85f);
}
