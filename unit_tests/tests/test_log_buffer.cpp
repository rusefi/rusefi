#include "pch.h"

using ::testing::ElementsAre;

TEST(logBuffer, writeSmall) {
	LogBuffer<10> dut;
	memset(dut.m_buffer, 0x55, sizeof(dut.m_buffer));

	LogLineBuffer line;

	strcpy(line.buffer, "test");

	dut.writeLine(&line);

	EXPECT_THAT(dut.m_buffer, ElementsAre(
		't', 'e', 's', 't', '\0',	// this part got copied in
		0x55, 0x55, 0x55, 0x55, 0x55	// rest of the buffer is untouched
	));
}

TEST(logBuffer, writeOverflow) {
	LogBuffer<10> dut;
	memset(dut.m_buffer, 0x55, sizeof(dut.m_buffer));

	LogLineBuffer line;

	strcpy(line.buffer, "testtesttest");

	dut.writeLine(&line);

	EXPECT_THAT(dut.m_buffer, ElementsAre(
		't', 'e', 's', 't',
		't', 'e', 's', 't',
		't', 0
	));
}
