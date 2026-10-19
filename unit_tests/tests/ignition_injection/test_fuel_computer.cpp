#include "pch.h"

// sneaky...
#define protected public
#include "fuel_computer.h"

#include "stored_value_sensor.h"

using ::testing::FloatEq;

class MockFuelComputer : public FuelComputerBase {
public:
	MOCK_METHOD(float, getStoichiometricRatio, (), (const, override));
	MOCK_METHOD(float, getTargetLambda, (float rpm, float load), (const, override));
	MOCK_METHOD(float, getTargetLambdaLoadAxis, (float defaultLoad), (const, override));
};

TEST(FuelComputer, getCycleFuel) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	MockFuelComputer dut;

	EXPECT_CALL(dut, getTargetLambdaLoadAxis(FloatEq(0.8f)))
		.WillOnce(Return(0.8f));
	EXPECT_CALL(dut, getStoichiometricRatio())
		.WillOnce(Return(3.0f));
	EXPECT_CALL(dut, getTargetLambda(1000, FloatEq(0.8f)))
		.WillOnce(Return(5.0f));

	auto result = dut.getCycleFuel(7.0f, 1000, 0.8f);
	EXPECT_FLOAT_EQ(result, 7.0f / (5 * 3));
}

TEST(FuelComputer, FlexFuel) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	FuelComputer dut;

	// easier values for testing
	engineConfiguration->stoichRatioPrimary = 15;
	engineConfiguration->stoichRatioSecondary = 10;

	// No sensor -> returns primary
	Sensor::resetMockValue(SensorType::FuelEthanolPercent);
	EXPECT_FLOAT_EQ(15.0f, dut.getStoichiometricRatio());

	// E0 -> primary afr
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 0);
	EXPECT_FLOAT_EQ(15.0f, dut.getStoichiometricRatio());

	// E50 -> half way between
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 50);
	EXPECT_FLOAT_EQ(12.5f, dut.getStoichiometricRatio());

	// E100 -> secondary afr
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 100);
	EXPECT_FLOAT_EQ(10.0f, dut.getStoichiometricRatio());

	// E(-10) -> clamp to primary
	Sensor::setMockValue(SensorType::FuelEthanolPercent, -10);
	EXPECT_FLOAT_EQ(15.0f, dut.getStoichiometricRatio());

	// E110 -> clamp to secondary
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 110);
	EXPECT_FLOAT_EQ(10.0f, dut.getStoichiometricRatio());
}

namespace {

class InvalidEthanolSensor final : public Sensor {
public:
	InvalidEthanolSensor() : Sensor(SensorType::FuelEthanolPercent) {}
	~InvalidEthanolSensor() { unregister(); }

	SensorResult get() const override {
		SensorResult result = payload;
		result.Valid = false;
		return result;
	}

	void showInfo(const char*) const override {}
	float payload = 0;
};

} // namespace

TEST(FuelComputer, InvalidEthanolPayloadDoesNotControlFuelStoichiometry) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	Sensor::resetMockValue(SensorType::FuelEthanolPercent);
	InvalidEthanolSensor sensor;
	ASSERT_TRUE(sensor.Register());
	ASSERT_TRUE(Sensor::hasSensor(SensorType::FuelEthanolPercent));
	ASSERT_FALSE(Sensor::get(SensorType::FuelEthanolPercent));
	engineConfiguration->stoichRatioPrimary = 15;
	engineConfiguration->stoichRatioSecondary = 10;
	FuelComputer dut;

	// Invalid readings must use primary stoichiometry regardless of payload.
	sensor.payload = 0;
	EXPECT_FLOAT_EQ(15, dut.getStoichiometricRatio());
	sensor.payload = 100;
	EXPECT_FLOAT_EQ(15, dut.getStoichiometricRatio());
}
