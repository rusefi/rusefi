#include "linear_sensor.h"

#include "unit_test_framework.h"

class SensorLinear : public ::testing::Test {
protected:
	// Maps (1, 4) -> (100, -100)
	LinearSensor dut;

	SensorLinear()
		: dut(SensorType::Clt) {}

	void SetUp() override {
		dut.configure(1, 100, 4, -100, -110, 110);
	}
};

#define test_point(in, out)                                                                                            \
	{                                                                                                                  \
		dut.postRawValue(in);                                                                                          \
		auto result = dut.get();                                                                                       \
                                                                                                                       \
		EXPECT_TRUE(result.Valid);                                                                                     \
		ASSERT_NEAR(result.Value, (out), EPS4D) << "Not " << out << " for " << in;                                     \
	}

#define test_point_invalid(in)                                                                                         \
	{                                                                                                                  \
		dut.postRawValue(in);                                                                                          \
		EXPECT_FALSE(dut.get().Valid);                                                                                 \
	}

TEST_F(SensorLinear, TestInRange) {
	test_point(2.5, 0);
	test_point(1, 100);
	test_point(4, -100);
}

TEST_F(SensorLinear, TestOutOfRange) {
	test_point(1, 100);
	test_point_invalid(0.5);

	test_point(4, -100);
	test_point_invalid(4.5);
}
