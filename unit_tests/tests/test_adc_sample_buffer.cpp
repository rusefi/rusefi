#include "pch.h"
#include "adc_sample_buffer.h"

TEST(AdcSampleBuffer, UnsupportedAdc3ChannelIsInvalid) {
	// Allocate the neighboring storage too, to reproduce the logical overflow
	// without undefined memory access in the test itself.
	volatile uint16_t memory[40] = {};
	memory[0] = 123;
	memory[15] = 4095;
	memory[32] = 2310;
	memory[39] = 3210;
	EXPECT_EQ(123, readSlowAdcSample(memory, 16, 0));
	EXPECT_EQ(4095, readSlowAdcSample(memory, 16, 15));
	EXPECT_EQ(-1, readSlowAdcSample(memory, 16, EFI_ADC_32 - EFI_ADC_0));
	EXPECT_EQ(-1, readSlowAdcSample(memory, 16, EFI_ADC_39 - EFI_ADC_0));
	EXPECT_EQ(-1, readSlowAdcSample(memory, 16, EFI_ADC_NONE - EFI_ADC_0));
	EXPECT_EQ(-1, readSlowAdcSample(memory, 16, 16));
}

TEST(AdcSampleBuffer, PortSpecificCapacity) {
	volatile uint16_t samples[40] = {};
	samples[31] = 1023;
	samples[39] = 4095;
	EXPECT_EQ(1023, readSlowAdcSample(samples, 32, 31));
	EXPECT_EQ(-1, readSlowAdcSample(samples, 32, 32));
	EXPECT_EQ(4095, readSlowAdcSample(samples, 40, 39));
	EXPECT_EQ(-1, readSlowAdcSample(samples, 40, 40));
	EXPECT_EQ(-1, readSlowAdcSample(samples, 0, 0));
}
