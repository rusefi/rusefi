#include "pch.h"

#include "dac.h"

#if EFI_UNIT_TEST
DACDriver unitTestDacDriver1;
DACDriver unitTestDacDriver2;

void dacStart(DACDriver* driver, DACConfig*)
{
	++driver->startCount;
}

void dacPutChannelX(DACDriver* driver, int channel, unsigned value)
{
	++driver->putCount;
	driver->lastChannel = channel;
	driver->lastValue = value;
}
#endif

class DacTest : public ::testing::Test {
protected:
	EngineTestHelper engineTestHelper{engine_type_e::TEST_ENGINE};

	void SetUp() override
	{
		resetDacForUnitTest();
		unitTestDacDriver1 = {};
		unitTestDacDriver2 = {};
		engineConfiguration->dacOutputPins[0] = Gpio::Unassigned;
		engineConfiguration->dacOutputPins[1] = Gpio::Unassigned;
		engineConfiguration->adcVcc = 5;
	}
};

TEST_F(DacTest, RetainedScriptWriteDoesNothingWhenFreshBootHasNoConfiguredOutputs)
{
	initDac();
	setDacVoltage(0, 2.5f);

	EXPECT_EQ(unitTestDacDriver1.startCount, 0);
	EXPECT_EQ(unitTestDacDriver2.startCount, 0);
	EXPECT_EQ(unitTestDacDriver1.putCount, 0);
	EXPECT_EQ(unitTestDacDriver2.putCount, 0);
}

TEST_F(DacTest, ConfiguredOutputStartsAndClampsVoltage)
{
	engineConfiguration->dacOutputPins[0] = Gpio::A0;
	initDac();

	setDacVoltage(0, 6.0f);

	EXPECT_EQ(unitTestDacDriver1.startCount, 1);
	EXPECT_EQ(unitTestDacDriver2.startCount, 1);
	EXPECT_EQ(unitTestDacDriver1.putCount, 1);
	EXPECT_EQ(unitTestDacDriver1.lastChannel, 0);
	EXPECT_EQ(unitTestDacDriver1.lastValue, 4095u);
	EXPECT_EQ(unitTestDacDriver2.putCount, 0);
}

TEST_F(DacTest, UnconfiguredChannelStaysDisabledWhenOtherOutputStartsHardware)
{
	engineConfiguration->dacOutputPins[0] = Gpio::A0;
	initDac();

	setDacVoltage(1, 2.5f);
	setDacVoltage(0, 2.5f);

	EXPECT_EQ(unitTestDacDriver1.putCount, 1);
	EXPECT_EQ(unitTestDacDriver2.putCount, 0);
}

TEST_F(DacTest, RepeatedInitDoesNotRestartHardwareAndClearedOutputRejectsWrites)
{
	engineConfiguration->dacOutputPins[0] = Gpio::A0;
	initDac();
	EXPECT_EQ(unitTestDacDriver1.startCount, 1);
	EXPECT_EQ(unitTestDacDriver2.startCount, 1);

	engineConfiguration->dacOutputPins[0] = Gpio::Unassigned;
	engineConfiguration->dacOutputPins[1] = Gpio::Unassigned;
	initDac();
	initDac();
	setDacVoltage(0, 2.5f);
	setDacVoltage(1, 2.5f);

	EXPECT_EQ(unitTestDacDriver1.startCount, 1);
	EXPECT_EQ(unitTestDacDriver2.startCount, 1);
	EXPECT_EQ(unitTestDacDriver1.putCount, 0);
	EXPECT_EQ(unitTestDacDriver2.putCount, 0);
}

TEST_F(DacTest, InvalidOutputIndicesAreIgnored)
{
	engineConfiguration->dacOutputPins[0] = Gpio::A0;
	initDac();

	setDacVoltage(-1, 2.5f);
	setDacVoltage(DAC_OUTPUT_COUNT, 2.5f);

	EXPECT_EQ(unitTestDacDriver1.putCount, 0);
	EXPECT_EQ(unitTestDacDriver2.putCount, 0);
}
