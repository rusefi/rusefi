#include "pch.h"

#include "dc_motor.h"

using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::StrictMock;

TEST(DcMotor, Disable) {
	StrictMock<MockOutputPin> dpin;

	EXPECT_CALL(dpin, setValue(1))
		.Times(2);	// happens twice - once for initial disable, once for set(0)

	TwoPinDcMotor dut(dpin);
}

TEST(DcMotor, Disable2) {
	StrictMock<MockOutputPin> dpin;

	EXPECT_CALL(dpin, setValue(1)).Times(4);

	TwoPinDcMotor dut(dpin);

	dut.disable();
}

TEST(DcMotor, Enable) {
	StrictMock<MockOutputPin> dpin;

	{
		InSequence is;

		// Construction disables
		EXPECT_CALL(dpin, setValue(1)).Times(2);

		// Then enable
		EXPECT_CALL(dpin, setValue(0));
	}

	TwoPinDcMotor dut(dpin);
	dut.enable();
}

TEST(DcMotor, SetUnconfigured) {
	StrictMock<MockOutputPin> dpin;
	EXPECT_CALL(dpin, setValue(1)).Times(3);

	TwoPinDcMotor dut(dpin);
	EXPECT_FLOAT_EQ(dut.get(), 0);
	EXPECT_NO_THROW(dut.set(0.5f));

	// Readback should work even without configuration
	EXPECT_FLOAT_EQ(dut.get(), 0.5f);
}

TEST(DcMotor, PwmEnablePinModePositive) {
	NiceMock<MockOutputPin> dpin;
	TwoPinDcMotor dut(dpin);
	dut.setType(TwoPinDcMotor::ControlType::PwmEnablePin);

	MockPwm enable;
	MockPwm dir1;
	MockPwm dir2;

	EXPECT_CALL(enable, setSimplePwmDutyCycle(0.5f));
	EXPECT_CALL(dir1, setSimplePwmDutyCycle(1));
	EXPECT_CALL(dir2, setSimplePwmDutyCycle(0));

	dut.configure(enable, dir1, dir2);
	dut.set(0.5f);
}

TEST(DcMotor, PwmEnablePinModeNegative) {
	NiceMock<MockOutputPin> dpin;
	TwoPinDcMotor dut(dpin);
	dut.setType(TwoPinDcMotor::ControlType::PwmEnablePin);

	MockPwm enable;
	MockPwm dir1;
	MockPwm dir2;
	

	EXPECT_CALL(enable, setSimplePwmDutyCycle(0.5f));
	EXPECT_CALL(dir1, setSimplePwmDutyCycle(0));
	EXPECT_CALL(dir2, setSimplePwmDutyCycle(1));

	dut.configure(enable, dir1, dir2);
	dut.set(-0.5f);
}

TEST(DcMotor, PwmDirectionPinsModePositive) {
	NiceMock<MockOutputPin> dpin;
	TwoPinDcMotor dut(dpin);
	dut.setType(TwoPinDcMotor::ControlType::PwmDirectionPins);

	MockPwm enable;
	MockPwm dir1;
	MockPwm dir2;
	

	EXPECT_CALL(enable, setSimplePwmDutyCycle(1));
	EXPECT_CALL(dir1, setSimplePwmDutyCycle(0.5f));
	EXPECT_CALL(dir2, setSimplePwmDutyCycle(0));

	dut.configure(enable, dir1, dir2);
	dut.set(0.5f);
}

TEST(DcMotor, PwmDirectionPinsModeNegative) {
	NiceMock<MockOutputPin> dpin;
	TwoPinDcMotor dut(dpin);
	dut.setType(TwoPinDcMotor::ControlType::PwmDirectionPins);

	MockPwm enable;
	MockPwm dir1;
	MockPwm dir2;
	

	EXPECT_CALL(enable, setSimplePwmDutyCycle(1));
	EXPECT_CALL(dir1, setSimplePwmDutyCycle(0));
	EXPECT_CALL(dir2, setSimplePwmDutyCycle(0.5f));

	dut.configure(enable, dir1, dir2);
	dut.set(-0.5f);
}
