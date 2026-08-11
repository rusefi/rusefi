/**
 * @file test_dc_hardware_pool.cpp
 *
 * Coverage for the shared DcHardware pool in dc_motors.cpp: the ETB/DC-actuator
 * functions own slots [0..ETB_COUNT) while the H-bridge idle stepper claims slots
 * [ETB_COUNT..ETB_COUNT+DC_PER_STEPPER) - see initIdleHardware().
 * Also covers the pickEtbOrStepper() conflict guard between those two users.
 *
 * https://github.com/rusefi/rusefi/pull/9466 refactors this pool - these tests pin
 * down the legacy behavior that refactoring must preserve (or consciously change).
 */

#include "pch.h"

#include "dc_motors.h"
#include "electronic_throttle.h"

TEST(DcHardwarePool, EtbAndStepperSlotsAreDistinct) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// ETB functions use the dc_io overload with slots 0..ETB_COUNT-1
	DcMotor* etb0 = initDcMotor("ETB disable", engineConfiguration->etbIo[0], 0, false);
	DcMotor* etb1 = initDcMotor("ETB disable", engineConfiguration->etbIo[1], 1, false);
	// the H-bridge idle stepper claims the two slots right after the ETBs, see initIdleHardware()
	DcMotor* stepperA = initDcMotor(Gpio::Unassigned, Gpio::Unassigned, ETB_COUNT + 0);
	DcMotor* stepperB = initDcMotor(Gpio::Unassigned, Gpio::Unassigned, ETB_COUNT + 1);

	ASSERT_NE(nullptr, etb0);
	ASSERT_NE(nullptr, etb1);
	ASSERT_NE(nullptr, stepperA);
	ASSERT_NE(nullptr, stepperB);

	// all four users must get their own hardware
	EXPECT_NE(etb0, etb1);
	EXPECT_NE(etb0, stepperA);
	EXPECT_NE(etb0, stepperB);
	EXPECT_NE(etb1, stepperA);
	EXPECT_NE(etb1, stepperB);
	EXPECT_NE(stepperA, stepperB);

	// driving one slot must not leak into any other slot
	stepperA->set(0.7f);
	EXPECT_FLOAT_EQ(0.7f, stepperA->get());
	EXPECT_FLOAT_EQ(0, etb0->get());
	EXPECT_FLOAT_EQ(0, etb1->get());
	EXPECT_FLOAT_EQ(0, stepperB->get());
}

TEST(DcHardwarePool, SlotIdentityIsStable) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	DcMotor* first = initDcMotor("ETB disable", engineConfiguration->etbIo[0], 0, false);

	// re-initialization of a started slot is a no-op returning the same hardware,
	// even via the other overload - the isStarted latch protects the slot
	EXPECT_EQ(first, initDcMotor("ETB disable", engineConfiguration->etbIo[0], 0, true));
	EXPECT_EQ(first, initDcMotor(Gpio::Unassigned, Gpio::Unassigned, 0));
	EXPECT_EQ(first, getDcMotorForUnitTest(0));
}

TEST(DcHardwarePool, ConsoleDutyCommandRoutesToSlot) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// start an ETB slot and a stepper slot
	initDcMotor("ETB disable", engineConfiguration->etbIo[0], 0, false);
	initDcMotor(Gpio::Unassigned, Gpio::Unassigned, ETB_COUNT + 0);

	// console 'set_dc' style control must reach the same motor the subsystem got at init;
	// note that today indices beyond ETB_COUNT address the idle stepper's motors
	setDcMotorDuty(0, 0.25f);
	setDcMotorDuty(ETB_COUNT + 0, -0.5f);

	EXPECT_FLOAT_EQ(0.25f, getDcMotorForUnitTest(0)->get());
	EXPECT_TRUE(getDcMotorForUnitTest(0)->isOpenDirection());

	EXPECT_FLOAT_EQ(-0.5f, getDcMotorForUnitTest(ETB_COUNT + 0)->get());
	EXPECT_FALSE(getDcMotorForUnitTest(ETB_COUNT + 0)->isOpenDirection());
}

TEST(DcHardwarePool, TooHighEtbFrequencyIsFatal) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// TEST_ENGINE configures DC_Wastegate, so slot 0 was already started during engine
	// initialization and the isStarted latch would swallow the re-init - reset the pool
	// so start() actually runs with the bad frequency
	resetDcHardwareForUnitTest();

	engineConfiguration->etbFreq = ETB_HW_MAX_FREQUENCY + 1;
	EXPECT_FATAL_ERROR(initDcMotor("ETB disable", engineConfiguration->etbIo[0], 0, false));
}

// pickEtbOrStepper(): H-bridge idle stepper and DC functions compete for the same
// hardware pool, so selecting both at once is a critical configuration error.
// PR #9466 gives the stepper its own pool - once that lands, this guard (and these
// tests) are expected to be consciously removed.

TEST(DcHardwarePool, PickEtbOrStepperAllowsDcFunctionsWithoutHbridgeStepper) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->useHbridgesToDriveIdleStepper = false;
	engineConfiguration->etbFunctions[0] = DC_Throttle1;

	EXPECT_NO_FATAL_ERROR(pickEtbOrStepper());
}

TEST(DcHardwarePool, PickEtbOrStepperAllowsHbridgeStepperAlone) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->useHbridgesToDriveIdleStepper = true;
	engineConfiguration->etbFunctions[0] = DC_None;
	engineConfiguration->etbFunctions[1] = DC_None;

	EXPECT_NO_FATAL_ERROR(pickEtbOrStepper());
}

TEST(DcHardwarePool, PickEtbOrStepperRejectsConflict) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->useHbridgesToDriveIdleStepper = true;
	engineConfiguration->etbFunctions[0] = DC_None;
	engineConfiguration->etbFunctions[1] = DC_Wastegate;

	EXPECT_FATAL_ERROR(pickEtbOrStepper());
}
