
#include "closed_loop_fuel_cell.h"

#include "engine.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

class MockClCell : public ClosedLoopFuelCellBase {
public:
	MOCK_METHOD(float, getLambdaError, (DECLARE_ENGINE_PARAMETER_SIGNATURE), (const));
	MOCK_METHOD(float, getMaxAdjustment, (), (const));
	MOCK_METHOD(float, getMinAdjustment, (), (const));
	MOCK_METHOD(float, getIntegratorGain, (), (const));
};

TEST(ClosedLoopCell, TestDeadband) {
	StrictMock<MockClCell> cl;

	// Error is more than deadtime, so nothing else should be called
	EXPECT_CALL(cl, getLambdaError(_, _, _))
		.WillOnce(Return(0.05f));

	cl.update(0.1f, true, nullptr, nullptr, nullptr);

	// Should be zero adjustment
	EXPECT_FLOAT_EQ(cl.getAdjustment(), 1.0f);
}

TEST(ClosedLoopFuelCell, AdjustRate) {
	StrictMock<MockClCell> cl;

	// Error is more than deadtime, so nothing else should be called
	EXPECT_CALL(cl, getLambdaError(_, _, _))
		.WillOnce(Return(0.1f));
	EXPECT_CALL(cl, getMinAdjustment())
		.WillOnce(Return(-0.2f));
	EXPECT_CALL(cl, getMaxAdjustment())
		.WillOnce(Return(0.2f));
	EXPECT_CALL(cl, getIntegratorGain())
		.WillOnce(Return(2.0f));

	cl.update(0.0f, false, nullptr, nullptr, nullptr);

	// Should have integrated 0.2 * dt
	// dt = 1000.0f / FAST_CALLBACK_PERIOD_MS
	EXPECT_FLOAT_EQ(cl.getAdjustment(), 1 + (0.2f / (1000.0f / FAST_CALLBACK_PERIOD_MS)));
}
