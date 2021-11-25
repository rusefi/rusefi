#include "pch.h"

#include "closed_loop_fuel_cell.h"
#include "closed_loop_fuel.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

class MockClCell : public ClosedLoopFuelCellBase {
public:
	MOCK_METHOD(float, getLambdaError, (), (const));
	MOCK_METHOD(float, getMaxAdjustment, (), (const));
	MOCK_METHOD(float, getMinAdjustment, (), (const));
	MOCK_METHOD(float, getIntegratorGain, (), (const));
};

TEST(ClosedLoopCell, TestDeadband) {
	StrictMock<MockClCell> cl;

	// Error is more than deadtime, so nothing else should be called
	EXPECT_CALL(cl, getLambdaError())
		.WillOnce(Return(0.05f));

	cl.update(0.1f, true);

	// Should be zero adjustment
	EXPECT_FLOAT_EQ(cl.getAdjustment(), 1.0f);
}

TEST(ClosedLoopFuelCell, AdjustRate) {
	StrictMock<MockClCell> cl;

	EXPECT_CALL(cl, getLambdaError())
		.WillOnce(Return(0.1f));
	EXPECT_CALL(cl, getMinAdjustment())
		.WillOnce(Return(-0.2f));
	EXPECT_CALL(cl, getMaxAdjustment())
		.WillOnce(Return(0.2f));
	EXPECT_CALL(cl, getIntegratorGain())
		.WillOnce(Return(2.0f));

	cl.update(0.0f, false);

	// Should have integrated 0.2 * dt
	// dt = 1000.0f / FAST_CALLBACK_PERIOD_MS
	EXPECT_FLOAT_EQ(cl.getAdjustment(), 1 + (0.2f / (1000.0f / FAST_CALLBACK_PERIOD_MS)));
}

TEST(ClosedLoopFuel, CellSelection) {
	stft_s cfg;

	// Sensible region config
	cfg.maxIdleRegionRpm = 1500 / RPM_1_BYTE_PACKING_MULT;
	cfg.minPowerLoad = 80;
	cfg.maxOverrunLoad = 30;

	// Test idle
	EXPECT_EQ(0, computeStftBin(1000, 10, cfg));
	EXPECT_EQ(0, computeStftBin(1000, 50, cfg));
	EXPECT_EQ(0, computeStftBin(1000, 90, cfg));

	// Test overrun
	EXPECT_EQ(1, computeStftBin(2000, 10, cfg));
	EXPECT_EQ(1, computeStftBin(4000, 10, cfg));
	EXPECT_EQ(1, computeStftBin(10000, 10, cfg));

	// Test load
	EXPECT_EQ(2, computeStftBin(2000, 90, cfg));
	EXPECT_EQ(2, computeStftBin(4000, 90, cfg));
	EXPECT_EQ(2, computeStftBin(10000, 90, cfg));

	// Main cell
	EXPECT_EQ(3, computeStftBin(2000, 50, cfg));
	EXPECT_EQ(3, computeStftBin(4000, 50, cfg));
	EXPECT_EQ(3, computeStftBin(10000, 50, cfg));
}

TEST(ClosedLoopFuel, afrLimits) {
	EngineTestHelper eth(TEST_ENGINE);

	engineConfiguration->stft.minAfr = 100;  // 10.0 AFR
	engineConfiguration->stft.maxAfr = 180;  // 18.0 AFR

	Sensor::setMockValue(SensorType::Lambda1, 0.1f);
	EXPECT_FALSE(shouldUpdateCorrection(SensorType::Lambda1));

	Sensor::setMockValue(SensorType::Lambda1, 1.0f);
	EXPECT_TRUE(shouldUpdateCorrection(SensorType::Lambda1));

	Sensor::setMockValue(SensorType::Lambda1, 2.0f);
	EXPECT_FALSE(shouldUpdateCorrection(SensorType::Lambda1));
}
