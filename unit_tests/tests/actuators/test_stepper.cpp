#include "pch.h"

#include "stepper.h"

TEST(Stepper, IgnoreSmallChanges) {
	StepperMotorBase dut;

	dut.setTargetPosition(10);

	// Record initial reported position
	auto initialPosition = dut.getTargetPosition();

	// Small changes should be ignored
	dut.setTargetPosition(10.5f);
	EXPECT_EQ(initialPosition, dut.getTargetPosition());
	dut.setTargetPosition(9.5f);
	EXPECT_EQ(initialPosition, dut.getTargetPosition());

	// Change of >= 1 should cause a change
	dut.setTargetPosition(11.5f);
	EXPECT_EQ(11.5f, dut.getTargetPosition());

	// Now go back the other way
	dut.setTargetPosition(9.5f);
	EXPECT_EQ(9.5f, dut.getTargetPosition());
}
