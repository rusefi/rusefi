#include "engine_test_helper.h"
#include "unit_test_framework.h"
#include <gtest/gtest.h>

TEST(gpio, testPinInitNonInverted) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	OutputPin dut;

	// Set the "hardware" pin to on initially
	setMockState(GPIOA_6, true);
	ASSERT_TRUE(efiReadPin(GPIOA_6));

	// this should turn it off
	pin_output_mode_e mode = OM_DEFAULT;
	dut.initPin("test", GPIOA_6, &mode);

	EXPECT_FALSE(efiReadPin(GPIOA_6));
}

TEST(gpio, testPinInitInverted) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	OutputPin dut;

	// Set the "hardware" pin to off initially
	setMockState(GPIOA_6, false);
	ASSERT_FALSE(efiReadPin(GPIOA_6));

	// this should turn it off
	pin_output_mode_e mode = OM_INVERTED;
	dut.initPin("test", GPIOA_6, &mode);

	EXPECT_TRUE(efiReadPin(GPIOA_6));
}

TEST(gpio, multipleInit) {
	OutputPin dut;

	// Initial setup should be ok
	EXPECT_NO_FATAL_ERROR(dut.initPin("testPin", GPIOA_6));

	// Reinit with the same pin should be ok
	EXPECT_NO_FATAL_ERROR(dut.initPin("testPin", GPIOA_6));

	// Reinit with DIFFERENT pin should fail
	EXPECT_FATAL_ERROR(dut.initPin("testPin", GPIOB_5));
}

TEST(gpio, deInit) {
	OutputPin dut;

	// Initial setup should be ok
	EXPECT_NO_FATAL_ERROR(dut.initPin("testPin", GPIOA_6));

	dut.deInit();

	// Reinit with DIFFERENT pin should work after deinit
	EXPECT_NO_FATAL_ERROR(dut.initPin("testPin", GPIOB_5));
}

TEST(gpio, pinSetNotInverted) {
	OutputPin dut;

	pin_output_mode_e mode = OM_DEFAULT;
	dut.initPin("test", GPIOA_6, &mode);

	// Initial state should be logic 0
	EXPECT_FALSE(efiReadPin(GPIOA_6));

	dut.setValue(true);
	EXPECT_TRUE(efiReadPin(GPIOA_6));
	EXPECT_TRUE(dut.getLogicValue());

	dut.setValue(false);
	EXPECT_FALSE(efiReadPin(GPIOA_6));
	EXPECT_FALSE(dut.getLogicValue());
}

TEST(gpio, pinSetInverted) {
	OutputPin dut;

	pin_output_mode_e mode = OM_INVERTED;
	dut.initPin("test", GPIOA_6, &mode);

	// Initial state should be logic 0
	EXPECT_TRUE(efiReadPin(GPIOA_6));

	dut.setValue(true);
	EXPECT_FALSE(efiReadPin(GPIOA_6));
	EXPECT_TRUE(dut.getLogicValue());

	dut.setValue(false);
	EXPECT_TRUE(efiReadPin(GPIOA_6));
	EXPECT_FALSE(dut.getLogicValue());
}
