#include <gtest/gtest.h>
#include "../../../firmware/hw_layer/ports/at32/at32_reset_cause.h"
#include "gpio/l9779_spi.h"
#include "../../../firmware/hw_layer/ports/at32/at32f4/cfg/mcuconf.h"

// ADC callbacks validate their NVIC priority against EFI_IRQ_ADC_PRIORITY.
TEST(At32IrqPriority, AdcMatchesCallbackExpectation) {
	EXPECT_EQ(EFI_IRQ_ADC_PRIORITY, STM32_ADC_IRQ_PRIORITY);
}

TEST(At32IrqPriority, AdcDmaMatchesCallbackExpectation) {
	EXPECT_EQ(EFI_IRQ_ADC_PRIORITY, STM32_ADC_ADC1_DMA_IRQ_PRIORITY);
	EXPECT_EQ(EFI_IRQ_ADC_PRIORITY, STM32_ADC_ADC2_DMA_IRQ_PRIORITY);
	EXPECT_EQ(EFI_IRQ_ADC_PRIORITY, STM32_ADC_ADC3_DMA_IRQ_PRIORITY);
}

TEST(At32ResetCause, ResetFlags) {
	EXPECT_EQ(Reset_Cause_NRST_Pin, decodeAt32ResetCause(1U << 26));
	EXPECT_EQ(Reset_Cause_POR, decodeAt32ResetCause(1U << 27));
	EXPECT_EQ(Reset_Cause_Soft_Reset, decodeAt32ResetCause(1U << 28));
	EXPECT_EQ(Reset_Cause_IWatchdog, decodeAt32ResetCause(1U << 29));
	EXPECT_EQ(Reset_Cause_WWatchdog, decodeAt32ResetCause(1U << 30));
	EXPECT_EQ(Reset_Cause_Illegal_Mode, decodeAt32ResetCause(1U << 31));
}

TEST(At32ResetCause, InternalResetTakesPrecedenceOverPinFlag) {
	EXPECT_EQ(Reset_Cause_IWatchdog, decodeAt32ResetCause((1U << 26) | (1U << 29)));
	EXPECT_EQ(Reset_Cause_Soft_Reset, decodeAt32ResetCause((1U << 26) | (1U << 28)));
	EXPECT_EQ(Reset_Cause_POR, decodeAt32ResetCause((1U << 27) | (1U << 29)));
	EXPECT_EQ(Reset_Cause_IWatchdog, decodeAt32ResetCause((1U << 25) | (1U << 29)));
}

TEST(At32ResetCause, ReservedBits) {
	EXPECT_EQ(Reset_Cause_Unknown, decodeAt32ResetCause(0));
	// Artery CRM_CTRLSTS bit 25 is reserved, despite the STM32 BOR alias.
	EXPECT_EQ(Reset_Cause_Unknown, decodeAt32ResetCause(1U << 25));
}

TEST(L9779Spi, ArrayWriteParity) {
	// Both single and array writes use the same parity encoder.
	EXPECT_EQ(0x0001, l9779PrepareSpiWord(0x0000));
	EXPECT_EQ(0x0601, l9779PrepareSpiWord(0x0600));
	EXPECT_EQ(0x0200, l9779PrepareSpiWord(0x0200));
}

TEST(L9779Spi, AllPayloadsPreserveDataAndHaveOddParity) {
	for (uint32_t payload = 0; payload <= 0xffff; payload++) {
		const uint16_t wire = l9779PrepareSpiWord(static_cast<uint16_t>(payload));
		EXPECT_EQ(payload & 0xfffeU, wire & 0xfffeU);
		unsigned ones = 0;
		for (unsigned bit = 0; bit < 16; bit++) {
			ones += (wire >> bit) & 1U;
		}
		EXPECT_EQ(1U, ones % 2U);
		EXPECT_EQ(wire, l9779PrepareSpiWord(wire));
	}
}
