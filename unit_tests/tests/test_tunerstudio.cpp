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

#define PAYLOAD "123"
#define SIZE strlen(PAYLOAD)

static void assertCrcPacket(BufferTsChannel& dut) {
	ASSERT_EQ(dut.writeIdx, SIZE + 7);

	// todo: proper uint16 comparison
	ASSERT_EQ(st5TestBuffer[0], 0);
	ASSERT_EQ(st5TestBuffer[1], SIZE + 1);

	ASSERT_EQ(st5TestBuffer[2], TS_RESPONSE_OK);

	ASSERT_EQ(memcmp(&st5TestBuffer[3], PAYLOAD, SIZE), 0);


	// todo: proper uint32 comparison
	ASSERT_EQ(st5TestBuffer[6], 86);
	EXPECT_EQ(st5TestBuffer[7], 77);
	EXPECT_EQ(st5TestBuffer[8], 101);
	EXPECT_EQ(st5TestBuffer[9], 220);
}

TEST(binary, testWriteCrc) {
	BufferTsChannel test;

	// Small impl
	test.reset();
	test.copyAndWriteSmallCrcPacket((const uint8_t*)PAYLOAD, SIZE);
	assertCrcPacket(test);

	// Large impl
	test.reset();
	test.writeCrcPacketLocked((const uint8_t*)PAYLOAD, SIZE);
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
	instance.handleWriteChunkCommand(&channel, 100, 1, &val);

	EXPECT_EQ(configBytes[100], 50);
}
