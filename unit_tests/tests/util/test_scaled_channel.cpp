#include "pch.h"

#include "boost_control.h"

using ::testing::_;
using ::testing::StrictMock;

TEST(ScaledChannel, Basic) {
	{
		scaled_channel<int16_t, 10> i1;
		i1 = 10;    EXPECT_FLOAT_EQ(i1, 10);
		i1 =  1;    EXPECT_FLOAT_EQ(i1,  1);
		i1 =  0.11; EXPECT_FLOAT_EQ(i1,  0.1); // round
		i1 =  0.1;  EXPECT_FLOAT_EQ(i1,  0.1);
		i1 =  0.09; EXPECT_FLOAT_EQ(i1,  0.1); // round
		i1 =  0.01; EXPECT_FLOAT_EQ(i1,  0.); // out of range
		i1 = -0.09; EXPECT_FLOAT_EQ(i1, -0.1); // round
		i1 = -0.1;  EXPECT_FLOAT_EQ(i1, -0.1);
		i1 = -0.11; EXPECT_FLOAT_EQ(i1, -0.1); // round
	}

	{
		scaled_channel<int16_t, 1, 10> i2;
		i2 = 1000; EXPECT_FLOAT_EQ(i2, 1000);
		i2 =  100; EXPECT_FLOAT_EQ(i2,  100);
		i2 =   11; EXPECT_FLOAT_EQ(i2,   10); // round
		i2 =   10; EXPECT_FLOAT_EQ(i2,   10);
		i2 =    9; EXPECT_FLOAT_EQ(i2,   10); // round
		i2 =    1; EXPECT_FLOAT_EQ(i2,    0); // out of range
		i2 =   -9; EXPECT_FLOAT_EQ(i2,  -10); // round
		i2 =  -10; EXPECT_FLOAT_EQ(i2,  -10);
		i2 =  -11; EXPECT_FLOAT_EQ(i2,  -10); // round
	}
}
