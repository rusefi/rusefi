#include "pch.h"

#include "linear_func.h"

class LinearFuncTest : public ::testing::Test {
protected:
	// Maps (1, 4) -> (100, -100)
	LinearFunc dut;

	void SetUp() override {
		dut.configure(1, 100, 4, -100, -110, 110);
	}
};

#define test_point(in, out)                                                                                            \
	{                                                                                                                  \
		auto result = dut.convert(in);                                                                                 \
                                                                                                                       \
		EXPECT_TRUE(result.Valid);                                                                                     \
		ASSERT_NEAR(result.Value, (out), EPS4D) << "Not " << out << " for " << in;                                     \
	}

#define test_point_invalid(in)                                                                                         \
	{ EXPECT_FALSE(dut.convert(in).Valid); }

TEST_F(LinearFuncTest, TestInRange) {
	test_point(2.5, 0);
	test_point(1, 100);
	test_point(4, -100);
}

TEST_F(LinearFuncTest, TestOutOfRange) {
	test_point(1, 100);
	test_point_invalid(0.5);

	test_point(4, -100);
	test_point_invalid(4.5);
}
