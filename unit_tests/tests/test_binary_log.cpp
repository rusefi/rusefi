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
		0,	// type
		// name
		'n', 'a', 'm', 'e', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		// units
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
