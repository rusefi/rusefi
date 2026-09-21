#include "pch.h"
#include "slcan_frame.h"

TEST(Slcan, ChannelPrefixesAndLegacyFormat) {
	const uint8_t data[] = {0xAA, 0xBB};
	const char* expected[] = {"t1232AABB\r", "&t1232AABB\r", "$t1232AABB\r"};
	char buffer[slcan::FrameBufferSize];
	for (size_t bus = 0; bus < 3; bus++) {
		ASSERT_EQ(strlen(expected[bus]), slcan::formatFrame(buffer, bus, true,
			0x123, false, false, 2, data, false, 0));
		EXPECT_STREQ(expected[bus], buffer);
		slcan::formatFrame(buffer, bus, false, 0x123, false, false, 2, data, false, 0);
		EXPECT_STREQ("t1232AABB\r", buffer);
	}
}

TEST(Slcan, ExtendedMaximumLengthAndTimestamp) {
	const uint8_t data[] = {0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0xFF};
	char buffer[slcan::FrameBufferSize];
	EXPECT_EQ(sizeof(buffer) - 1, slcan::formatFrame(buffer, 2, true,
		0x1FFFFFFF, true, false, 8, data, true, 0xABCD));
	EXPECT_STREQ("$T1FFFFFFF800112233445566FFABCD\r", buffer);
}

TEST(Slcan, RemoteFramesAndInvalidInput) {
	char buffer[slcan::FrameBufferSize];
	slcan::formatFrame(buffer, 1, true, 0x123, false, true, 8, nullptr, true, 0x1234);
	EXPECT_STREQ("&r12381234\r", buffer);
	slcan::formatFrame(buffer, 2, true, 0x123, true, true, 0, nullptr, false, 0);
	EXPECT_STREQ("$R000001230\r", buffer);
	EXPECT_EQ(0u, slcan::formatFrame(buffer, 3, true, 0, false, true, 0, nullptr, false, 0));
	EXPECT_STREQ("", buffer);
	EXPECT_EQ(0u, slcan::formatFrame(buffer, 0, true, 0, false, true, 9, nullptr, false, 0));
}

TEST(Slcan, NewTunesIncludeBusByDefault) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	EXPECT_TRUE(engineConfiguration->canSnifferIncludeBus);
}
