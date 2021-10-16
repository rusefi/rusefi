/**
 * @file	test_gpiochip.cpp
 *
 * @date Mar 12, 2019
 */

#include "pch.h"

#include "gpio/gpio_ext.h"

using ::testing::_;

static int io_state = 0;

static int initcalls = 0;
struct GoodChip : public GpioChip {
	int init() override {
		initcalls++;
		return 0;
	}
};

class TestChip1 : public GoodChip {
	int readPad(size_t pin) override {
		if (pin & 0x01)
			return 1;
		return 0;
	}
};

static TestChip1 testchip1;

class TestChip2 : public GoodChip {
	int writePad(size_t pin, int value) override {
		if (value)
			io_state |=  (1 << value);
		else
			io_state &= ~(1 << value);

		return 0;
	}
};

static TestChip2 testchip2;

static int calls_to_failed_chip = 0;
// This chip fails to start
class TestChip3 : public GpioChip {
	int writePad(size_t pin, int value) override {
		calls_to_failed_chip++;
		return 0;
	}

	int init() override {
		return -1;
	}
};

static TestChip3 testchip3;

TEST(gpioext, testGpioExt) {
	int ret;
	int chip1_base, chip2_base, chip3_base;

	printf("====================================================================================== testGpioExt\r\n");

	/* should fail to register chip with zero gpios */
	EXPECT_FALSE(gpiochip_register((brain_pin_e)(BRAIN_PIN_ONCHIP_LAST + 1), "invalid", testchip1, 0) > 0);

	/* should fail to register chip with base overlapig on-chip gpios */
	EXPECT_FALSE(gpiochip_register((brain_pin_e)(BRAIN_PIN_ONCHIP_LAST - 1), "invalid", testchip1, 0) > 0);

	chip1_base = gpiochip_register((brain_pin_e)(BRAIN_PIN_ONCHIP_LAST + 1), "input only", testchip1, 16);
	EXPECT_TRUE(chip1_base > 0);

	EXPECT_EQ(16, gpiochips_get_total_pins());

	/* should fail to register chip overlapping other one */
	EXPECT_FALSE(gpiochip_register((brain_pin_e)(BRAIN_PIN_ONCHIP_LAST + 1 + 15), "output only", testchip2, 16) > 0);

	chip2_base = gpiochip_register((brain_pin_e)(BRAIN_PIN_ONCHIP_LAST + 1 + 16), "output only", testchip2, 16);
	EXPECT_TRUE(chip2_base > 0);

	/* this chip will fail to init, but should be registered without errors */
	chip3_base = gpiochip_register((brain_pin_e)(BRAIN_PIN_ONCHIP_LAST + 1 + 16 + 16), "failed chip", testchip3, 16);
	EXPECT_TRUE(chip2_base > 0);

	EXPECT_EQ(48, gpiochips_get_total_pins());

	/* init 3 chips, one will fail */
	ret = gpiochips_init();
	EXPECT_EQ(32, ret);

	/* two drivers should be inited */
	EXPECT_EQ(2, initcalls);

	/* gpio reads */
	EXPECT_TRUE(gpiochips_readPad((brain_pin_e)(chip1_base + 0)) == 0);
	EXPECT_TRUE(gpiochips_readPad((brain_pin_e)(chip1_base + 1)) != 0);

	/* gpio write */
	gpiochips_writePad((brain_pin_e)(chip2_base + 0), 0);
	gpiochips_writePad((brain_pin_e)(chip2_base + 1), 1);
	EXPECT_EQ(0x02, io_state);

	/* try to access failed chip */
	EXPECT_FALSE(gpiochips_writePad((brain_pin_e)(chip3_base + 0), 0) >= 0);
	EXPECT_FALSE(gpiochips_writePad((brain_pin_e)(chip3_base + 1), 1) >= 0);
	EXPECT_EQ(0, calls_to_failed_chip);

	/* read/write outside range */
	EXPECT_TRUE(gpiochips_readPad((brain_pin_e)(chip1_base - 1)) < 0);
	EXPECT_TRUE(gpiochips_writePad((brain_pin_e)(chip1_base - 1), 1) < 0);

	EXPECT_TRUE(gpiochips_readPad((brain_pin_e)(chip3_base + 16)) < 0);
	EXPECT_TRUE(gpiochips_writePad((brain_pin_e)(chip3_base + 16), 1) < 0);

}
