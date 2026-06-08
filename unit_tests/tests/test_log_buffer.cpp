#include "pch.h"

// https://github.com/rusefi/rusefi/issues/10159 - a truncated message (think
// massive Lua print) must keep both its null terminator and its trailing
// LOG_DELIMITER framing
TEST(logBuffer, terminateLogLineTruncated) {
	char buffer[16];
	// simulate chvsnprintf of a 100-char message into 16 bytes:
	// 15 chars written, null terminator in the last byte, returns 100
	memset(buffer, 'A', 15);
	buffer[15] = '\0';

	size_t len = priv::terminateLogLine(buffer, sizeof(buffer), 100);

	EXPECT_EQ(15u, len);
	EXPECT_EQ(LOG_DELIMITER[0], buffer[14]);
	EXPECT_EQ('\0', buffer[15]);
	EXPECT_EQ(15u, std::strlen(buffer));
}

TEST(logBuffer, terminateLogLineShortMessageUntouched) {
	char buffer[16] = "abc" LOG_DELIMITER;

	size_t len = priv::terminateLogLine(buffer, sizeof(buffer), 4);

	EXPECT_EQ(4u, len);
	EXPECT_STREQ("abc" LOG_DELIMITER, buffer);
}

// boundary: a message of exactly bufferSize - 1 chars fits with its null
// terminator and must not be modified
TEST(logBuffer, terminateLogLineExactFitUntouched) {
	char buffer[16];
	memset(buffer, 'A', 14);
	buffer[14] = LOG_DELIMITER[0];
	buffer[15] = '\0';

	size_t len = priv::terminateLogLine(buffer, sizeof(buffer), 15);

	EXPECT_EQ(15u, len);
	EXPECT_EQ(LOG_DELIMITER[0], buffer[14]);
	EXPECT_EQ('A', buffer[13]);
	EXPECT_EQ('\0', buffer[15]);
}
