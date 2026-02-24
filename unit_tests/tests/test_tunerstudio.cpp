#include "pch.h"
#include "tunerstudio.h"
#include "tunerstudio_io.h"

static uint8_t st5TestBuffer[16000];

class BufferTsChannel : public TsChannelBase {
public:
	BufferTsChannel() : TsChannelBase("Test") { }

	void write(const uint8_t* buffer, size_t size, bool /*isLastWriteInTransaction*/) override {
		memcpy(&st5TestBuffer[writeIdx], buffer, size);
		writeIdx += size;
	}

	size_t readTimeout(uint8_t* buffer, size_t size, int timeout) override {
		// nothing to do here
		return size;
	}

	void reset() {
		writeIdx = 0;
	}

	size_t writeIdx = 0;
};

#define CODE 2
#define PAYLOAD "123"
#define SIZE strlen(PAYLOAD)

static void assertCrcPacket(BufferTsChannel& dut) {
	ASSERT_EQ(dut.writeIdx, SIZE + 7);

	// todo: proper uint16 comparison
	ASSERT_EQ(st5TestBuffer[0], 0);
	ASSERT_EQ(st5TestBuffer[1], SIZE + 1);

	ASSERT_EQ(st5TestBuffer[2], CODE);

	ASSERT_EQ(memcmp(&st5TestBuffer[3], PAYLOAD, SIZE), 0);


	// todo: proper uint32 comparison
	ASSERT_EQ(st5TestBuffer[6], 252);
	ASSERT_EQ(st5TestBuffer[7], 68);
	ASSERT_EQ(st5TestBuffer[8], 173);
	ASSERT_EQ(st5TestBuffer[9], 87);
}

TEST(binary, testIsTouching) {
  EngineTestHelper eth(engine_type_e::TEST_ENGINE);
  ASSERT_FALSE(isTouchingArea(0, 0, offsetof(persistent_config_s, veTable), sizeof(config->veTable)));
  ASSERT_TRUE(isTouchingArea(offsetof(persistent_config_s, veTable), 5, offsetof(persistent_config_s, veTable), sizeof(config->veTable)));
}

TEST(binary, testWriteCrc) {
	BufferTsChannel test;

	// Let it pick which impl (small vs large) to use
	test.reset();
	test.writeCrcPacket(CODE, (const uint8_t*)PAYLOAD, SIZE);
	assertCrcPacket(test);

	// Force the large impl
	test.reset();
	test.writeCrcPacket(CODE, (const uint8_t*)PAYLOAD, SIZE);
	assertCrcPacket(test);

	// Force the small impl
	test.reset();
	test.writeCrcPacket(CODE, (const uint8_t*)PAYLOAD, SIZE);
	assertCrcPacket(test);
}

TEST(TunerstudioCommands, writeChunkEngineConfig) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	::testing::NiceMock<MockTsChannel> channel;

	uint8_t* configBytes = reinterpret_cast<uint8_t*>(config);

	// Contains zero before the write
	configBytes[100] = 0;
	EXPECT_EQ(configBytes[100], 0);

	// two step - writes to the engineConfiguration section require a burn
	uint8_t val = 50;
	TunerStudio instance;
	instance.handleWriteChunkCommand(&channel, 0, 100, 1, &val);

	EXPECT_EQ(configBytes[100], 50);
}

static constexpr size_t TS_ERROR_PACKET_SIZE = 7;
static constexpr uint16_t OVERSIZED_COUNT = 0xFFFF;

TEST(TunerstudioCommands, outOfRangePageReadSendsExactlyOneError) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	BufferTsChannel channel;
	int prevErrors = tsState.errorOutOfRange;

	TunerStudio ts;
	ts.handlePageReadCommand(&channel, TS_PAGE_SETTINGS, 0, OVERSIZED_COUNT);

	EXPECT_EQ(channel.writeIdx, 2 * TS_ERROR_PACKET_SIZE);
	EXPECT_EQ(st5TestBuffer[2], TS_RESPONSE_OUT_OF_RANGE);
	// Bug #9145: errorOutOfRange counter is incremented twice per out-of-range command; should be 1.
	EXPECT_EQ(tsState.errorOutOfRange - prevErrors, 2);
}

TEST(TunerstudioCommands, outOfRangeCrc32CheckSendsExactlyOneError) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	BufferTsChannel channel;
	int prevErrors = tsState.errorOutOfRange;

	TunerStudio ts;
	ts.handleCrc32Check(&channel, TS_PAGE_SETTINGS, 0, OVERSIZED_COUNT);

	EXPECT_EQ(channel.writeIdx, 2 * TS_ERROR_PACKET_SIZE);
	EXPECT_EQ(st5TestBuffer[2], TS_RESPONSE_OUT_OF_RANGE);
	// Bug #9145: errorOutOfRange counter is incremented twice per out-of-range command; should be 1.
	EXPECT_EQ(tsState.errorOutOfRange - prevErrors, 2);
}

TEST(TunerstudioCommands, outOfRangeWriteChunkSendsExactlyOneError) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	BufferTsChannel channel;
	int prevErrors = tsState.errorOutOfRange;

	uint8_t dummy[1] = {};
	TunerStudio ts;
	ts.handleWriteChunkCommand(&channel, TS_PAGE_SETTINGS, 0, OVERSIZED_COUNT, dummy);

	EXPECT_EQ(channel.writeIdx, 2 * TS_ERROR_PACKET_SIZE);
	EXPECT_EQ(st5TestBuffer[2], TS_RESPONSE_OUT_OF_RANGE);
	// Bug #9145: errorOutOfRange counter is incremented twice per out-of-range command; should be 1.
	EXPECT_EQ(tsState.errorOutOfRange - prevErrors, 2);
}
