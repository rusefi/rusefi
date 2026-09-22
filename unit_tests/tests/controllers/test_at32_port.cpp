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

TEST(L9779Spi, ReadRepliesMatchBySubaddress) {
	L9779ReadTracker tracker;

	EXPECT_TRUE(tracker.push(0x0e));
	EXPECT_TRUE(tracker.push(0x0f));
	EXPECT_EQ(static_cast<size_t>(2), tracker.size());

	// A later request may be answered first without losing the older one.
	EXPECT_TRUE(tracker.consume(0x0f));
	EXPECT_EQ(static_cast<size_t>(1), tracker.size());

	// An unrelated/stale reply does not shift the outstanding request stream.
	EXPECT_FALSE(tracker.consume(0x01));
	EXPECT_EQ(static_cast<size_t>(1), tracker.size());
	EXPECT_TRUE(tracker.consume(0x0e));
	EXPECT_EQ(static_cast<size_t>(0), tracker.size());
}

TEST(L9779Spi, ReadTrackerBoundsOutstandingRequests) {
	L9779ReadTracker tracker;

	for (size_t i = 0; i < L9779ReadTracker::Capacity; i++) {
		EXPECT_TRUE(tracker.push(static_cast<uint8_t>(i)));
	}

	EXPECT_FALSE(tracker.push(0xff));
	EXPECT_EQ(L9779ReadTracker::Capacity, tracker.size());

	tracker.clear();
	EXPECT_EQ(static_cast<size_t>(0), tracker.size());
}

TEST(L9779Spi, FrameLogRetainsNewestFramesInChronologicalOrder) {
	L9779SpiFrameLog log;

	for (size_t i = 0; i < L9779SpiFrameLog::Capacity + 2; i++) {
		log.record(
			static_cast<uint16_t>(0x1000 + i),
			static_cast<uint16_t>(0x2000 + i),
			static_cast<uint8_t>(i),
			-static_cast<int>(i));
	}

	ASSERT_EQ(L9779SpiFrameLog::Capacity, log.size());
	const L9779SpiFrame* oldest = log.get(0);
	ASSERT_NE(nullptr, oldest);
	EXPECT_EQ(0x1002, oldest->tx);
	EXPECT_EQ(0x2002, oldest->rx);
	EXPECT_EQ(2, oldest->subaddress);
	EXPECT_EQ(-2, oldest->result);

	const L9779SpiFrame* newest = log.get(log.size() - 1);
	ASSERT_NE(nullptr, newest);
	EXPECT_EQ(0x1000 + L9779SpiFrameLog::Capacity + 1, newest->tx);
	EXPECT_EQ(0x2000 + L9779SpiFrameLog::Capacity + 1, newest->rx);
	EXPECT_EQ(L9779SpiFrameLog::Capacity + 1, newest->subaddress);
	EXPECT_EQ(-static_cast<int>(L9779SpiFrameLog::Capacity + 1), newest->result);
	EXPECT_EQ(nullptr, log.get(log.size()));
}

TEST(L9779Spi, DirectDriveChannelsUsePermanentEnableMask) {
	const L9779OutputRegisters packed = l9779PackOutputRegisters(
		0,
		L9779_DIRECT_DRIVE_MASK);

	EXPECT_EQ(L9779_DIRECT_DRIVE_MASK, packed.enabledState);
	EXPECT_EQ(0xf8, packed.control[0]); // OUT1..5
	EXPECT_EQ(0x0f, packed.control[1]); // IGN1..4
	EXPECT_EQ(0x03, packed.control[2]); // OUT6..7
	EXPECT_EQ(0x00, packed.control[3]);

	// A logical high cannot enable a direct channel with no physical input.
	const L9779OutputRegisters unavailable = l9779PackOutputRegisters(
		L9779_DIRECT_DRIVE_MASK,
		0);
	EXPECT_EQ(0U, unavailable.enabledState);
}

TEST(L9779Spi, Output13And14PackIntoTheirOwnControlBits) {
	constexpr uint32_t Out13 = uint32_t{1} << 16;
	constexpr uint32_t Out14 = uint32_t{1} << 17;

	const L9779OutputRegisters out13 = l9779PackOutputRegisters(Out13, 0);
	EXPECT_EQ(0x00, out13.control[1]);
	EXPECT_EQ(0x10, out13.control[2]);

	const L9779OutputRegisters out14 = l9779PackOutputRegisters(Out14, 0);
	EXPECT_EQ(0x40, out14.control[1]);
	EXPECT_EQ(0x00, out14.control[2]);
}
