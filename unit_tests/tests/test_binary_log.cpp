#include "log_field.h"

#include <gmock/gmock.h>

TEST(BinaryLogField, FieldHeader) {
	scaled_channel<int8_t, 10> channel;
	LogField field(channel, "name", "units", 2);

	char buffer[64];
	memset(buffer, 0xAA, sizeof(buffer));

	// Should write 55 bytes
	EXPECT_EQ(55, field.writeHeader(buffer));

	// Expect correctly written header
	EXPECT_THAT(buffer, ElementsAre(
		0,	// type: int8_t
		// name - 34 bytes, 0 padded
		'n', 'a', 'm', 'e', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		// units - 10 bytes, 0 padded
		'u', 'n', 'i', 't', 's', 0, 0, 0, 0, 0,
		// display style: float
		0,
		// Scale (float, grumble)
		0, 0, 0, 0,
		// Transform - we always use 0
		0, 0, 0, 0,
		// Digits - 2, as configured
		2,

		// After end should be 0xAA, not touched
		0xAA, 0xAA, 0xAA
	));
}

TEST(BinaryLogField, Value) {
	uint32_t testValue = 0x12345678;
	LogField lf(testValue, "test", "unit", 0);

	char buffer[6];

	// Should write 4 bytes
	EXPECT_EQ(4, lf.writeData(buffer));

	// Check that big endian data was written, and bytes after weren't touched
	EXPECT_THAT(buffer, ElementsAre(0x78, 0x56, 0x34, 0x12, 0xAA, 0xAA));
}
