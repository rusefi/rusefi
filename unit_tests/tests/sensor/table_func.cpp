#include "pch.h"

#include "table_func.h"

TEST(TableFuncTest, basic) {
	float in[] = { 0, 10 };
	float out[] = { 30, 40 };

	TableFunc dut(in, out);

	EXPECT_EQ(30, dut.convert(-10).value_or(0));
	EXPECT_EQ(30, dut.convert(0).value_or(0));
	EXPECT_EQ(35, dut.convert(5).value_or(0));
	EXPECT_EQ(40, dut.convert(10).value_or(0));
	EXPECT_EQ(40, dut.convert(20).value_or(0));
}

TEST(TableFuncTest, scaled) {
	uint16_t in[] = { 0, 1000, 2000 };
	uint8_t out[] = { 70, 60, 50 };

	TableFunc<uint16_t, uint8_t, 3, 
		// Input units are 1/1000
		efi::ratio<1, 1000>,
		// output units are 1/100
		efi::ratio<1, 100>>
			dut(in, out);

	EXPECT_EQ(0.65f, dut.convert(0.5f).value_or(0));
	EXPECT_EQ(0.55f, dut.convert(1.5f).value_or(0));
}
