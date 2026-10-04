#include "pch.h"
#include "../../firmware/hw_layer/ports/stm32/stm32_adc_channel_map.h"

TEST(AdcChannelMap, Adc3OnlyInputs) {
    EXPECT_EQ(-1, getStm32AdcInternalChannel(1, EFI_ADC_32));
    EXPECT_EQ(-1, getStm32AdcInternalChannel(1, EFI_ADC_39));
    EXPECT_EQ(-1, getStm32AdcInternalChannel(2, EFI_ADC_32));
    EXPECT_EQ(-1, getStm32AdcInternalChannel(2, EFI_ADC_39));
    EXPECT_EQ(4, getStm32AdcInternalChannel(4, EFI_ADC_32));
    EXPECT_EQ(15, getStm32AdcInternalChannel(4, EFI_ADC_39));
}

TEST(AdcChannelMap, AllControllerMembershipsAndSequencePositions) {
    for (uint8_t mask : {1, 2, 4}) {
        int expectedIndex = 0;
        for (const auto& entry : adcChannels) {
            if (entry.adc & mask) {
                EXPECT_EQ(expectedIndex++, getStm32AdcInternalChannel(mask, entry.ch));
            } else {
                EXPECT_EQ(-1, getStm32AdcInternalChannel(mask, entry.ch));
            }
        }
        EXPECT_EQ(16, expectedIndex);
        EXPECT_EQ(-1, getStm32AdcInternalChannel(mask, EFI_ADC_NONE));
        EXPECT_EQ(-1, getStm32AdcInternalChannel(mask, EFI_ADC_16));
    }
    EXPECT_EQ(-1, getStm32AdcInternalChannel(0, EFI_ADC_0));
}
