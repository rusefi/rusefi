#include "pch.h"

TEST(gpio, testPinInitNonInverted) {
	EngineTestHelper eth(TEST_ENGINE);

	OutputPin dut;

	// Set the "hardware" pin to on initially
	setMockState(Gpio::A6, true);
	ASSERT_TRUE(efiReadPin(Gpio::A6));

	// this should turn it off
	pin_output_mode_e mode = OM_DEFAULT;
	dut.initPin("test", Gpio::A6, &mode);

	EXPECT_FALSE(efiReadPin(Gpio::A6));
}

TEST(gpio, testPinInitInverted) {
	EngineTestHelper eth(TEST_ENGINE);

	OutputPin dut;

	// Set the "hardware" pin to off initially
	setMockState(Gpio::A6, false);
	ASSERT_FALSE(efiReadPin(Gpio::A6));

	// this should turn it off
	pin_output_mode_e mode = OM_INVERTED;
	dut.initPin("test", Gpio::A6, &mode);

	EXPECT_TRUE(efiReadPin(Gpio::A6));
}

TEST(gpio, multipleInit) {
	OutputPin dut;

	// Initial setup should be ok
	EXPECT_NO_FATAL_ERROR(dut.initPin("testPin", Gpio::A6));

	// Reinit with the same pin should be ok
	EXPECT_NO_FATAL_ERROR(dut.initPin("testPin", Gpio::A6));

	// Reinit with DIFFERENT pin should fail
	EXPECT_FATAL_ERROR(dut.initPin("testPin", Gpio::B5));
}

TEST(gpio, deInit) {
	OutputPin dut;

	// Initial setup should be ok
	EXPECT_NO_FATAL_ERROR(dut.initPin("testPin", Gpio::A6));

	dut.deInit();

	// Reinit with DIFFERENT pin should work after deinit
	EXPECT_NO_FATAL_ERROR(dut.initPin("testPin", Gpio::B5));
}

TEST(gpio, pinSetNotInverted) {
	OutputPin dut;

	pin_output_mode_e mode = OM_DEFAULT;
	dut.initPin("test", Gpio::A6, &mode);

	// Initial state should be logic 0
	EXPECT_FALSE(efiReadPin(Gpio::A6));

	dut.setValue(true);
	EXPECT_TRUE(efiReadPin(Gpio::A6));
	EXPECT_TRUE(dut.getLogicValue());

	dut.setValue(false);
	EXPECT_FALSE(efiReadPin(Gpio::A6));
	EXPECT_FALSE(dut.getLogicValue());
}

TEST(gpio, pinSetInverted) {
	OutputPin dut;

	pin_output_mode_e mode = OM_INVERTED;
	dut.initPin("test", Gpio::A6, &mode);

	// Initial state should be logic 0
	EXPECT_TRUE(efiReadPin(Gpio::A6));

	dut.setValue(true);
	EXPECT_FALSE(efiReadPin(Gpio::A6));
	EXPECT_TRUE(dut.getLogicValue());

	dut.setValue(false);
	EXPECT_TRUE(efiReadPin(Gpio::A6));
	EXPECT_FALSE(dut.getLogicValue());
}
