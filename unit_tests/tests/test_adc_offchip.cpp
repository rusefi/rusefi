#include "pch.h"
#include "adc_offchip.h"

namespace {
class TestAdcChip : public AdcChip {
public:
	expected<AdcSample> readAdc(size_t channel) override {
		lastChannel = channel;
		return valid ? expected<AdcSample>({raw, voltage}) : expected<AdcSample>(unexpected);
	}

	size_t lastChannel = 0;
	bool valid = true;
	int raw = 32768;
	float voltage = 2.5f;
};

class OffchipAdc : public testing::Test {
	void SetUp() override { resetAdcChipsForUnitTest(); }
	void TearDown() override { resetAdcChipsForUnitTest(); }
};
}

TEST_F(OffchipAdc, UnregisteredAndInvalidChannels) {
	for (int i = 0; i <= EFI_ADC_ERROR; i++) {
		auto channel = static_cast<adc_channel_e>(i);
		EXPECT_FALSE(adcOffchipRead(channel));
		EXPECT_EQ(adcOffchipGetPin(channel), Gpio::Invalid);
	}
}

TEST_F(OffchipAdc, AllEightChannelsAndNativeVoltage) {
	TestAdcChip chip;
	ASSERT_TRUE(adcchipRegister(EFI_ADC_40, chip, Gpio::EXTIOCHIP_0_IO_1, 8));
	for (int i = 0; i < 8; i++) {
		auto channel = static_cast<adc_channel_e>(EFI_ADC_40 + i);
		auto sample = adcOffchipRead(channel);
		ASSERT_TRUE(sample);
		EXPECT_EQ(chip.lastChannel, static_cast<size_t>(i));
		EXPECT_EQ(sample.Value.raw, 32768);
		EXPECT_FLOAT_EQ(sample.Value.voltage, 2.5f);
		EXPECT_EQ(static_cast<int>(adcOffchipGetPin(channel)), static_cast<int>(Gpio::EXTIOCHIP_0_IO_1) + i);
	}
	chip.valid = false;
	EXPECT_FALSE(adcOffchipRead(EFI_ADC_40));
	chip.valid = true;
	chip.raw = 0;
	chip.voltage = 0;
	auto zero = adcOffchipRead(EFI_ADC_47);
	ASSERT_TRUE(zero);
	EXPECT_FLOAT_EQ(zero.Value.voltage, 0);
}

TEST_F(OffchipAdc, RejectsInvalidRangesAndOverlapsWithoutPartialRegistration) {
	TestAdcChip first;
	TestAdcChip second;
	EXPECT_FALSE(adcchipRegister(EFI_ADC_39, first, Gpio::EXTIOCHIP_0_IO_1, 8));
	EXPECT_FALSE(adcchipRegister(EFI_ADC_40, first, Gpio::EXTIOCHIP_0_IO_1, 0));
	EXPECT_FALSE(adcchipRegister(EFI_ADC_41, first, Gpio::EXTIOCHIP_0_IO_1, 8));
	EXPECT_FALSE(adcchipRegister(EFI_ADC_ERROR, first, Gpio::EXTIOCHIP_0_IO_1, 1));
	ASSERT_TRUE(adcchipRegister(EFI_ADC_44, first, Gpio::EXTIOCHIP_0_IO_1, 4));
	EXPECT_FALSE(adcchipRegister(EFI_ADC_40, second, Gpio::EXTIOCHIP_0_IO_5, 8));
	EXPECT_FALSE(adcOffchipRead(EFI_ADC_40));
	ASSERT_TRUE(adcchipRegister(EFI_ADC_40, second, Gpio::EXTIOCHIP_0_IO_5, 4));
	second.voltage = 3.3f;
	EXPECT_FLOAT_EQ(adcOffchipRead(EFI_ADC_43).Value.voltage, 3.3f);
	EXPECT_FLOAT_EQ(adcOffchipRead(EFI_ADC_47).Value.voltage, 2.5f);
}
