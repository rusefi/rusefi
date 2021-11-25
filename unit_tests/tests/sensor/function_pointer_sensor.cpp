#include "pch.h"

#include "function_pointer_sensor.h"

class SensorFunctionPointer : public ::testing::Test {
protected:
	void SetUp() override {
		Sensor::resetRegistry();
	}

	void TearDown() override {
		Sensor::resetRegistry();
	}
};

float testFunc() {
	return 23;
}

TEST_F(SensorFunctionPointer, TestValue) {
	FunctionPointerSensor dut(SensorType::Clt, testFunc);
	ASSERT_TRUE(dut.Register());

	auto result = Sensor::get(SensorType::Clt);
	EXPECT_TRUE(result.Valid);
	EXPECT_FLOAT_EQ(result.Value, 23);
}
