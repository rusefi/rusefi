#include "pch.h"
#include "../../../firmware/hw_layer/ports/at32/at32_reset_cause.h"
#include "gpio/l9779_spi.h"

TEST(At32ResetCause, ResetFlags) {
	// Reproduce the port reporting Unknown for every documented reset source.
	for (unsigned bit = 26; bit <= 31; bit++) {
		EXPECT_EQ(Reset_Cause_Unknown, decodeAt32ResetCause(1U << bit));
	}
}

TEST(At32ResetCause, ReservedBits) {
	EXPECT_EQ(Reset_Cause_Unknown, decodeAt32ResetCause(0));
	// Artery CRM_CTRLSTS bit 25 is reserved, despite the STM32 BOR alias.
	EXPECT_EQ(Reset_Cause_Unknown, decodeAt32ResetCause(1U << 25));
}

TEST(L9779Spi, ArrayWriteParity) {
	// Reproduce the array path sending an even-parity frame unchanged.
	EXPECT_EQ(0x0000, l9779PrepareSpiWord(0x0000));
	EXPECT_EQ(0x0600, l9779PrepareSpiWord(0x0600));
	EXPECT_EQ(0x0200, l9779PrepareSpiWord(0x0200));
}
